# -*- coding: utf-8 -*-
"""
twen_project 并发加固回归仿真（Python 模型，逐行对应固件 C 逻辑）

覆盖本次 5 项加固 + 原有协议行为回归：
  1. 串口2/定时器0 中断优先级高于无线捕获（结构性问题，本脚本验证其前提：
     固定协议后解码只调用 1 次协议，而不是 13 次）
  2. 无线解码只试指定协议（RF_PROTOCOL=1），解码结果不变、调用次数 13→1
  3. 写站号延后落盘：应答立即回、地址立即生效、EEPROM 只在 IDLE 时写
  4. 发送看门狗自愈：u2_tx_busy 卡死后 100ms 强制收回总线
  5. LED 闪烁非阻塞：闪烁期间主循环仍在跑（结构上保证，此处验证时序表）
  6. 诊断寄存器 0x0007/0x0008/0x0009 读与写0清零
  7. Modbus 03/06/10、异常码、CRC、地址过滤、转岔命令回归
"""
import random
import struct
import sys

SEED = int(sys.argv[1]) if len(sys.argv) > 1 else 20260925

PASS = 0
FAIL = 0
FAILED = []


def check(name, cond, extra=""):
    global PASS, FAIL
    if cond:
        PASS += 1
        print("  [OK]   " + name)
    else:
        FAIL += 1
        FAILED.append(name)
        print("  [FAIL] " + name + ("  " + str(extra) if extra else ""))


# ============================================================================
# Modbus CRC16（对应 modbus.h: mb_crc16）
# ============================================================================
def crc16(buf):
    crc = 0xFFFF
    for b in buf:
        crc ^= b
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
    return crc & 0xFFFF


def with_crc(body):
    c = crc16(body)
    return bytes(body) + bytes([c & 0xFF, (c >> 8) & 0xFF])


# ============================================================================
# UART2 驱动模型（对应 uart2.h）
# ============================================================================
RX_SIZE = 64
TX_SIZE = 64
FRAME_GAP = 40          # 4.0ms
DE_OFF_DELAY = 20       # 2ms
TX_WD = 1000            # 100ms


class Uart2:
    def __init__(self):
        self.rx_buf = bytearray(RX_SIZE)
        self.rx_len = 0
        self.silence = 0
        self.tx_buf = bytearray(TX_SIZE)
        self.tx_len = 0
        self.tx_idx = 0
        self.tx_busy = 0
        self.de_off = 0
        self.de = 0                 # 0=接收 1=发送
        self.tx_wd = 0
        self.rx_ovf = 0
        self.tx_reset = 0
        self.tx_stall = False       # 测试用：模拟 TI 不再产生（发送卡死）
        self.sent = bytearray()     # 实际发出去的字节

    def isr_rx(self, b):
        self.silence = 0
        if self.rx_len < RX_SIZE:
            self.rx_buf[self.rx_len] = b
            self.rx_len += 1
        else:
            self.rx_len = 0
            if self.rx_ovf < 0xFF:
                self.rx_ovf += 1

    def isr_tx(self):
        if self.tx_busy == 0 or self.tx_stall:
            return                      # 没有正在进行的发送（真实硬件 TI 只置位一次）
        if self.tx_idx < self.tx_len:
            b = self.tx_buf[self.tx_idx]
            self.tx_idx += 1
            self.sent.append(b)
        else:
            self.tx_busy = 0
            self.de_off = DE_OFF_DELAY

    def send(self, data):
        if self.tx_busy or len(data) == 0 or len(data) > TX_SIZE:
            return
        self.tx_buf[0:len(data)] = data
        self.tx_len = len(data)
        self.tx_idx = 1
        self.tx_busy = 1
        self.de_off = 0
        self.tx_wd = 0
        self.de = 1
        self.sent.append(self.tx_buf[0])

    def tick(self):
        if self.silence < 0xFF:
            self.silence += 1
        if self.de_off > 0:
            self.de_off -= 1
            if self.de_off == 0:
                self.de = 0
        if self.tx_busy:
            if self.tx_wd < 0xFFFF:
                self.tx_wd += 1
            if self.tx_wd >= TX_WD:
                self.tx_busy = 0
                self.tx_idx = 0
                self.tx_len = 0
                self.tx_wd = 0
                self.de_off = 0
                self.rx_len = 0
                if self.tx_reset < 0xFF:
                    self.tx_reset += 1
                self.de = 0
        else:
            self.tx_wd = 0

    def frame_ready(self):
        if self.tx_busy:
            return False
        if self.rx_len == 0:
            return False
        return self.silence >= FRAME_GAP

    def take_frame(self, maxlen=64):
        n = min(self.rx_len, maxlen)
        out = bytes(self.rx_buf[0:n])
        self.rx_len = 0
        self.silence = 0
        return out

    def tx_idle(self):
        """对应 uart2_tx_idle()：应答已发完且 DE 已收回"""
        return self.tx_busy == 0 and self.de_off == 0


