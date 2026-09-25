/**
 * @file uart2.h
 * @brief STC8H 串口2 + RS485 半双工驱动（硬件层）
 *
 * @details 只用硬件串口2，收发由中断驱动（非阻塞）：
 *   发送：uart2_send() 装填缓冲 → 置 DE=1 → 逐字节由中断发出 →
 *         最后一位发完后延时一小段再置 DE=0，保证停止位完整送出。
 *   接收：中断收字节 → 由 100us 时基统计静默时长 → 静默超过 RTU 帧间隔
 *         即认为一帧结束，上层用 uart2_frame_ready() 取走。
 *
 * 依赖项（由使用者在包含本文件前提供）：
 *   - RS485_DIR_TX() / RS485_DIR_RX()：方向控制宏（DE/RE）
 *   - uart2_tick() 必须在 100us 时基里被调用一次
 *   - 主程序需开总中断 EA=1
 *
 * @note 本文件为单编译单元工程里的头文件实现（与 rcswitch.h 风格一致），
 *       只被 main.c 包含一次，不存在重复定义问题。
 */
#ifndef _UART2_H_
#define _UART2_H_

#include <STC8HX.h>

/*===================================================================
 * 可配置参数
 *=================================================================*/

#ifndef UART2_RX_SIZE
#define UART2_RX_SIZE       64      // 接收缓冲（一帧 Modbus RTU 最长 256，64 够用）
#endif
#ifndef UART2_TX_SIZE
#define UART2_TX_SIZE       64      // 发送缓冲
#endif

/* RTU 帧间隔判定：3.5 个字符时间。
 * 9600bps 8N1 下 1 个字符 = 10bit ≈ 1.04ms → 3.5 字符 ≈ 3.64ms。
 * 时基为 100us，取 40（4.0ms）留余量。改波特率时需同步调整。 */
#ifndef UART2_FRAME_GAP
#define UART2_FRAME_GAP     40
#endif

/* 最后一个字节发出后，延时多久把 DE 拉回接收。
 * 8051 的 TI 在"停止位开始"时即置位，若此时立刻关 DE 会切掉停止位，
 * 因此延时约 2 个字符时间（9600 下取 20 = 2ms）再关。 */
#ifndef UART2_DE_OFF_DELAY
#define UART2_DE_OFF_DELAY  20
#endif

/* 发送看门狗：从"启动发送"起算，超过这个时间还没发完就认为中断续发卡死，
 * 强制收回总线并回到接收态。
 * 一帧 Modbus 应答最长 61 字节 @9600 ≈ 63ms，取 100ms 留足余量。
 * 没有它的话，一旦 TI 未产生/被误清，u2_tx_busy 会永久为 1，
 * uart2_frame_ready() 永远返回 0 → 从站从此收不到任何请求（只能断电重启）。 */
#ifndef UART2_TX_WD
#define UART2_TX_WD         1000    // 1000 × 100us = 100ms
#endif

/* 波特率重载值（Timer2 作波特率发生器，1T 模式）
 *   串口2 波特率 = T2溢出率 / 4
 *   T2 溢出率    = SYSclk / (65536 - RL)
 *   → RL = 65536 - SYSclk / (BAUD × 4)
 *   24MHz、9600bps：RL = 65536 - 24000000/(9600×4) = 65536 - 625 = 64911 = 0xFD8F
 * ★ 改主频或波特率时按上式重算这两个字节。常用值：
 *   24MHz/19200 → 65536-313 = 0xFEC3     24MHz/38400 → 65536-157 = 0xFF63
 *   24MHz/4800  → 65536-1250 = 0xFB1E */
#ifndef UART2_RL_H
#define UART2_RL_H          0xFD
#define UART2_RL_L          0x8F
#endif

/* 方向控制缺省引脚（主程序若在包含前自定义 RS485_DIR_TX/RX 则本段不生效） */
#ifndef RS485_DIR_TX
#define RS485_DIR_TX()      (P3_5 = 1)
#define RS485_DIR_RX()      (P3_5 = 0)
#endif

/*===================================================================
 * 缓冲区与状态
 *=================================================================*/
volatile uint8  xdata u2_rx_buf[UART2_RX_SIZE];
volatile uint8  xdata u2_rx_len;        // 当前帧已收字节数
volatile uint8  xdata u2_silence;       // 静默时长（100us 单位，收到字节清零）

volatile uint8  xdata u2_tx_buf[UART2_TX_SIZE];
volatile uint8  xdata u2_tx_len;
volatile uint8  xdata u2_tx_idx;
volatile uint8  xdata u2_tx_busy;       // 1=正在发送
volatile uint8  xdata u2_de_off;        // 发完后的关断倒计时（100us 单位）

/* 诊断计数（供上位机读取，写 0 清零） */
volatile uint16 xdata u2_tx_wd;         // 发送看门狗计时（100us）
volatile uint8  xdata u2_rx_ovf;        // 接收缓冲溢出次数（丢字节/超长帧）
volatile uint8  xdata u2_tx_reset;      // 发送看门狗自愈次数

/*===================================================================
 * 硬件初始化
 *=================================================================*/
