/**
 * @file modbus.h
 * @brief Modbus RTU 从站协议栈（协议层，与硬件无关）
 *
 * @details 支持功能码：
 *   0x03 读保持寄存器
 *   0x06 写单个寄存器
 *   0x10 写多个寄存器
 *
 * 协议层不碰硬件：
 *   - 进：mb_handle(req, len) 传入一帧完整请求（不含帧边界判定）
 *   - 出：结果放在 mb_tx / mb_tx_len；mb_tx_len==0 表示不应答
 *         （CRC 错误、非本站地址、广播写 都静默丢弃，符合 Modbus 规范）
 *   - 寄存器含义完全由使用者通过 mb_read_reg()/mb_write_reg() 回调定义
 *
 * 使用方法：
 *   uint8 n = uart2_take_frame(mb_req, MB_MAX_FRAME);
 *   mb_handle(mb_req, n);
 *   if(mb_tx_len > 0) uart2_send(mb_tx, mb_tx_len);
 */
#ifndef _MODBUS_H_
#define _MODBUS_H_

#include <STC8HX.h>

/*===================================================================
 * 常量
 *=================================================================*/
#define MB_FC_READ_HOLD     0x03    // 读保持寄存器
#define MB_FC_WRITE_SINGLE  0x06    // 写单个寄存器
#define MB_FC_WRITE_MULTI   0x10    // 写多个寄存器

#define MB_EX_FUNC          0x01    // 异常：非法功能码
#define MB_EX_ADDR          0x02    // 异常：非法数据地址
#define MB_EX_VALUE         0x03    // 异常：非法数据值

#define MB_ADDR_BROADCAST   0x00    // 广播地址

#ifndef MB_MAX_FRAME
#define MB_MAX_FRAME        64      // 单帧最大字节数
#endif

/*===================================================================
 * 由使用者（main.c）提供的部分
 * 注意：mb_slave_addr 的存储类型必须与 main.c 中的定义一致（xdata）
 *=================================================================*/
extern uint8 xdata mb_slave_addr;                    // 本站地址 1~247
extern uint8 mb_read_reg(uint16 addr, uint16 *val);  // 返回 0=成功，非0=MB_EX_*
extern uint8 mb_write_reg(uint16 addr, uint16 val);  // 返回 0=成功，非0=MB_EX_*

/*===================================================================
 * 缓冲区
 *=================================================================*/
uint8 xdata mb_req[MB_MAX_FRAME];    // 请求帧副本（避免在中断缓冲上就地解析）
uint8 xdata mb_tx[MB_MAX_FRAME];     // 应答帧
uint8 xdata mb_tx_len;               // 0 = 不应答
uint8 xdata mb_crc_err;              // CRC校验失败帧计数（诊断用，上位机写0清零）

/*===================================================================
 * CRC16（Modbus 多项式 0xA001，低字节先发）
 *=================================================================*/