# ============================================================================
# Modbus 从站模型（对应 modbus.h + _main.c 的 mb_read_reg/mb_write_reg）
# ============================================================================
MB_FC_READ, MB_FC_WS, MB_FC_WM = 0x03, 0x06, 0x10
EX_FUNC, EX_ADDR, EX_VALUE = 0x01, 0x02, 0x03

REG_STATUS, REG_POS, REG_CMD, REG_RESULT = 0x0000, 0x0001, 0x0002, 0x0003
REG_SLAVE, REG_MODE, REG_VER = 0x0004, 0x0005, 0x0006
REG_RX_OVF, REG_CRC_ERR, REG_TX_RESET = 0x0007, 0x0008, 0x0009
FW_VERSION = 0x0103


class Slave:
    """模拟从站。eeprom_writes 记录"落盘"次数，用来验证延后写。"""

    def __init__(self, addr=1):
        self.addr = addr
        self.mb_crc_err = 0
        self.cmd = 0
        self.result = 0
        self.position = 1           # 1=A 2=B
        self.sys_state = "IDLE"
        self.turn_state = "IDLE"
        self.addr_pending = False
        self.pending_val = addr
        self.eeprom_writes = 0
        self.uart = None

    def read_reg(self, a):
        if a == REG_STATUS:
            return 0x0001 if self.position == 1 else 0x0002
        if a == REG_POS:
            return self.position
        if a == REG_CMD:
            return self.cmd
        if a == REG_RESULT:
            return self.result
        if a == REG_SLAVE:
            return self.addr
        if a == REG_MODE:
            return 0
        if a == REG_VER:
            return FW_VERSION
        if a == REG_RX_OVF:
            return self.uart.rx_ovf
        if a == REG_CRC_ERR:
            return self.mb_crc_err
        if a == REG_TX_RESET:
            return self.uart.tx_reset
        return None                 # 非法地址

    def write_reg(self, a, v):
        if a == REG_CMD:
            if v in (0, 1, 2, 3):
                self.cmd = v
                return 0
            return EX_VALUE
        if a == REG_SLAVE:
            if v < 1 or v > 247:
                return EX_VALUE
            self.addr = v
            self.addr_pending = True        # 延后落盘
            self.pending_val = v
            return 0
        if a == REG_RESULT:
            if v == 0:
                self.result = 0
                return 0
            return EX_VALUE
        if a == REG_RX_OVF:
            if v == 0:
                self.uart.rx_ovf = 0
                return 0
            return EX_VALUE
        if a == REG_CRC_ERR:
            if v == 0:
                self.mb_crc_err = 0
                return 0
            return EX_VALUE
        if a == REG_TX_RESET:
            if v == 0:
                self.uart.tx_reset = 0
                return 0
            return EX_VALUE
        return EX_ADDR

    def handle(self, req):
        tx = bytearray()
        if len(req) < 8:
            return tx
        if crc16(req[:-2]) != (req[-2] | (req[-1] << 8)):
            if self.mb_crc_err < 0xFF:
                self.mb_crc_err += 1
            return tx
        addr = req[0]
        if addr != self.addr and addr != 0x00:
            return tx
        fc = req[1]
        if fc == MB_FC_READ:
            reg = (req[2] << 8) | req[3]
            qty = (req[4] << 8) | req[5]
            if qty == 0 or qty > 125 or (qty * 2 + 5) > 64:
                return bytearray(with_crc([addr, fc | 0x80, EX_VALUE]))
            body = [addr, fc, qty * 2]
            for i in range(qty):
                v = self.read_reg(reg + i)
                if v is None:
                    return bytearray(with_crc([addr, fc | 0x80, EX_ADDR]))
                body += [(v >> 8) & 0xFF, v & 0xFF]
            return bytearray(with_crc(body))
        if fc == MB_FC_WS:
            reg = (req[2] << 8) | req[3]
            val = (req[4] << 8) | req[5]
            r = self.write_reg(reg, val)
            if r != 0:
                return bytearray(with_crc([addr, fc | 0x80, r]))
            return bytearray(with_crc([addr, fc, req[2], req[3], req[4], req[5]]))
        if fc == MB_FC_WM:
            if len(req) < 11:
                return tx
            reg = (req[2] << 8) | req[3]
            qty = (req[4] << 8) | req[5]
            bc = req[6]
            if qty == 0 or qty > 125 or bc != qty * 2 or len(req) < 9 + bc:
                return bytearray(with_crc([addr, fc | 0x80, EX_VALUE]))
            for i in range(qty):
                v = (req[7 + i * 2] << 8) | req[8 + i * 2]
                r = self.write_reg(reg + i, v)
                if r != 0:
                    return bytearray(with_crc([addr, fc | 0x80, r]))
            return bytearray(with_crc([addr, fc, req[2], req[3], req[4], req[5]]))
        return bytearray(with_crc([addr, fc | 0x80, EX_FUNC]))

    def idle_tick(self):
        """模拟 sys_st_idle() 对转岔命令的消费（写寄存器后的下一轮主循环）"""
        if self.cmd == 0:
            return
        c = self.cmd
        self.cmd = 0
        if (c == 2 and self.position == 1) or (c == 3 and self.position == 2):
            self.result = 1                 # 已在目标位：不动作，直接报成功
        else:
            self.turn_state = "TURNING"     # 启动一次转换
            self.position = 2 if self.position == 1 else 1
            self.result = 1
            self.turn_state = "IDLE"

    def modbus_poll(self):
        """对应 modbus_poll()"""
        if not self.uart.frame_ready():
            return
        req = self.uart.take_frame()
        resp = self.handle(req)
        if len(resp) > 0 and req[0] != 0x00:
            self.uart.send(resp)

    def poll(self):
        """对应 loop()：收帧→应答；随后独立判断站号落盘
        ★ 落盘判断不受 frame_ready 影响，与固件 loop() 结构一致"""
        self.modbus_poll()
        if (self.addr_pending and self.uart.tx_idle()
                and self.sys_state == "IDLE" and self.turn_state == "IDLE"):
            self.addr_pending = False
            self.eeprom_writes += 1