void uart2_init(void)
{
    /* 引脚：P_SW2 bit0 = 0 → 串口2 落在 P1.0(RxD2) / P1.1(TxD2)
     * 注意 P_SW2 bit7 是 EAXFR，不能动，所以只清 bit0 */
    P_SW2 &= ~0x01;

    P1M1 |=  0x01;  P1M0 &= ~0x01;      // P1.0 (RxD2) 高阻输入
    P1M1 &= ~0x02;  P1M0 |=  0x02;      // P1.1 (TxD2) 推挽输出

    S2CON = S2REN;                      // 8位UART、可变波特率、允许接收

    AUXR |=  T2x12;                     // Timer2 1T 模式
    AUXR &= ~T2_CT;                     // Timer2 定时（非计数）
    T2L   = UART2_RL_L;                 // 先写低字节
    T2H   = UART2_RL_H;
    AUXR |=  T2R;                       // 启动 Timer2

    u2_rx_len  = 0;
    u2_silence = 0;
    u2_tx_len  = 0;
    u2_tx_idx  = 0;
    u2_tx_busy = 0;
    u2_de_off  = 0;

    u2_rx_ovf  = 0;
    u2_tx_reset = 0;
    u2_tx_wd   = 0;

    RS485_DIR_RX();                     // 默认处于接收
    IE2 |= ES2;                         // 允许串口2中断（EA 由主程序开）
}

/*===================================================================
 * 发送：装填缓冲并启动中断发送，发送期间 DE=1
 * 参数：buf=数据首字节指针，len=长度（不含 CRC 之外的东西，纯字节流）
 *=================================================================*/
void uart2_send(uint8 *buf, uint8 len)
{
    uint8 i;

    if(u2_tx_busy)      return;         // 上一帧未发完，丢弃（正常不会走到）
    if(len == 0)        return;
    if(len > UART2_TX_SIZE) return;

    for(i = 0; i < len; i++) {
        u2_tx_buf[i] = buf[i];
    }
    u2_tx_len  = len;
    u2_tx_idx  = 1;                     // 第0个字节马上手动发出
    u2_tx_busy = 1;
    u2_de_off  = 0;
    u2_tx_wd   = 0;                     // 启动发送看门狗

    RS485_DIR_TX();
    S2CON &= ~S2TI;
    S2BUF = u2_tx_buf[0];
}

/*===================================================================
 * 100us 时基：由 Timer0 中断调用
 *   1) 累加静默时长（用于 RTU 帧边界判定）
 *   2) 发送完成后的 DE 关断倒计时
 *=================================================================*/
void uart2_tick(void)
{
    if(u2_silence < 0xFF) {
        u2_silence++;
    }
    if(u2_de_off > 0) {
        u2_de_off--;
        if(u2_de_off == 0) {
            RS485_DIR_RX();             // 停止位已发出，转回接收
        }
    }

    /* ---- 发送看门狗：中断续发若异常卡死，强制收回总线 ----
     * 否则 u2_tx_busy 会永久为 1，从站再也收不到任何请求。 */
    if(u2_tx_busy) {
        if(u2_tx_wd < 0xFFFF) {
            u2_tx_wd++;
        }
        if(u2_tx_wd >= UART2_TX_WD) {
            u2_tx_busy = 0;
            u2_tx_idx  = 0;
            u2_tx_len  = 0;
            u2_tx_wd   = 0;
            u2_de_off  = 0;
            u2_rx_len  = 0;             // 半帧残留丢掉，重新同步
            if(u2_tx_reset < 0xFF) {
                u2_tx_reset++;
            }
            RS485_DIR_RX();
        }
    } else {
        u2_tx_wd = 0;
    }
}

/*===================================================================
 * 查询：总线是否完全空闲（可以安全地做"关中断"的慢操作）
 * 必须等应答发完（tx_busy=0）且 DE 已收回（de_off=0），
 * 否则 EEPROM 擦除这类关中断操作会把正在发送的字节流掐断。
 *=================================================================*/
uint8 uart2_tx_idle(void)
{
    return ((u2_tx_busy == 0) && (u2_de_off == 0)) ? 1 : 0;
}

/*===================================================================
 * 帧就绪判定：收到数据且静默超过帧间隔
 *=================================================================*/
uint8 uart2_frame_ready(void)
{
    if(u2_tx_busy)      return 0;
    if(u2_rx_len == 0)  return 0;
    return (u2_silence >= UART2_FRAME_GAP) ? 1 : 0;
}

/*===================================================================
 * 取走一帧：复制到 dst，返回长度，并复位接收缓冲
 *=================================================================*/
uint8 uart2_take_frame(uint8 *dst, uint8 maxlen)
{
    uint8 i;
    uint8 n;

    n = u2_rx_len;
    if(n > maxlen) n = maxlen;
    for(i = 0; i < n; i++) {
        dst[i] = u2_rx_buf[i];
    }
    u2_rx_len  = 0;
    u2_silence = 0;
    return n;
}

/*===================================================================
 * 串口2 中断：接收字节 / 续发下一字节
 *=================================================================*/
void uart2_isr(void) interrupt UART2_VECTOR
{
    uint8 b;

    if(S2CON & S2RI) {
        S2CON &= ~S2RI;
        b = S2BUF;
        u2_silence = 0;                 // 收到字节 → 重新计静默
        if(u2_rx_len < UART2_RX_SIZE) {
            u2_rx_buf[u2_rx_len++] = b;
        } else {
            u2_rx_len = 0;              // 超长帧：丢弃并重新同步
            if(u2_rx_ovf < 0xFF) {
                u2_rx_ovf++;            // 记下来供上位机诊断
            }
        }
    }

    if(S2CON & S2TI) {
        S2CON &= ~S2TI;
        if(u2_tx_idx < u2_tx_len) {
            S2BUF = u2_tx_buf[u2_tx_idx++];
        } else {
            u2_tx_busy = 0;
            u2_de_off  = UART2_DE_OFF_DELAY;    // 延时关 DE，保证停止位完整
        }
    }
}

#endif /* _UART2_H_ */