uint16 mb_crc16(uint8 *buf, uint8 len)
{
    uint16 crc = 0xFFFF;
    uint8  i, j;

    for(i = 0; i < len; i++) {
        crc ^= buf[i];
        for(j = 0; j < 8; j++) {
            if(crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

/* 给 mb_tx 的前 n 字节追加 CRC，并设置 mb_tx_len */
void mb_append_crc(uint8 n)
{
    uint16 crc = mb_crc16(mb_tx, n);

    mb_tx[n]     = (uint8)(crc & 0xFF);   // CRC 低字节在前
    mb_tx[n + 1] = (uint8)(crc >> 8);
    mb_tx_len    = n + 2;
}

/* 异常响应：地址 + 功能码|0x80 + 异常码 + CRC */
void mb_exception(uint8 addr, uint8 fc, uint8 excode)
{
    mb_tx[0] = addr;
    mb_tx[1] = fc | 0x80;
    mb_tx[2] = excode;
    mb_append_crc(3);
}

/*===================================================================
 * 请求帧处理
 *=================================================================*/
void mb_handle(uint8 *req, uint8 len)
{
    uint8  addr, fc, ret, i, bc;
    uint16 crc, crc_recv;
    uint16 reg, qty, val;

    mb_tx_len = 0;
    ret = 0;

    if(len < 8) return;                     // 最短合法 RTU 请求为 8 字节

    /* ---- CRC 校验：错误则静默丢弃（Modbus 规范要求不回应） ----
     * 计数在地址过滤之前，因此统计的是总线上所有 CRC 错的帧（含其它站号的），
     * 用来区分"干扰大"和"本站处理有问题"两种故障。 */
    crc_recv = (uint16)req[len - 2] | ((uint16)req[len - 1] << 8);
    crc      = mb_crc16(req, (uint8)(len - 2));
    if(crc != crc_recv) {
        if(mb_crc_err < 0xFF) {
            mb_crc_err++;
        }
        return;
    }

    /* ---- 地址过滤：本站或广播才处理 ---- */
    addr = req[0];
    if((addr != mb_slave_addr) && (addr != MB_ADDR_BROADCAST)) return;

    fc = req[1];

    switch(fc) {
    /*---------------- 0x03 读保持寄存器 ----------------*/
    case MB_FC_READ_HOLD:
        reg = ((uint16)req[2] << 8) | req[3];
        qty = ((uint16)req[4] << 8) | req[5];

        if((qty == 0) || (qty > 125) ||
           ((uint16)(qty * 2 + 5) > MB_MAX_FRAME)) {
            mb_exception(addr, fc, MB_EX_VALUE);
            break;
        }

        for(i = 0; i < qty; i++) {
            ret = mb_read_reg((uint16)(reg + i), &val);
            if(ret != 0) {
                mb_exception(addr, fc, ret);
                break;
            }
            mb_tx[3 + i * 2]     = (uint8)(val >> 8);
            mb_tx[3 + i * 2 + 1] = (uint8)(val & 0xFF);
        }
        if(ret != 0) break;                 // 上面已构造异常响应

        mb_tx[0] = addr;
        mb_tx[1] = fc;
        mb_tx[2] = (uint8)(qty * 2);
        mb_append_crc((uint8)(3 + qty * 2));
        break;

    /*---------------- 0x06 写单个寄存器 ----------------*/
    case MB_FC_WRITE_SINGLE:
        reg = ((uint16)req[2] << 8) | req[3];
        val = ((uint16)req[4] << 8) | req[5];

        ret = mb_write_reg(reg, val);
        if(ret != 0) {
            mb_exception(addr, fc, ret);
            break;
        }

        /* 正常应答 = 原样回显请求（地址+功能码+寄存器地址+值） */
        mb_tx[0] = addr;
        mb_tx[1] = fc;
        mb_tx[2] = req[2];
        mb_tx[3] = req[3];
        mb_tx[4] = req[4];
        mb_tx[5] = req[5];
        mb_append_crc(6);
        break;

    /*---------------- 0x10 写多个寄存器 ----------------*/
    case MB_FC_WRITE_MULTI:
        if(len < 11) return;                // 帧太短，不合法

        reg = ((uint16)req[2] << 8) | req[3];
        qty = ((uint16)req[4] << 8) | req[5];
        bc  = req[6];                       // 字节数

        if((qty == 0) || (qty > 125) ||
           (bc != (uint8)(qty * 2)) ||
           (len < (uint8)(9 + bc))) {
            mb_exception(addr, fc, MB_EX_VALUE);
            break;
        }

        for(i = 0; i < qty; i++) {
            val = ((uint16)req[7 + i * 2] << 8) | req[8 + i * 2];
            ret = mb_write_reg((uint16)(reg + i), val);
            if(ret != 0) {
                mb_exception(addr, fc, ret);
                break;
            }
        }
        if(ret != 0) break;

        /* 正常应答 = 地址+功能码+起始地址+寄存器数量 */
        mb_tx[0] = addr;
        mb_tx[1] = fc;
        mb_tx[2] = req[2];
        mb_tx[3] = req[3];
        mb_tx[4] = req[4];
        mb_tx[5] = req[5];
        mb_append_crc(6);
        break;

    /*---------------- 不支持的功能码 ----------------*/
    default:
        mb_exception(addr, fc, MB_EX_FUNC);
        break;
    }

    /* 广播写成功也不应答（否则总线上多个从站会同时抢占） */
    if(addr == MB_ADDR_BROADCAST) {
        mb_tx_len = 0;
    }
}

#endif /* _MODBUS_H_ */