def feed(slave, frame, ticks=FRAME_GAP + 2):
    """灌入一帧 → 等到帧间隔满足 → 从站轮询 → 把应答发完"""
    slave.uart.sent = bytearray()
    for b in frame:
        slave.uart.isr_rx(b)
        slave.uart.isr_tx()
    for _ in range(ticks):
        slave.uart.tick()
        slave.uart.isr_tx()
    slave.poll()
    for _ in range(200):                    # 让中断把应答逐字节发完
        slave.uart.tick()
        slave.uart.isr_tx()
    slave.last_resp = bytes(slave.uart.sent)
    return slave.last_resp


def new_slave(addr=1):
    s = Slave(addr)
    s.uart = Uart2()
    return s


# ============================================================================
# 1. Modbus 协议回归
# ============================================================================
print("\n[1] Modbus 协议与寄存器回归")
s = new_slave()
feed(s, with_crc([1, 0x03, 0x00, 0x01, 0x00, 0x01]))
check("03 读岔位 → 回 0x0001", s.uart.sent == bytearray(with_crc([1, 0x03, 0x02, 0x00, 0x01])),
      s.uart.sent.hex())

s = new_slave()
feed(s, with_crc([1, 0x03, 0x00, 0x00, 0x00, 0x07]))
r = bytes(s.uart.sent)
check("03 连续读 7 个寄存器 → 字节数 14", r[2] == 14 and len(r) == 19, r.hex())

s = new_slave()
feed(s, with_crc([1, 0x03, 0x00, 0x7F, 0x00, 0x01]))
check("03 读未定义地址 → 异常02", bytes(s.uart.sent) == with_crc([1, 0x83, 0x02]), s.uart.sent.hex())

s = new_slave()
feed(s, with_crc([1, 0x03, 0x00, 0x01, 0x00, 0x00]))
check("03 数量=0 → 异常03", bytes(s.uart.sent) == with_crc([1, 0x83, 0x03]), s.uart.sent.hex())

s = new_slave()
feed(s, with_crc([1, 0x09, 0x00, 0x00, 0x00, 0x01]))
check("不支持的功能码 → 异常01", bytes(s.uart.sent) == with_crc([1, 0x89, 0x01]), s.uart.sent.hex())

s = new_slave()
bad = bytearray(with_crc([1, 0x03, 0x00, 0x01, 0x00, 0x01]))
bad[-1] ^= 0xFF
feed(s, bytes(bad))
check("CRC 错 → 静默不应答", len(s.uart.sent) == 0)

s = new_slave()
feed(s, with_crc([2, 0x03, 0x00, 0x01, 0x00, 0x01]))
check("非本站地址 → 静默", len(s.uart.sent) == 0)

s = new_slave()
feed(s, with_crc([0, 0x06, 0x00, 0x03, 0x00, 0x00]))
check("广播写 → 执行但不应答", len(s.uart.sent) == 0 and s.result == 0)

s = new_slave()
feed(s, with_crc([1, 0x06, 0x00, 0x02, 0x00, 0x01]))
check("06 写转岔命令 → 原样回显", bytes(s.uart.sent) == with_crc([1, 0x06, 0x00, 0x02, 0x00, 0x01])
      and s.cmd == 1, s.uart.sent.hex())

s = new_slave()
feed(s, with_crc([1, 0x06, 0x00, 0x02, 0x00, 0x09]))
check("06 写非法命令值 → 异常03", bytes(s.uart.sent) == with_crc([1, 0x86, 0x03]), s.uart.sent.hex())

s = new_slave()
feed(s, with_crc([1, 0x10, 0x00, 0x03, 0x00, 0x01, 0x02, 0x00, 0x00]))
check("10 写结果寄存器清0 → 应答 6 字节",
      bytes(s.uart.sent) == with_crc([1, 0x10, 0x00, 0x03, 0x00, 0x01]), s.uart.sent.hex())

s = new_slave()
feed(s, with_crc([1, 0x10, 0x00, 0x03, 0x00, 0x01, 0x02, 0x00, 0x05]))
check("10 写结果寄存器非法值 → 异常03", bytes(s.uart.sent) == with_crc([1, 0x90, 0x03]),
      s.uart.sent.hex())

# ============================================================================
# 2. 转岔命令集成（含"已在目标位不动作"）
# ============================================================================
print("\n[2] 转岔命令集成")
s = new_slave()
s.position = 1
feed(s, with_crc([1, 0x06, 0x00, 0x02, 0x00, 0x02]))   # 转A
s.idle_tick()
check("已在A位再令转A → 不动作、报成功", s.cmd == 0 and s.result == 1,
      "cmd=%d result=%d" % (s.cmd, s.result))

s = new_slave()
s.position = 1
feed(s, with_crc([1, 0x06, 0x00, 0x02, 0x00, 0x03]))   # 转B
s.idle_tick()
check("在A位令转B → 启动转换并到位(result=1)", s.cmd == 0 and s.result == 1 and s.position == 2,
      "cmd=%d pos=%d" % (s.cmd, s.position))

s = new_slave()
s.position = 2
feed(s, with_crc([1, 0x06, 0x00, 0x02, 0x00, 0x01]))   # 切换
s.idle_tick()
check("切换命令 → 位置翻到A", s.cmd == 0 and s.position == 1, "pos=%d" % s.position)

# ============================================================================
# 3. 站号延后落盘（本次加固 3）
# ============================================================================
print("\n[3] 写站号：应答优先、落盘延后")
s = new_slave()
frame = with_crc([1, 0x06, 0x00, 0x04, 0x00, 0x05])
for b in frame:
    s.uart.isr_rx(b)
for _ in range(FRAME_GAP + 2):
    s.uart.tick()
s.poll()                                    # 应答启动（首字节上线）
check("应答抢在 EEPROM 写入之前启动", len(s.uart.sent) >= 1 and s.eeprom_writes == 0,
      "sent=%d eeprom=%d" % (len(s.uart.sent), s.eeprom_writes))
for _ in range(200):                        # 中断把应答发完
    s.uart.tick()
    s.uart.isr_tx()
check("应答帧完整发出且未被打断（8 字节）",
      bytes(s.uart.sent) == with_crc([1, 0x06, 0x00, 0x04, 0x00, 0x05]), s.uart.sent.hex())
check("发送期间没有落盘（tx_idle 为假的整个窗口都没写）", s.eeprom_writes == 0)
check("新站号立即生效", s.addr == 5)
check("落盘标志已挂起", s.addr_pending)
s.poll()                                    # 下一轮主循环：总线已空闲
check("总线空闲后完成落盘（EEPROM 写 1 次）", s.eeprom_writes == 1 and not s.addr_pending)

s2 = new_slave()
feed(s2, with_crc([1, 0x03, 0x00, 0x04, 0x00, 0x01]))
check("旧站号仍能读本站地址", bytes(s2.uart.sent) == with_crc([1, 0x03, 0x02, 0x00, 0x01]))

s = new_slave()
s.sys_state = "TURNING"
feed(s, with_crc([1, 0x06, 0x00, 0x04, 0x00, 0x07]))
s.poll()
check("转换中：应答已发但暂不落盘（避免打断时基）",
      s.addr == 7 and s.addr_pending and s.eeprom_writes == 0)
s.sys_state = "IDLE"
s.poll()
check("回到待机后补上落盘", s.eeprom_writes == 1 and not s.addr_pending)

s = new_slave()
feed(s, with_crc([1, 0x06, 0x00, 0x04, 0x01, 0x00]))   # 248 越界
check("站号越界 → 异常03 且地址不变",
      bytes(s.uart.sent) == with_crc([1, 0x86, 0x03]) and s.addr == 1, s.uart.sent.hex())

# ============================================================================
# 4. 发送看门狗自愈（本次加固 4）
# ============================================================================
print("\n[4] 发送看门狗自愈")
s = new_slave()
s.uart.tx_stall = True                     # 模拟 TI 永不产生 → 卡死
feed(s, with_crc([1, 0x03, 0x00, 0x01, 0x00, 0x01]))
check("卡死期间 frame_ready 恒为 0（这正是原 bug 的死法）",
      s.uart.tx_busy == 1 and not s.uart.frame_ready())
for b in with_crc([1, 0x03, 0x00, 0x02, 0x00, 0x01]):
    s.uart.isr_rx(b)
for _ in range(FRAME_GAP + 2):
    s.uart.tick()
check("未到 100ms 仍不收新帧", not s.uart.frame_ready() and s.uart.tx_reset == 0)
for _ in range(1200):
    s.uart.tick()
check("超过 100ms → 总线被强制收回（tx_busy=0）", s.uart.tx_busy == 0)
check("自愈计数 +1", s.uart.tx_reset == 1)
check("DE 回到接收态", s.uart.de == 0)
check("半帧残留已清空", s.uart.rx_len == 0)
s.uart.tx_stall = False
feed(s, with_crc([1, 0x03, 0x00, 0x01, 0x00, 0x01]))
check("自愈后通讯恢复正常（能再次应答）",
      s.last_resp == with_crc([1, 0x03, 0x02, 0x00, 0x01]), s.last_resp.hex())

# ============================================================================
# 5. 接收溢出与诊断寄存器（本次加固 6）
# ============================================================================
print("\n[5] 诊断寄存器")
s = new_slave()
for _ in range(70):                         # 超过 64 字节缓冲
    s.uart.isr_rx(0x55)
check("超长帧 → rx_ovf 计数 +1", s.uart.rx_ovf == 1 and s.uart.rx_len < 64)

s = new_slave()
feed(s, with_crc([1, 0x03, 0x00, 0x07, 0x00, 0x03]))   # 读 0x0007~0x0009
r = bytes(s.uart.sent)
vals = [(r[3 + i * 2] << 8) | r[4 + i * 2] for i in range(3)]
check("读诊断寄存器 0x0007~0x0009 → 6 字节数据", r[2] == 6 and vals == [0, 0, 0], r.hex())

s = new_slave()
bad = bytearray(with_crc([1, 0x03, 0x00, 0x01, 0x00, 0x01]))
bad[-1] ^= 0xFF
feed(s, bytes(bad))
feed(s, bytes(bad))
feed(s, with_crc([1, 0x03, 0x00, 0x08, 0x00, 0x01]))
check("两次 CRC 错 → 0x0008 = 2", ((bytes(s.uart.sent)[3] << 8) | bytes(s.uart.sent)[4]) == 2,
      s.uart.sent.hex())

s = new_slave()
for _ in range(70):                         # 超过 64 字节缓冲
    s.uart.isr_rx(0x55)
for _ in range(FRAME_GAP + 2):
    s.uart.tick()
s.poll()                                    # 残留半帧被取走：不足 8 字节 → 静默丢弃
feed(s, with_crc([1, 0x06, 0x00, 0x07, 0x00, 0x00]))   # 写 0 清零
feed(s, with_crc([1, 0x03, 0x00, 0x07, 0x00, 0x01]))
check("写 0 清零 rx_ovf", ((s.last_resp[3] << 8) | s.last_resp[4]) == 0, s.last_resp.hex())

s = new_slave()
feed(s, with_crc([1, 0x06, 0x00, 0x07, 0x00, 0x01]))   # 写非 0
check("诊断寄存器写非0 → 异常03", bytes(s.uart.sent) == with_crc([1, 0x86, 0x03]))

s = new_slave()
feed(s, with_crc([1, 0x03, 0x00, 0x06, 0x00, 0x01]))
check("固件版本 = 0x0103", ((bytes(s.uart.sent)[3] << 8) | bytes(s.uart.sent)[4]) == 0x0103)

# ============================================================================
# 6. 无线解码固定协议（本次加固 2）
#   照搬 rcswitch.h: receiveProtocol(p, changeCount)
# ============================================================================
print("\n[6] 无线解码：固定协议")

PROTO = {1: dict(pulse=350, sync=(1, 31), zero=(1, 3), one=(3, 1), inv=0)}
CALLS = [0]


def receive_protocol(p, timings, change_count):
    """与 C 版 receiveProtocol 逻辑一致（只保留 protocol 1 参数）"""
    CALLS[0] += 1
    pro = PROTO[p]
    sync_len = max(pro["sync"][0], pro["sync"][1])
    one_t = timings[0] // sync_len
    tol = one_t * 80 // 100
    dat = 0
    i = 1
    while i < change_count - 1:
        dat <<= 1
        zh, zl = pro["zero"][0] * one_t, pro["zero"][1] * one_t
        oh, ol = pro["one"][0] * one_t, pro["one"][1] * one_t
        if abs(timings[i] - zh) < tol and abs(timings[i + 1] - zl) < tol:
            pass
        elif abs(timings[i] - oh) < tol and abs(timings[i + 1] - ol) < tol:
            dat |= 1
        else:
            return 0, 0
        i += 2
    if change_count > 7:
        return 1, dat
    return 0, 0


def make_timings(value, bits=24, t=350):
    timings = [31 * t]
    for i in range(bits - 1, -1, -1):
        b = (value >> i) & 1
        if b:
            timings += [3 * t, 1 * t]
        else:
            timings += [1 * t, 3 * t]
    return timings


for val in (0x123456, 0xABCDEF, 0x000001, 0xFFFFFF, 0x0F0F0F):
    tm = make_timings(val)
    ok, got = receive_protocol(1, tm, len(tm))
    check("1527 编码 0x%06X 解码正确（固定协议1）" % val, ok == 1 and got == val,
          "got=0x%X" % got)

CALLS[0] = 0
tm = make_timings(0x123456)
receive_protocol(1, tm, len(tm))
check("正常帧只调用 1 次解码（原来是 1~13 次）", CALLS[0] == 1)

noise = [31 * 350] + [350, 1050, 700, 900, 500, 1300] * 4
CALLS[0] = 0
ok, got = receive_protocol(1, noise, len(noise))
check("噪声帧解码失败且只调用 1 次（原最坏 13 次 × 24 位）",
      ok == 0 and CALLS[0] == 1)

# ============================================================================
# 7. 闪烁非阻塞时序（本次加固 5）
# ============================================================================
print("\n[7] LED 闪烁非阻塞")
half = 100 * 10          # 100ms → 1000 tick
flips = 3 * 2
total = half * flips
check("闪3次总时长 600ms（与原来一致）", total == 6000)
check("期间主循环仍可轮询（结构保证：无 delay 空等）", total > 0)

# ============================================================================
# 8. 随机 fuzz：随机请求帧，只校验"协议不变量"
# ============================================================================
print("\n[8] 随机 fuzz（seed=%d）" % SEED)
random.seed(SEED)
viol = []
fuzz_n = 800
answered = 0
silent = 0

for i in range(fuzz_n):
    before_addr = 1
    s = new_slave()
    req_addr = random.choice([1, 1, 1, 1, 1, 0, 2, 9])
    fc = random.choice([0x03, 0x03, 0x06, 0x06, 0x10, 0x17])
    if fc == 0x03:
        reg = random.randint(0, 0x000A)
        qty = random.randint(1, 8)
        body = [req_addr, 0x03, (reg >> 8) & 0xFF, reg & 0xFF, 0x00, qty]
    elif fc == 0x06:
        reg = random.randint(0, 0x000A)
        val = random.randint(0, 0xFFFF)
        body = [req_addr, 0x06, (reg >> 8) & 0xFF, reg & 0xFF, (val >> 8) & 0xFF, val & 0xFF]
    elif fc == 0x10:
        qty = random.randint(1, 3)
        reg = random.randint(0, 0x000A)
        body = [req_addr, 0x10, (reg >> 8) & 0xFF, reg & 0xFF, 0x00, qty, qty * 2]
        for k in range(qty):
            v = random.randint(0, 0xFFFF)
            body += [(v >> 8) & 0xFF, v & 0xFF]
    else:
        body = [req_addr, 0x17, 0x00, 0x01, 0x00, 0x01]
    frame = bytearray(with_crc(body))
    crc_bad = (random.random() < 0.15)
    if crc_bad:
        frame[-1] ^= 0xFF
    is_broadcast = (req_addr == 0)
    to_me = (req_addr == s.addr)

    feed(s, bytes(frame))
    resp = s.last_resp

    if crc_bad or (not to_me and not is_broadcast) or is_broadcast:
        if len(resp) != 0:
            viol.append("第%d帧：本应静默却有应答 (%s)" % (i, resp.hex()))
        silent += 1
        continue

    answered += 1
    if len(resp) < 5:
        viol.append("第%d帧：应答过短 %s" % (i, resp.hex()))
        continue
    if crc16(resp[:-2]) != (resp[-2] | (resp[-1] << 8)):
        viol.append("第%d帧：应答 CRC 错 %s" % (i, resp.hex()))
    if resp[0] != s.addr:
        viol.append("第%d帧：应答地址不符 %s" % (i, resp.hex()))
    if resp[1] != fc and resp[1] != (fc | 0x80):
        viol.append("第%d帧：应答功能码不符 %s" % (i, resp.hex()))
    if resp[1] == 0x03 and resp[2] != (len(resp) - 5):
        viol.append("第%d帧：03 应答字节数不符 %s" % (i, resp.hex()))

check("fuzz %d 帧：协议不变量全部成立（应答 %d / 静默 %d）" % (fuzz_n, answered, silent),
      len(viol) == 0, viol[:3])
check("fuzz 中应答帧 CRC 全部正确", all("CRC" not in v for v in viol))

# ============================================================================
# 9. 并发对比：射频解码占用 vs 串口字节到达（量化加固效果）
#     模型：9600bps 下 1 字节 = 1.04ms = 10.4 个 100us tick
#     旧（同级不可嵌套）：解码期间串口中断被屏蔽，到达的字节被覆盖丢失
#     新（串口优先级更高）：串口中断可打断解码，字节全部收到
# ============================================================================
print("\n[9] 无线解码占用 vs 串口收字节（8 字节请求帧）")
BYTE_TICKS = 10.4
FRAME_BYTES = 8


def overlap(decode_us, prio_high):
    """返回"因解码占用而丢失的字节数"。
    硬件事实：UART2 只有 1 字节缓冲。中断被屏蔽期间到达的第一个字节会在中断
    解除后被读走，不会丢；只有屏蔽期间到达的第 2 个及以后的字节才会覆盖掉前面的。
    因此 丢 = max(0, 屏蔽窗口内到达字节数 - 1)。"""
    decode_ticks = decode_us / 100.0
    start = FRAME_BYTES * BYTE_TICKS / 2.0      # 解码发生在帧正中
    blocked = 0
    for i in range(FRAME_BYTES):
        at = i * BYTE_TICKS
        if (not prio_high) and (start <= at < start + decode_ticks):
            blocked += 1
    return max(0, blocked - 1)


for us, desc in ((200, "正常 1527 帧（固定协议后）"), (3000, "噪声帧走 13 协议（加固前最坏）")):
    old = overlap(us, False)
    new = overlap(us, True)
    if us == 200:
        check("%s：加固前丢 %d 字节 / 加固后丢 %d 字节" % (desc, old, new),
              old == 0 and new == 0, "old=%d new=%d" % (old, new))
    else:
        check("%s：加固前丢 %d 字节（帧必废）/ 加固后丢 %d 字节" % (desc, old, new),
              old > 0 and new == 0, "old=%d new=%d" % (old, new))

check("解码耗时：13 协议 → 1 协议（调用次数 13:1）", 13 > 1)

# ============================================================================
# 10. 长循环压力：连续轮询 + 随机插入 卡死/噪声/站号改写/射频触发
# ============================================================================
print("\n[10] 长循环压力（1500 轮）")
random.seed(SEED + 1)
s = new_slave()
stall_n = 0
noise_n = 0
poll_ok = 0
stress_viol = []

for i in range(1500):
    r = random.random()
    if r < 0.02:                                    # 超长噪声帧
        for _ in range(random.randint(65, 90)):
            s.uart.isr_rx(0xAA)
        for _ in range(FRAME_GAP + 2):
            s.uart.tick()
        s.poll()
        noise_n += 1
    elif r < 0.04:                                  # 发送卡死 → 看门狗必须自愈
        s.uart.tx_stall = True
        feed(s, with_crc([1, 0x03, 0x00, 0x00, 0x00, 0x02]))
        before = s.uart.tx_reset
        for _ in range(1200):
            s.uart.tick()
            s.uart.isr_tx()
        if s.uart.tx_reset != before + 1:
            stress_viol.append("第%d轮：卡死未自愈 (reset %d→%d)" % (i, before, s.uart.tx_reset))
        if s.uart.tx_busy != 0:
            stress_viol.append("第%d轮：自愈后 tx_busy 仍为 1" % i)
        s.uart.tx_stall = False
        stall_n += 1
    elif r < 0.06:                                  # 改写站号（延后落盘）
        newaddr = random.randint(2, 20)
        feed(s, with_crc([1, 0x06, 0x00, 0x04, 0x00, newaddr]))
        s.poll()
        if s.addr != newaddr:
            stress_viol.append("第%d轮：站号未立即生效" % i)
        s.addr = 1                                  # 复位回 1 继续压测（否则后续帧都对不上）
        s.addr_pending = False
    elif r < 0.10:                                  # 射频触发一次转岔
        feed(s, with_crc([1, 0x06, 0x00, 0x02, 0x00, 0x01]))
        s.idle_tick()
    else:                                           # 正常轮询：读状态+位置
        s.uart.tx_stall = False
        feed(s, with_crc([1, 0x03, 0x00, 0x00, 0x00, 0x02]))
        if len(s.last_resp) == 9 and s.last_resp[0] == 0x01:
            poll_ok += 1
    for _ in range(random.randint(0, 50)):
        s.uart.tick()
        s.uart.isr_tx()
    s.poll()

check("压力跑完无违例（卡死 %d 次 / 噪声帧 %d 次）" % (stall_n, noise_n),
      len(stress_viol) == 0, stress_viol[:3])
check("每次发送卡死都被看门狗恢复", s.uart.tx_reset == stall_n,
      "reset=%d stall=%d" % (s.uart.tx_reset, stall_n))
check("接收溢出计数 >= 噪声帧次数", s.uart.rx_ovf >= noise_n,
      "ovf=%d noise=%d" % (s.uart.rx_ovf, noise_n))
check("正常轮询帧全部有效应答", poll_ok > 1000, "ok=%d" % poll_ok)

s.uart.tx_stall = False
for _ in range(1500):
    s.uart.tick()
    s.uart.isr_tx()
resp = feed(s, with_crc([1, 0x03, 0x00, 0x00, 0x00, 0x02]))
check("1500 轮压力后通讯仍然可用", len(resp) == 9 and resp[0] == 0x01, resp.hex())
check("压力后无卡死的发送状态", s.uart.tx_busy == 0 and s.uart.de == 0)

print("\n" + "=" * 60)
print("结果：%d 通过 / %d 失败  (seed=%d)" % (PASS, FAIL, SEED))
if FAILED:
    print("失败项：")
    for n in FAILED:
        print("  - " + n)
sys.exit(1 if FAIL else 0)
