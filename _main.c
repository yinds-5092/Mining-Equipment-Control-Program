/*****************************************************************************
 * @file    _main.c
 * @brief   铁路道岔控制器 — 气动电磁阀版（STC8H1K17）
 *
 * @details 一次道岔转换 = 给一个电磁阀得电 + 分两段确认位置信号：
 *
 *    【阶段1 离开原位】阀得电 → 道岔开始移动 → 等"出发位信号消失"
 *    【阶段2 到达目标】阀保持得电 → 等"目标位信号出现" → 立即断电
 *
 *    到位后 OUT1/OUT2 全部断电，不再有任何输出，进入待机等待下一次触发。
 *
 *    ★ 不到位才允许"返回"：
 *      只有"等到达目标位"超时（道岔卡在中间、两端信号都无效）时，
 *      才会给**另一个阀**得电，把道岔推回出发位；
 *      一旦任何一端信号有效（即已到位），两个阀都不再输出，不允许返回。
 *
 *    触发方式有三种：手动按钮(P3.2)、已配对遥控器、RS485 上位机命令。
 *    （SW1 不参与转换，只用于遥控器的配对与清空）
 *
 * @note    本文件是**唯一源码**；main.c 由构建脚本(stc-conv_hz.py)生成，不要手改。
 *
 * @version 5.0 - 增加 RS485 / Modbus RTU
 *
 * ---------------------------------------------------------------------------
 * 分层结构（上层不直接操作引脚，改硬件只需动第 0/2 层）
 * ---------------------------------------------------------------------------
 *   第 0 层  引脚定义     引脚 ↔ 功能 的唯一映射表
 *   第 1 层  常量与状态   超时常量、寄存器映射、全局变量
 *   第 2 层  HAL          读写引脚的最小函数（本文件唯一接触 P1_3/P3_2 的地方）
 *   第 3 层  输入扫描     消抖，把"电平"翻译成"事件"
 *   第 4 层  定时器       启动/停止/超时判断 + 100us 时基
 *   第 5 层  业务模块     位置判定、配对存储(EEPROM)、道岔子状态机
 *   第 5.5 层 RS485       Modbus 寄存器映射（协议栈在 myLib/modbus.h，
 *                         串口驱动在 myLib/uart2.h）
 *   第 6 层  系统调度     系统主状态机，协调 按键/RF/手动按钮/RS485 四种触发源
 *   第 7 层  入口         RF接收、gpio_init()、setup() / loop() / main()
 *
 * ---------------------------------------------------------------------------
 * 调参速查表（时间单位统一为 100us tick：1秒 = 10000）
 * ---------------------------------------------------------------------------
 *   T_SOL_PULSE     电磁阀最小得电时间（阀芯可靠动作所需）
 *   T_LEAVE_POSx    阶段1 等"离开出发位"超时（点动/_Q 全程两套，由 IN5 选择）
 *   T_REACH_POSx    阶段2 等"到达目标位"超时（同上）
 *   T_RETURN        不到位时反向返回出发位的超时
 *   T_RET_WAIT      返回前的泄压等待
 *   T_SUCCESS       转换成功后位置指示保持时间
 *   T_FAIL          失败提示时长
 *   SW1_LONG_MIN    SW1 长按判定门限（清空配对）
 *   KEY_TURN_MIN    手动按钮消抖时间
 *   DEBOUNCE_ON     到位开关确认有效所需时间
 *
 *   ---- RS485 / Modbus ----
 *   MB_ADDR_DEFAULT  出厂默认站号；上位机可写 40005(0x0004) 修改并存入EEPROM
 *   UART2_RL_H/L     myLib/uart2.h：波特率重载值，24MHz/9600 = 0xFD8F
 *                    （公式 RL = 65536 - SYSclk/(BAUD×4)，改波特率需重算）
 *   UART2_FRAME_GAP  帧间隔判定（100us单位，40=4ms，须 ≥ 3.5字符@9600≈3.6ms）
 *   UART2_DE_OFF_DELAY 发完最后一字节后多久切回接收（20=2ms）
 *   寄存器映射表      见本文件"Modbus 保持寄存器映射表"注释块
 *****************************************************************************/

#define _STC8

// ============================================================================
// 第 0 层：引脚定义
// 所有"引脚 ↔ 功能"的映射只在这里出现一次，改接线只改这一块
// ============================================================================

/* ---- 输出：执行机构 ----
 * 两个电磁阀各负责一个方向：一个把道岔推向 A 位，另一个推向 B 位。
 * 一次转换只给其中一个得电，绝不两个同时开。
 * 若实际接线相反，把下面 SOL_TO_A / SOL_TO_B 两个宏对调即可，状态机不用动。
 * ------------------------------------------------------------------*/
#define OUT1        P1_3    // 电磁阀A
#define OUT2        P1_4    // 电磁阀B
#define SOL_TO_A    OUT1    // 得电 → 把道岔推向 A 位
#define SOL_TO_B    OUT2    // 得电 → 把道岔推向 B 位

/* ---- 输出：指示灯 ----
 * 点亮电平与位置对应关系对齐现场正在使用的原固件（2024-01-24 版）：
 *   停在岔位A（POS_SIG_A 有效）→ OUT4 亮
 *   停在岔位B（POS_SIG_B 有效）→ OUT3 亮
 *   异常（两端信号同态）       → OUT3/OUT4 都灭（LED快闪提示 + 485上报）
 * ★ 若现场面板标注与此相反，把下面 indicator_show() 里的 OUT3/OUT4 对调即可。
 * ------------------------------------------------------------------*/
#define OUT3        P1_5    // 岔位B指示灯
#define OUT4        P1_6    // 岔位A指示灯
#define LED         P3_1    // 板载LED：低电平点亮（配对闪烁、成功/错误提示）

/* ---- RS485 接口 ----
 * 串口2固定落在 P1.0(RxD2) / P1.1(TxD2)（由 P_SW2 选引脚组 S1，见 myLib/uart2.h），
 * P1.2 仍作为无线接收输入，两者互不冲突。
 * P3.5 接 485 收发器的方向控制 DE/RE：1=发送，0=接收。
 * ★ 原 P3.5 的"异常位置指示灯 OUT5"已取消：异常状态改由 LED 快闪提示，
 *   并通过 Modbus 状态寄存器 MB_REG_STATUS 的 bit2 上报给上位机。
 * ------------------------------------------------------------------*/
#define RS485_DE        P3_5
#define RS485_DIR_TX()  (RS485_DE = 1)
#define RS485_DIR_RX()  (RS485_DE = 0)

/* ---- 输入：岔位位置信号 ----
 * 道岔只有 A / B 两个位置，各有一个位置信号（低电平有效），
 * 有效即表示道岔当前停在该位；转换到位后对应信号会变有效。
 *   IN2(P3.3) → 岔位A信号
 *   IN3(P3.4) → 岔位B信号
 * ★ 若现场接线相反，只需把下面 POS_SIG_A / POS_SIG_B 两个宏对调，
 *   状态机、指示灯、到位判定会全部跟着变，其它代码一行都不用动。
 * ------------------------------------------------------------------------*/
#define IN2         P3_3    // 原理图网络名 IN2
#define IN3         P3_4    // 原理图网络名 IN3
#define POS_SIG_A   IN2     // 岔位A信号（有效=道岔在A位）
#define POS_SIG_B   IN3     // 岔位B信号（有效=道岔在B位）

#define IN1         P3_2    // 手动转换按钮：低电平有效，按一次道岔转换一次位置
#define IN4         P3_6    // 未使用（保留）
#define IN5         P3_7    // 模式选择：0=点动模式，1=全程模式
#define SW1         P3_0    // 功能按键：短按进入配对，长按清空配对
#define PB1         P1_7    // 蜂鸣器控制

/* 蜂鸣器驱动电平：当前按"低电平有效"接线（PB1=1 关断，PB1=0 鸣响），
 * 与原代码上电初始化 PB1=1 的行为保持一致。
 * 若实际硬件是高电平驱动，把下面两行的 1 和 0 对调即可，无需改其它地方。 */
#define BUZZER_OFF()    (PB1 = 1)
#define BUZZER_ON()     (PB1 = 0)

#include <STC8HX.h>
uint32 sys_clk = 24000000;

/* 遥控器协议与位长
 * rcswitch.h 在中断里解码时要用 RCSWITCH_FIXED_PROTOCOL，
 * 因此必须在包含 rcswitch.h 之前定义，它才会按本工程的协议解码。 */
#define RF_PROTOCOL         1    // 仅接受该协议的遥控器（1527 = 协议1）
#define RF_BITLENGTH       24    // 仅接受该位长（1527 24位）

#include "myLib/rcswitch.h"
#include "lib/eeprom.h"
#include "lib/delay.h"
#include "myLib/uart2.h"        // 串口2 + RS485 方向控制（硬件层）
#include "myLib/modbus.h"       // Modbus RTU 从站（协议层）

// ============================================================================
// 第 1 层：类型、常量与全局状态
// ============================================================================

/*-------------------------------------------------------------------
 * 道岔位置（由两个到位开关组合判定，本文件内统一用这三个值传递）
 *------------------------------------------------------------------*/
#define POS_UNKNOWN    0    // 位置异常：两个到位开关同态（都有效或都无效）
#define POS_A          1    // 停在位置A
#define POS_B          2    // 停在位置B

/*-------------------------------------------------------------------
 * 道岔转换方向：由"当前位置"推导，不需要外部指定
 *------------------------------------------------------------------*/
#define DIR_NONE       0
#define DIR_A_TO_B     1    // 由位置A转换到位置B
#define DIR_B_TO_A     2    // 由位置B转换到位置A

/*-------------------------------------------------------------------
 * 系统主状态
 *------------------------------------------------------------------*/
typedef enum {
    SYS_INIT = 0,       // 上电初始化
    SYS_IDLE,           // 待机：显示位置，等待触发
    SYS_PAIRING,        // 配对：等待遥控器信号并保存
    SYS_PAIRING_WAIT,   // 配对等待：等遥控器松手
    SYS_CLEARING,       // 清空配对数据
    SYS_TURNING         // 道岔转换中（子状态机接管）
} SystemState;

/*-------------------------------------------------------------------
 * 道岔子状态：一次转换 = 得电 → 离开原位 → 到达目标位 → 断电
 *
 * 正常路径： DETECT → SOL → LEAVE → REACH → SUCCESS → IDLE
 * 不到位时： DETECT → SOL → LEAVE → REACH(超时) → RET_WAIT → RETURN → SUCCESS/FAIL
 * 出错时  ： 任一环节超时/位置异常 → FAIL → IDLE
 *------------------------------------------------------------------*/
typedef enum {
    TURN_IDLE = 0,      // 空闲：等待 turnout_start()
    TURN_DETECT,        // 读当前岔位 → 决定目标位 → 给对应的阀得电
    TURN_SOL,           // 阀已得电，保证最小得电时间（阀芯可靠动作）
    TURN_LEAVE,         // 阶段1：等"出发位信号消失"（道岔确实动了）
    TURN_REACH,         // 阶段2：等"目标位信号出现"（道岔已到位）
    TURN_SUCCESS,       // 成功：已断电，保持位置指示并慢闪 LED 一段时间后回空闲
    TURN_RET_WAIT,      // 不到位：先等 1.5 秒让气缸泄压，再反向返回
    TURN_RETURN,        // 返回：给反向阀得电，等"出发位信号恢复"
    TURN_FAIL           // 错误：已断电，LED 快闪提示一段时间后回空闲
} TurnState;

/*-------------------------------------------------------------------
 * 按键事件值
 *------------------------------------------------------------------*/
#define SW1_EVT_NONE   0
#define SW1_EVT_SHORT  1    // 短按（50ms~2s）：进入配对
#define SW1_EVT_LONG   2    // 长按（>6s）：清空配对
#define KEY_EVT_NONE   0
#define KEY_EVT_PRESS  1    // 手动按钮按下一次：请求转换

/*-------------------------------------------------------------------
 * 时间常量（单位：100us tick，1秒 = 10000）
 *------------------------------------------------------------------*/
#define T_SOL_PULSE       5000   // 电磁阀最小得电时间：500ms（保证阀芯可靠动作）
/* 超时值取自现场正常使用的原固件（反汇编核对）：点动 40304 tick=4.03s，全程 50000=5s */
#define T_LEAVE_POS       40300  // 点动模式：等"离开出发位"超时 4.03秒
#define T_LEAVE_POS_Q     50000  // 全程模式：等"离开出发位"超时 5秒
#define T_REACH_POS       40300  // 点动模式：等"到达目标位"超时 4.03秒
#define T_REACH_POS_Q     50000  // 全程模式：等"到达目标位"超时 5秒
#define T_RETURN          40300  // 返回出发位的超时：4.03秒（不分模式）
#define T_RET_WAIT        15000  // 返回前的泄压等待：1.5秒
#define T_SUCCESS         50000  // 成功指示保持时间：5秒
#define T_FAIL            50000  // 错误提示时长：5秒
#define T_BLINK_SLOW      5000   // 慢闪半周期：500ms（成功提示，1Hz）
#define T_BLINK_FAST      500    // 快闪半周期：50ms（错误/返回提示，10Hz）

#define SW1_SHORT_MIN     500    // SW1短按最短时间：50ms（消抖）
#define SW1_SHORT_MAX   20000    // SW1短按最长时间：2秒
#define SW1_LONG_MIN    60000    // SW1长按门限：6秒
#define KEY_TURN_MIN      500    // 手动按钮稳定确认时间：50ms（消抖）

/*-------------------------------------------------------------------
 * 遥控"防重复触发"
 * 1527遥控器按住不放会连续发帧；若不加约束，一次按住会被识别成多次触发，
 * 表现为"转换刚结束、阀立刻又得电"。规则：一次触发消费后必须等遥控
 * 松手（无新帧持续 RF_SILENCE_MIN）才允许下一次触发。
 *------------------------------------------------------------------*/
#define RF_SILENCE_MIN    30     // 静默判定时长：30 × 10ms = 300ms

/*-------------------------------------------------------------------
 * 到位开关积分消抖参数
 * 低电平连续累计到 DEBOUNCE_ON 判为"有效"；高电平连续减到 DEBOUNCE_OFF 判为"无效"
 *------------------------------------------------------------------*/
#define DEBOUNCE_ON      1000    // 100ms
#define DEBOUNCE_OFF        5    // 0.5ms
#define DEBOUNCE_CAP     1010    // 计数器上限，防止溢出回绕

/*-------------------------------------------------------------------
 * 遥控器配对存储（EEPROM）
 *------------------------------------------------------------------*/
#define EEPROM_BUF_SIZE   255    // 缓存/读写长度
#define EEPROM_PAIR_COUNT 250    // 存放"已配对字节数"的地址
#define PAIR_SLOT_BYTES     4    // 每组配对编码占4字节（32位）
#define PAIR_MAX_SLOTS     60    // 最多60组
#define PAIR_MAX_BYTES   (PAIR_MAX_SLOTS * PAIR_SLOT_BYTES)   // =240
/* RF_PROTOCOL / RF_BITLENGTH 已定义在包含 rcswitch.h 之前（见文件头部） */

/*-------------------------------------------------------------------
 * Modbus RTU：从站地址存储
 *------------------------------------------------------------------*/
#define EEPROM_SLAVE_ADDR  251    // EEPROM中存放从站地址的字节（250已用于配对计数）
#define MB_ADDR_DEFAULT      1    // 出厂默认站号
#define MB_ADDR_MIN          1
#define MB_ADDR_MAX        247

/*-------------------------------------------------------------------
 * Modbus 保持寄存器映射表
 * 地址从 0 开始；上位机侧按 4xxxx 区访问时，寄存器号 = 地址 + 1
 * （即本表的 0x0000 对应 40001）
 *
 *   地址   名称             权限   含义
 *   0x0000 STATUS           只读   运行状态字（位定义见 MB_ST_*）
 *   0x0001 POSITION         只读   当前岔位：0=未知/中间  1=A位  2=B位
 *   0x0002 COMMAND          只写   转岔命令（见 MB_CMD_*，写入后自动清零）
 *   0x0003 RESULT           读写   上次转换结果（见 MB_RES_*，写0可清除）
 *   0x0004 SLAVE_ADDR       读写   从站地址 1~247（写入后存EEPROM，掉电保持）
 *   0x0005 MODE             只读   转换模式：0=点动  1=全程（反映IN5）
 *   0x0006 VERSION          只读   固件版本，如 0x0103 表示 V1.03
 *   0x0007 RX_OVF           读写   串口接收溢出次数（丢字节），写0清零
 *   0x0008 CRC_ERR          读写   CRC校验失败帧数，写0清零
 *   0x0009 TX_RESET         读写   发送看门狗自愈次数，写0清零
 *
 * 后三个是通讯诊断用：无线解码与总线收发重叠时若仍有丢帧，
 * 读这三个计数就能判断是"根本没收到/收到但 CRC 错"还是"发送卡死"。
 *------------------------------------------------------------------*/
#define MB_REG_STATUS       0x0000
#define MB_REG_POSITION     0x0001
#define MB_REG_COMMAND      0x0002
#define MB_REG_RESULT       0x0003
#define MB_REG_SLAVE_ADDR   0x0004
#define MB_REG_MODE         0x0005
#define MB_REG_VERSION      0x0006
#define MB_REG_RX_OVF       0x0007
#define MB_REG_CRC_ERR      0x0008
#define MB_REG_TX_RESET     0x0009

/* 转岔命令值（写入 MB_REG_COMMAND） */
#define MB_CMD_NONE            0    // 无命令
#define MB_CMD_TOGGLE          1    // 切换一次（等同按一次手动按钮）
#define MB_CMD_GOTO_A          2    // 转到A位（当前已在A位则直接返回成功，不动作）
#define MB_CMD_GOTO_B          3    // 转到B位（当前已在B位则直接返回成功，不动作）

/* 上次转换结果（MB_REG_RESULT） */
#define MB_RES_NONE            0    // 尚无转换记录（或已被上位机清除）
#define MB_RES_SUCCESS         1    // 成功
#define MB_RES_FAIL            2    // 失败：超时未到位 / 岔位信号异常

/* 状态字位定义（MB_REG_STATUS） */
#define MB_ST_POS_A         0x01    // bit0：岔位A信号有效
#define MB_ST_POS_B         0x02    // bit1：岔位B信号有效
#define MB_ST_ABNORMAL      0x04    // bit2：位置异常（两端信号同态）
#define MB_ST_TURNING       0x08    // bit3：正在转换中
#define MB_ST_LAST_OK       0x10    // bit4：上次转换成功
#define MB_ST_LAST_FAIL     0x20    // bit5：上次转换失败
#define MB_ST_MODE_FULL     0x40    // bit6：1=全程模式，0=点动模式

#define MB_FW_VERSION      0x0103   // 固件版本 V1.03（并发加固版）

/*-------------------------------------------------------------------
 * 全局变量
 *------------------------------------------------------------------*/
// ---- 状态机 ----
SystemState xdata sys_state;         // 系统主状态机当前状态
TurnState   xdata turn_state;        // 道岔子状态机当前状态
uint8       xdata turn_direction;    // 本次转换方向：DIR_A_TO_B / DIR_B_TO_A

// ---- 定时器（由100us中断驱动）----
volatile uint16 xdata fsm_timer;     // 状态机计时器
uint8           xdata fsm_timer_en;  // 1=计时中，0=停止
volatile uint16 xdata blink_tick;    // LED闪烁计时器（独立于状态机计时）

// ---- SW1按键 ----
volatile uint16 xdata sw1_tick;      // 按下时长计数
volatile uint8  xdata sw1_event;     // 事件：SW1_EVT_*
volatile uint8  xdata sw1_long_fired;   // 长按是否已触发过（防止按住不放时反复清空EEPROM）

// ---- 手动转换按钮(P3.2) ----
volatile uint16 xdata key_tick;      // 按下时长计数
volatile uint8  xdata key_event;     // 事件：KEY_EVT_*
volatile uint8  xdata key_fired;     // 本次按下是否已触发（防止长按连发）

// ---- 岔位位置信号（消抖后的稳定状态，命名跟随 POS_SIG_A/B 语义）----
volatile uint8  xdata pos_a_state;   // 岔位A信号：1=有效（道岔在A位）
volatile uint8  xdata pos_b_state;   // 岔位B信号：1=有效（道岔在B位）
volatile uint16 xdata pos_a_tick;    // 消抖计数器
volatile uint16 xdata pos_b_tick;

// ---- 无线接收 ----
uint32 xdata rf_value;               // 最近一次收到的32位编码
uint8  xdata rf_received;            // 1=有新数据待处理
volatile uint8  xdata rf_silence;    // 距上一次收到RF帧的时长（单位10ms，上限255）
volatile uint8  xdata rf_silence_tick;   // 10ms基准分频计数器
uint8  xdata rf_armed;               // 1=允许RF触发；0=已消费本次按下，需松手后重新武装

// ---- EEPROM缓存 ----
uint8  xdata eeprom_buf[EEPROM_BUF_SIZE];

// ---- RS485 / Modbus ----
uint8 xdata mb_slave_addr;       // 本站地址：上电从EEPROM加载，可用06/10功能码改写
uint8 xdata mb_cmd;              // 待执行的转岔命令 MB_CMD_*（上位机写入，主状态机消费）
uint8 xdata last_turn_result;    // 上次转换结果 MB_RES_*（供上位机查询）

/* 站号落盘延后标志
 * EEPROM 扇区擦除期间必须关中断（见 lib/eeprom.h 的 eeprom_trig），会屏蔽
 * 串口 4~10ms。若在上位机写站号时同步擦写，应答还没发出去就被卡住。
 * 做法：应答先发，落盘放到主循环空闲时做。 */
uint8 xdata mb_addr_pending;     // 1=有待落盘的站号
uint8 xdata mb_addr_pending_val; // 待落盘的站号值

// ---- LED 闪烁提示（非阻塞，由主循环驱动）----
uint8  xdata blink_active;       // 1=闪烁任务进行中
uint8  xdata blink_left;         // 剩余翻转次数（次数×2）
uint16 xdata blink_half;         // 半周期（100us tick）
uint16 xdata blink_ref;          // 上一次翻转时刻（取自 blink_tick）

// ============================================================================
// 第 2 层：HAL —— 读写引脚的最小函数
// 除本层外，其它任何地方都不应直接出现 OUT1 / LED / IN2 这类引脚名
// ============================================================================

/*-------------------------------------------------------------------
 * 执行机构：电磁阀
 * 气动系统的"停机"就是断电——电磁阀失电即切断气源，是最安全的处置。
 *------------------------------------------------------------------*/
void solenoid_all_off(void)   // 两个阀都断电：切断气源
{
    SOL_TO_A = 0;
    SOL_TO_B = 0;
}

void sol_to_a_on(void)        // 推向A位的阀得电（同时确保另一个断电，禁止双阀同开）
{
    SOL_TO_B = 0;
    SOL_TO_A = 1;
}

void sol_to_b_on(void)        // 推向B位的阀得电（同时确保另一个断电，禁止双阀同开）
{
    SOL_TO_A = 0;
    SOL_TO_B = 1;
}

/*-------------------------------------------------------------------
 * 按"本次转换方向"选择要得电的那个阀
 * 状态机只说"我要往哪边转"，具体开哪个阀由这里决定 —— 阀位改接线时
 * 只需改 SOL_TO_A / SOL_TO_B 两个宏，状态机不用动。
 *------------------------------------------------------------------*/
void sol_valve_on(void)          // 去程：推向目标位的阀
{
    if(turn_direction == DIR_A_TO_B) {
        sol_to_b_on();
    } else {
        sol_to_a_on();
    }
}

void sol_return_valve_on(void)   // 回程：推回出发位的阀（与去程相反）
{
    if(turn_direction == DIR_A_TO_B) {
        sol_to_a_on();
    } else {
        sol_to_b_on();
    }
}

/*-------------------------------------------------------------------
 * 指示灯：LED
 * 硬件为低电平点亮，这里统一用 led_on/led_off 表达意图，调用方不必记极性
 *------------------------------------------------------------------*/
void led_on(void)      { LED = 0; }
void led_off(void)     { LED = 1; }
void led_toggle(void)  { LED = !LED; }

/* 非阻塞闪烁提示：只"登记"一次闪烁任务，真正的翻转由 led_blink_run() 在主循环里做。
 *
 * ★ 原来是 delay() 空等（闪3次=600ms 一动不动），期间 modbus_poll() 完全不执行，
 *   上位机必然超时；遥控/按钮事件也漏掉。改成主循环驱动后，闪烁期间
 *   串口照样应答、射频照样接收。
 * 用法：led_blink_start(次数, 半周期ms) → 主循环每轮 led_blink_run()。 */
void led_blink_start(uint8 times, uint16 ms)
{
    if(times == 0) return;
    blink_half   = (uint16)(ms * 10);   // ms → 100us tick
    blink_left   = (uint8)(times * 2);  // 亮+灭 算两次翻转
    blink_ref    = blink_tick;
    blink_active = 1;
    led_on();
}

void led_blink_run(void)
{
    if(!blink_active) return;

    /* 用差值判断，避免和道岔状态机共用 blink_tick 时互相干扰
     * （闪烁只在配对/清空提示时用，此时道岔状态机不占用 blink_tick） */
    if((uint16)(blink_tick - blink_ref) >= blink_half) {
        blink_ref = blink_tick;
        led_toggle();
        if(--blink_left == 0) {
            blink_active = 0;
            led_off();          // 结束时确保熄灭
        }
    }
}

/*-------------------------------------------------------------------
 * 指示灯：位置指示 OUT3/OUT4/OUT5
 * 入参为 POS_A / POS_B / POS_UNKNOWN，位置判定由 get_position() 统一负责
 *------------------------------------------------------------------*/
void indicator_show(uint8 pos)
{
    switch(pos) {
    case POS_A:       OUT3 = 0; OUT4 = 1; break;  // 停在岔位A
    case POS_B:       OUT3 = 1; OUT4 = 0; break;  // 停在岔位B
    default:          OUT3 = 0; OUT4 = 0; break;  // 异常：两灯全灭（LED快闪 + 485上报）
    }
}

void indicator_off(void)   // 全部位置指示灯熄灭
{
    OUT3 = 0; OUT4 = 0;
}

/*-------------------------------------------------------------------
 * 输入读取：统一屏蔽"低电平有效"的极性细节
 *------------------------------------------------------------------*/
uint8 sw1_pressed(void)        { return (SW1 == 0); }        // 功能键按下
uint8 key_turn_pressed(void)   { return (IN1 == 0); }        // 手动转换按钮按下
uint8 pos_a_active(void)       { return (POS_SIG_A == 0); }  // 岔位A信号有效
uint8 pos_b_active(void)       { return (POS_SIG_B == 0); }  // 岔位B信号有效
uint8 mode_is_full(void)       { return (IN5 != 0); }        // 1=全程模式，0=点动模式

// ============================================================================
// 第 3 层：输入扫描 —— 把"电平"翻译成"事件"
// 全部由定时器中断以 100us 周期调用，不要在扫描函数里做耗时操作
// ============================================================================

/*-------------------------------------------------------------------
 * 积分型消抖：低电平连续累计 → 判有效；高电平连续递减 → 判无效
 * 比"延时再读一次"响应更快，且能抑制抖动和尖峰干扰
 *------------------------------------------------------------------*/
#define DEBOUNCE_UPDATE(raw_active, tick, state)          \
    do {                                                  \
        if(raw_active) {                                  \
            if((tick) < DEBOUNCE_CAP) (tick)++;           \
            if((tick) > DEBOUNCE_ON) (state) = 1;         \
        } else {                                          \
            if((tick) > 0) (tick)--;                      \
            if((tick) < DEBOUNCE_OFF) (state) = 0;        \
        }                                                 \
    } while(0)

/*-------------------------------------------------------------------
 * 岔位信号扫描：更新 pos_a_state(岔位A) / pos_b_state(岔位B)
 *------------------------------------------------------------------*/
void sensor_scan(void)
{
    DEBOUNCE_UPDATE(pos_a_active(), pos_a_tick, pos_a_state);  // IN2 → 岔位A
    DEBOUNCE_UPDATE(pos_b_active(), pos_b_tick, pos_b_state);  // IN3 → 岔位B
}

/*-------------------------------------------------------------------
 * SW1 扫描：短按(50ms~2s)在松手时产生；长按(>6s)在按住期间就产生
 *
 * 注意：长按必须"一次按下只触发一次"。
 *   原写法在按住期间每个中断都会置 sw1_event=2，若用户按住不放，
 *   清空状态(SYS_CLEARING)会反复进出 → EEPROM 被反复擦写；
 *   且转换过程中事件被丢弃后、一回到待机又会立即再次触发清空。
 *   这里用 sw1_long_fired 锁存，松手才复位。
 *------------------------------------------------------------------*/
void sw1_scan(void)
{
    if(sw1_pressed()) {
        if(sw1_tick < 0xFFFF) {
            sw1_tick++;
        }
        if((sw1_tick > SW1_LONG_MIN) && (sw1_long_fired == 0)) {
            sw1_long_fired = 1;         // 本次按下只触发一次长按
            sw1_event = SW1_EVT_LONG;   // 长按：立即响应，不等松手
        }
    } else {
        if(sw1_tick != 0) {
            if((sw1_tick > SW1_SHORT_MIN) && (sw1_tick < SW1_SHORT_MAX) &&
               (sw1_event == SW1_EVT_NONE)) {
                sw1_event = SW1_EVT_SHORT;  // 短按：松手时才确认
            }
            sw1_tick = 0;
        }
        sw1_long_fired = 0;             // 松手解锁，准备下一次按下
    }
}

/*-------------------------------------------------------------------
 * 手动转换按钮扫描：按一次只产生一次事件
 * key_fired 保证"按住不放"不会连发，松手后复位才能再次触发
 *------------------------------------------------------------------*/
void key_turn_scan(void)
{
    if(key_turn_pressed()) {
        if(key_tick < 0xFFFF) {
            key_tick++;
        }
        if((key_tick > KEY_TURN_MIN) && (key_fired == 0)) {
            key_fired = 1;
            key_event = KEY_EVT_PRESS;
        }
    } else {
        key_tick = 0;
        key_fired = 0;                      // 松手解锁，准备下一次按下
    }
}

/*-------------------------------------------------------------------
 * 事件清除：进入/离开某些状态时调用，避免残留事件被下一状态误消费
 *------------------------------------------------------------------*/
void event_clear_all(void)
{
    sw1_event = SW1_EVT_NONE;
    key_event = KEY_EVT_NONE;
}

// ============================================================================
// 第 4 层：定时器 —— 100us 时基
// ============================================================================

/*-------------------------------------------------------------------
 * 定时器0初始化：24MHz / 12T，初值 0xFF38 → 溢出周期约 100us
 * 必须设为 16 位模式1，并在中断里手动重装初值（模式1不自动重装）
 *------------------------------------------------------------------*/
void Timer0Init(void)
{
    AUXR &= 0x7f;       // 定时器时钟 12T 模式
    TMOD &= 0xf0;       // 清 Timer0 模式位
    TMOD |= 0x01;       // Timer0 = 16位模式1
    TL0 = 0x38;         // 初值低8位
    TH0 = 0xff;         // 初值高8位
}

/*-------------------------------------------------------------------
 * 定时器0中断：只做三件事——喂时基、重装初值、调用各扫描函数
 * 中断里不放业务逻辑，便于单独测试和替换
 *------------------------------------------------------------------*/
void T_IRQ0(void) interrupt 1 using 1
{
    TL0 = 0x38;         // 重装初值（模式1必须手动重装，否则周期会变成长溢出）
    TH0 = 0xff;

    if(fsm_timer_en) {
        fsm_timer++;    // 状态机计时（由 timer_start/timer_stop 控制）
    }
    blink_tick++;       // LED闪烁计时（常开，独立于状态机计时）

    /* ---- 10ms基准：统计"遥控静默时长"，用于防重复触发 ---- */
    if(++rf_silence_tick >= 100) {
        rf_silence_tick = 0;
        if(rf_silence < 255) {
            rf_silence++;
        }
    }

    uart2_tick();       // RS485：帧间隔计时 + 发送完成后的方向关断倒计时

    sw1_scan();         // SW1 按键
    key_turn_scan();    // 手动转换按钮
    sensor_scan();      // 到位开关
}

/*-------------------------------------------------------------------
 * 定时器操作：状态机只通过这三个函数使用计时器，不直接碰 fsm_timer
 *------------------------------------------------------------------*/
void timer_start(void)                  // 从0开始计时（切换状态时调用）
{
    fsm_timer = 0;
    fsm_timer_en = 1;
}

void timer_stop(void)                   // 停止计时
{
    fsm_timer_en = 0;
}

uint8 timer_is_timeout(uint16 limit)    // 1=已超过 limit 个 tick
{
    return (fsm_timer > limit);
}

/*-------------------------------------------------------------------
 * 按 IN5 模式开关返回对应超时值
 *------------------------------------------------------------------*/
uint16 timeout_by_mode(uint16 jog_val, uint16 full_val)
{
    if(mode_is_full()) {
        return full_val;    // IN5=1 全程模式：较长超时
    }
    return jog_val;         // IN5=0 点动模式：较短超时
}

// ============================================================================
// 第 5 层 - A：位置判定（到位开关 → 位置），全文件唯一判定入口
// ============================================================================

/*-------------------------------------------------------------------
 * 当前岔位：两个岔位信号互斥，正常时必有一个有效
 *   岔位A信号有效、岔位B无效 → 停在A位
 *   岔位B信号有效、岔位A无效 → 停在B位
 *   其余（都有效/都无效）→ 岔位异常，此时不允许转换
 *------------------------------------------------------------------*/
uint8 get_position(void)
{
    if((pos_a_state == 1) && (pos_b_state == 0)) {
        return POS_A;
    }
    if((pos_a_state == 0) && (pos_b_state == 1)) {
        return POS_B;
    }
    return POS_UNKNOWN;
}

/*-------------------------------------------------------------------
 * 出发位 / 目标位信号判定
 *
 * 【两阶段确认】一次转换只动一个阀，但位置信号要确认两次：
 *   1) 出发位信号消失  = 道岔确实动了（防止"信号没变就误判完成"）
 *   2) 目标位信号出现  = 道岔已到达，此时立即断电
 *
 *   例：从 A 位转向 B 位（turn_direction = DIR_A_TO_B）
 *       出发位 = A（pos_a），目标位 = B（pos_b）
 *       得电 → 等 pos_a 变无效 → 等 pos_b 变有效 → 断电
 *
 * ★ 只要任一端信号有效（即道岔明确停在某一位），两个阀都不输出；
 *   只有两端都无效（道岔卡在中间）才允许反向返回。
 *------------------------------------------------------------------*/
uint8 start_pos_active(void)    // 出发位信号是否仍然有效（=道岔还没离开）
{
    if(turn_direction == DIR_A_TO_B) {
        return (pos_a_state == 1);  // 从A出发 → 看岔位A信号
    }
    return (pos_b_state == 1);      // 从B出发 → 看岔位B信号
}

uint8 target_pos_reached(void)  // 目标位信号是否已有效（=道岔已到位）
{
    if(turn_direction == DIR_A_TO_B) {
        return (pos_b_state == 1);  // 去B位 → 等岔位B信号
    }
    return (pos_a_state == 1);      // 去A位 → 等岔位A信号
}

/* 显示当前位置指示灯 */
void show_position(void)
{
    indicator_show(get_position());
}

// ============================================================================
// 第 5 层 - B：遥控器配对存储（EEPROM）
// ============================================================================

/*-------------------------------------------------------------------
 * 读取并校正"已配对字节数"
 * 全新芯片 EEPROM 擦除后为 0xFF(255)，若直接拿去和 240 比较会被判成"已满"，
 * 导致首次配对永远失败。这里把 0xFF、非4倍数、超过240 的非法值统一按 0 处理。
 *------------------------------------------------------------------*/
uint8 get_pair_count(void)
{
    uint8 count;

    count = eeprom_buf[EEPROM_PAIR_COUNT];
    if((count > PAIR_MAX_BYTES) || ((count % PAIR_SLOT_BYTES) != 0)) {
        return 0;           // 未初始化或数据损坏 → 视为"尚未配对"
    }
    return count;
}

/* 判断第 slot 组是否为空白槽（擦除后全0xFF，清零后全0x00） */
uint8 is_slot_blank(uint8 slot)
{
    uint8 base;

    base = (uint8)(slot * PAIR_SLOT_BYTES);
    if((eeprom_buf[base] == 0xFF) && (eeprom_buf[base + 1] == 0xFF) &&
       (eeprom_buf[base + 2] == 0xFF) && (eeprom_buf[base + 3] == 0xFF)) {
        return 1;           // 未写入过
    }
    if((eeprom_buf[base] == 0x00) && (eeprom_buf[base + 1] == 0x00) &&
       (eeprom_buf[base + 2] == 0x00) && (eeprom_buf[base + 3] == 0x00)) {
        return 1;           // 已清零
    }
    return 0;
}

/*-------------------------------------------------------------------
 * 判断收到的编码是否已配对
 * 只遍历"实际已配对"的组数，并跳过空白槽，避免把空槽误判为有效配对
 *------------------------------------------------------------------*/
uint8 check_rf_match(uint32 val)
{
    uint8 i, n, base;
    uint8 b0, b1, b2, b3;

    if(val == 0) {
        return 0;               // 空编码不参与匹配
    }

    b0 = (uint8)(val & 0xFF);          // 拆分32位编码为4字节
    b1 = (uint8)((val >> 8) & 0xFF);
    b2 = (uint8)((val >> 16) & 0xFF);
    b3 = (uint8)((val >> 24) & 0xFF);

    n = get_pair_count() / PAIR_SLOT_BYTES;     // 实际已配对的组数

    for(i = 0; i < n; i++) {
        if(is_slot_blank(i)) {
            continue;           // 跳过空白槽
        }
        base = (uint8)(i * PAIR_SLOT_BYTES);
        if((eeprom_buf[base + 0] == b0) &&
           (eeprom_buf[base + 1] == b1) &&
           (eeprom_buf[base + 2] == b2) &&
           (eeprom_buf[base + 3] == b3)) {
            return 1;           // 4字节全匹配 → 已配对
        }
    }
    return 0;
}

/*-------------------------------------------------------------------
 * 保存一个遥控器编码
 * 返回：1=成功（或此前已配对过），0=失败（存储空间已满）
 * 同一遥控器不会重复占用槽位；EEPROM 需先擦除整扇区再整页写回
 *------------------------------------------------------------------*/
uint8 save_rf_pair(uint32 val)
{
    uint8 count;

    if(check_rf_match(val)) {
        return 1;               // 已配对过，不重复占槽
    }

    count = get_pair_count();
    if(count >= PAIR_MAX_BYTES) {
        return 0;               // 已满（60组）
    }

    eeprom_sector_erase(0);     // 擦除扇区（EEPROM写前必须擦）
    eeprom_buf[(uint16)(count + 0)] = (uint8)(val & 0xFF);
    eeprom_buf[(uint16)(count + 1)] = (uint8)((val >> 8) & 0xFF);
    eeprom_buf[(uint16)(count + 2)] = (uint8)((val >> 16) & 0xFF);
    eeprom_buf[(uint16)(count + 3)] = (uint8)((val >> 24) & 0xFF);
    eeprom_buf[EEPROM_PAIR_COUNT] = count + PAIR_SLOT_BYTES;   // 更新计数
    eeprom_write_uchar(0, eeprom_buf, EEPROM_BUF_SIZE);        // 整页写回
    return 1;
}

/*-------------------------------------------------------------------
 * 清空全部配对
 * 注意：必须写回 EEPROM！只擦扇区、只清 RAM 缓存都会导致重启后读回 0xFF，
 * 清空等于没生效。
 *------------------------------------------------------------------*/
void clear_all_pairs(void)
{
    uint8 i;

    eeprom_sector_erase(0);
    for(i = 0; i < EEPROM_BUF_SIZE; i++) {
        eeprom_buf[i] = 0;
    }
    eeprom_write_uchar(0, eeprom_buf, EEPROM_BUF_SIZE);   // 保持缓存与EEPROM一致
}

// ============================================================================
// 第 5 层 - C：道岔子状态机
// 每个状态一个独立小函数，返回"下一个状态"；本函数只负责分发
// 好处：单个状态的逻辑可以独立阅读/修改，不会互相牵连
// ============================================================================

/*-------------------------------------------------------------------
 * 统一进入失败状态：切断气源 → 复位计时 → 复位闪烁相位
 * 所有异常分支都走这里，保证失败处理只有一处、不会漏掉断电
 *------------------------------------------------------------------*/
TurnState turn_goto_fail(void)
{
    solenoid_all_off();     // 断电切气源（气动系统最安全的处置）
    timer_start();          // 开始计失败提示时长
    blink_tick = 0;         // 复位LED闪烁相位，保证从熄灭开始闪
    led_off();
    last_turn_result = MB_RES_FAIL;     // 记下结果，供上位机查询
    return TURN_FAIL;
}

/*-------------------------------------------------------------------
 * 统一进入成功状态：切断气源 → 复位计时 → 复位闪烁相位
 * ★ 只要道岔已到位（任一端位置信号有效），两个阀都必须断电、且不再输出。
 *   所有"到位"出口都走这里，杜绝"忘了断电"的漏改。
 *------------------------------------------------------------------*/
TurnState turn_goto_success(void)
{
    solenoid_all_off();
    timer_start();
    blink_tick = 0;
    led_off();
    last_turn_result = MB_RES_SUCCESS;  // 记下结果，供上位机查询
    return TURN_SUCCESS;
}

/*-------------------------------------------------------------------
 * TURN_DETECT：读取当前岔位，推导本次转换方向
 * 停在A就转B、停在B就转A —— 所以任何触发源（遥控/手动按钮）都只需
 * 调用 turnout_start()，无需关心方向。
 *------------------------------------------------------------------*/
TurnState turn_st_detect(void)
{
    uint8 pos;

    pos = get_position();

    if(pos == POS_A) {
        turn_direction = DIR_A_TO_B;
        timer_start();
        return TURN_SOL;
    }
    if(pos == POS_B) {
        turn_direction = DIR_B_TO_A;
        timer_start();
        return TURN_SOL;
    }

    /* 岔位异常（两个位置信号都有效或都无效）：不允许转换，直接报失败 */
    return turn_goto_fail();
}

/*-------------------------------------------------------------------
 * TURN_SOL：给"目标方向对应的那个阀"得电
 * 先保证 T_SOL_PULSE 让阀芯可靠动作（最短得电时间）。
 * 若机构动作很快、目标位信号在这个阶段就已有效，则立即断电，不多供电。
 *------------------------------------------------------------------*/
TurnState turn_st_sol(void)
{
    sol_valve_on();

    if(target_pos_reached()) {
        return turn_goto_success();     // 已到位 → 立即断电
    }
    if(timer_is_timeout(T_SOL_PULSE)) {
        timer_start();
        return TURN_LEAVE;
    }
    return TURN_SOL;
}

/*-------------------------------------------------------------------
 * TURN_LEAVE（阶段1）：等"出发位信号消失"
 * 意义：确认道岔真的动了。若一直是出发位信号有效，说明阀/气路/机构有问题，
 *       继续供气没有意义，直接断电报错。
 * 注意：这个阶段**保持得电**，阀继续推动。
 *------------------------------------------------------------------*/
TurnState turn_st_leave(void)
{
    uint16 timeout;

    sol_valve_on();                     // 保持得电（每轮重复写，状态明确）
    timeout = timeout_by_mode(T_LEAVE_POS, T_LEAVE_POS_Q);

    if(start_pos_active() == 0) {   // 出发位信号已消失 → 道岔离开了
        timer_start();
        return TURN_REACH;
    }
    if(timer_is_timeout(timeout)) {
        return turn_goto_fail();    // 一直没动 → 断电报错
    }
    return TURN_LEAVE;
}

/*-------------------------------------------------------------------
 * TURN_REACH（阶段2）：等"目标位信号出现" = 到位
 * 到位 → 立即断电，OUT1/OUT2 都不再输出（这是最关键的一步：到位后禁止输出）
 * 超时 → 说明道岔卡在中间（两端信号都无效），才允许进入"返回"流程：
 *        点动模式 → 反向返回出发位；
 *        全程模式 → 直接报错不返回（行程长，反向推回风险大）
 *------------------------------------------------------------------*/
TurnState turn_st_reach(void)
{
    uint16 timeout;

    sol_valve_on();                     // 保持得电
    timeout = timeout_by_mode(T_REACH_POS, T_REACH_POS_Q);

    if(target_pos_reached()) {
        return turn_goto_success();     // 到位 → 立即断电
    }

    if(timer_is_timeout(timeout)) {
        solenoid_all_off();             // 不到位也先断电，准备返回
        timer_start();
        blink_tick = 0;
        led_off();
        if(mode_is_full()) {
            return TURN_FAIL;           // 全程模式：不返回，直接报错
        }
        return TURN_RET_WAIT;           // 点动模式：等 1.5 秒后反向返回
    }
    return TURN_REACH;
}

/*-------------------------------------------------------------------
 * TURN_RET_WAIT：返回前的泄压等待（1.5秒），LED 快闪提示
 * 给气缸一点时间泄压，避免立刻反向冲击
 *------------------------------------------------------------------*/
TurnState turn_st_ret_wait(void)
{
    if(blink_tick > T_BLINK_FAST) {
        blink_tick = 0;
        led_toggle();
    }
    if(timer_is_timeout(T_RET_WAIT)) {
        led_off();
        timer_start();
        return TURN_RETURN;
    }
    return TURN_RET_WAIT;
}

/*-------------------------------------------------------------------
 * TURN_RETURN：给**反向**的阀得电，把道岔推回出发位
 * 等"出发位信号恢复"即认为已回到原位 → 断电 → 按成功处理（走 SUCCESS 提示）
 * 返回同样有超时保护，超时则断电报错
 *------------------------------------------------------------------*/
TurnState turn_st_return(void)
{
    sol_return_valve_on();      // 给反向的阀得电，把道岔推回出发位

    if(start_pos_active()) {    // 出发位信号恢复 = 已回到原位
        return turn_goto_success();
    }

    if(timer_is_timeout(T_RETURN)) {
        return turn_goto_fail();
    }
    return TURN_RETURN;
}

/*-------------------------------------------------------------------
 * TURN_SUCCESS：到位后的保持阶段
 * 此时两个阀都已断电，只做两件事：刷新位置指示灯 + LED 慢闪提示成功
 * T_SUCCESS 后熄灭 LED 并回到空闲，等待下一次触发
 *------------------------------------------------------------------*/
TurnState turn_st_success(void)
{
    show_position();

    if(blink_tick > T_BLINK_SLOW) {
        blink_tick = 0;
        led_toggle();
    }
    if(timer_is_timeout(T_SUCCESS)) {
        led_off();
        timer_stop();
        return TURN_IDLE;
    }
    return TURN_SUCCESS;
}

/*-------------------------------------------------------------------
 * TURN_FAIL：LED 快闪提示（50ms 半周期），T_FAIL 后熄灭并回空闲
 * 进入本状态时一定已经断电（由 turn_goto_fail 保证）
 *------------------------------------------------------------------*/
TurnState turn_st_fail(void)
{
    if(blink_tick > T_BLINK_FAST) {
        blink_tick = 0;
        led_toggle();
    }
    if(timer_is_timeout(T_FAIL)) {
        led_off();
        timer_stop();
        return TURN_IDLE;
    }
    return TURN_FAIL;
}

/*-------------------------------------------------------------------
 * 子状态机分发：只做 switch，不含业务逻辑
 *------------------------------------------------------------------*/
void turnout_fsm_run(void)
{
    /* ===== 输出安全阀 =====
     * 这些状态下两个阀都必须断电。每轮调度重复写一次，而不只是"切换时写一次"，
     * 这样即使别处误写了输出，下一轮也会被强制纠正 —— 保证"到位后不再输出"。
     * ------------------------------------------------------------------*/
    switch(turn_state) {
    case TURN_IDLE:
    case TURN_SUCCESS:
    case TURN_RET_WAIT:
    case TURN_FAIL:
        solenoid_all_off();
        break;
    default:
        break;                  /* 其余状态由阀自身控制得电 */
    }

    switch(turn_state) {
    case TURN_IDLE:      break;                                   /* 等待 turnout_start() */
    case TURN_DETECT:    turn_state = turn_st_detect();    break;
    case TURN_SOL:       turn_state = turn_st_sol();       break;
    case TURN_LEAVE:     turn_state = turn_st_leave();     break;
    case TURN_REACH:     turn_state = turn_st_reach();     break;
    case TURN_SUCCESS:   turn_state = turn_st_success();   break;
    case TURN_RET_WAIT:  turn_state = turn_st_ret_wait();  break;
    case TURN_RETURN:    turn_state = turn_st_return();    break;
    case TURN_FAIL:      turn_state = turn_st_fail();      break;
    }
}

/*-------------------------------------------------------------------
 * 启动一次转换：方向由子状态机自己检测，调用方不需要指定
 *------------------------------------------------------------------*/
void turnout_start(void)
{
    solenoid_all_off();         // 确保从"全断电"开始
    turn_direction = DIR_NONE;
    turn_state = TURN_DETECT;
    timer_stop();               // 计时由各子状态自行管理
}

/*-------------------------------------------------------------------
 * 复位道岔子状态机：断电 + 停表 + 回到空闲
 * 用于上电初始化和状态复位；注意这不是"急停"——气动系统靠每步超时
 * 自动断电（turn_goto_fail）来保证安全，中途人为断电反而会让气缸停在中间。
 *------------------------------------------------------------------*/
void turnout_reset(void)
{
    solenoid_all_off();
    timer_stop();
    turn_direction = DIR_NONE;
    turn_state = TURN_IDLE;
}

// ============================================================================
// 第 5.5 层：RS485 / Modbus —— 把"读写寄存器"翻译成业务动作
//   协议帧的解析与组帧在 myLib/modbus.h，本层只定义"寄存器是什么意思"
// ============================================================================

/*-------------------------------------------------------------------
 * 保存从站地址到 EEPROM
 * EEPROM 只能整扇区擦除，所以做法是：改缓存 → 擦除 → 整页写回。
 * 这样同一扇区里的遥控器配对数据不会丢（缓存里始终有一份完整副本）。
 *------------------------------------------------------------------*/
void save_slave_addr(uint8 addr)
{
    eeprom_buf[EEPROM_SLAVE_ADDR] = addr;
    eeprom_sector_erase(0);
    eeprom_write_uchar(0, eeprom_buf, EEPROM_BUF_SIZE);
}

/*-------------------------------------------------------------------
 * 读保持寄存器（由协议层回调）
 * 返回 0=成功；非0=Modbus异常码
 *------------------------------------------------------------------*/
uint8 mb_read_reg(uint16 addr, uint16 *val)
{
    uint16 st;

    st = 0;
    switch(addr) {
    case MB_REG_STATUS:                     // 运行状态字
        if(pos_a_state)          st |= MB_ST_POS_A;
        if(pos_b_state)          st |= MB_ST_POS_B;
        if(get_position() == POS_UNKNOWN) st |= MB_ST_ABNORMAL;
        if(turn_state != TURN_IDLE)      st |= MB_ST_TURNING;
        if(last_turn_result == MB_RES_SUCCESS) st |= MB_ST_LAST_OK;
        if(last_turn_result == MB_RES_FAIL)    st |= MB_ST_LAST_FAIL;
        if(mode_is_full())       st |= MB_ST_MODE_FULL;
        *val = st;
        return 0;

    case MB_REG_POSITION:                   // 当前岔位
        *val = (uint16)get_position();
        return 0;

    case MB_REG_COMMAND:                    // 命令寄存器（读到0表示已执行完毕）
        *val = (uint16)mb_cmd;
        return 0;

    case MB_REG_RESULT:                     // 上次转换结果
        *val = (uint16)last_turn_result;
        return 0;

    case MB_REG_SLAVE_ADDR:                 // 本站地址
        *val = (uint16)mb_slave_addr;
        return 0;

    case MB_REG_MODE:                       // 点动/全程
        *val = mode_is_full() ? 1 : 0;
        return 0;

    case MB_REG_VERSION:                    // 固件版本
        *val = MB_FW_VERSION;
        return 0;

    /* ---- 通讯诊断计数：无线与总线重叠时用来定位丢帧原因 ---- */
    case MB_REG_RX_OVF:                     // 串口接收缓冲溢出（丢字节）次数
        *val = (uint16)u2_rx_ovf;
        return 0;

    case MB_REG_CRC_ERR:                    // CRC校验失败帧数
        *val = (uint16)mb_crc_err;
        return 0;

    case MB_REG_TX_RESET:                   // 发送看门狗自愈次数
        *val = (uint16)u2_tx_reset;
        return 0;

    default:
        return MB_EX_ADDR;                  // 未定义地址 → 非法数据地址
    }
}

/*-------------------------------------------------------------------
 * 写保持寄存器（由协议层回调）
 *
 * 注意：写命令寄存器只是"置一个待执行标志"，不在这里启动转换 ——
 * 真正的启动统一放在主状态机 SYS_IDLE 里消费（见 sys_st_idle），
 * 保证任何时刻只有一条转换流程在跑，遥控/按钮/485 三种触发源不会打架。
 *------------------------------------------------------------------*/
uint8 mb_write_reg(uint16 addr, uint16 val)
{
    switch(addr) {
    case MB_REG_COMMAND:
        if((val == MB_CMD_TOGGLE) ||
           (val == MB_CMD_GOTO_A) ||
           (val == MB_CMD_GOTO_B) ||
           (val == MB_CMD_NONE)) {
            mb_cmd = (uint8)val;            // 写0 = 撤销尚未执行的命令
            return 0;
        }
        return MB_EX_VALUE;

    case MB_REG_SLAVE_ADDR:
        if((val < MB_ADDR_MIN) || (val > MB_ADDR_MAX)) {
            return MB_EX_VALUE;
        }
        mb_slave_addr = (uint8)val;         // 立即生效，不用重启
        /* 落盘延后：EEPROM 扇区擦除要关中断 4~10ms，若在这里同步写，
         * 应答帧还没发出去就被卡住（主站会超时）。这里只登记，
         * 由 loop() 在应答发出后、系统空闲时再真正写 EEPROM。 */
        mb_addr_pending     = 1;
        mb_addr_pending_val = (uint8)val;
        return 0;

    case MB_REG_RESULT:
        if(val == MB_RES_NONE) {            // 只允许清0，用于清除结果标志
            last_turn_result = MB_RES_NONE;
            return 0;
        }
        return MB_EX_VALUE;

    /* ---- 通讯诊断计数：只允许写 0 清零 ---- */
    case MB_REG_RX_OVF:
        if(val == 0) { u2_rx_ovf   = 0; return 0; }
        return MB_EX_VALUE;

    case MB_REG_CRC_ERR:
        if(val == 0) { mb_crc_err  = 0; return 0; }
        return MB_EX_VALUE;

    case MB_REG_TX_RESET:
        if(val == 0) { u2_tx_reset = 0; return 0; }
        return MB_EX_VALUE;

    default:
        return MB_EX_ADDR;
    }
}

/*-------------------------------------------------------------------
 * RS485 轮询：收到完整帧 → 交给协议层 → 有应答就发出去
 * 必须在主循环里频繁调用；单次耗时与帧长成正比（微秒级，不阻塞）
 *------------------------------------------------------------------*/
void modbus_poll(void)
{
    uint8 n;

    if(!uart2_frame_ready()) {
        return;
    }
    n = uart2_take_frame(mb_req, MB_MAX_FRAME);
    mb_handle(mb_req, n);
    if(mb_tx_len > 0) {
        uart2_send(mb_tx, mb_tx_len);
    }
}

// ============================================================================
// 第 6 层：系统主状态机
// 每个状态一个函数，返回"下一个状态"；本层只做调度，不做具体动作
// ============================================================================

/*-------------------------------------------------------------------
 * 统一的"启动一次转换"入口：三种触发源共用
 *   手动按钮(P3.2) / 已配对遥控(RF) / RS485转岔命令
 * 作用：清掉可能残留的遥控状态 → 启动子状态机 → 切到 SYS_TURNING。
 * 所有触发源都走这里，保证任何时刻只有一条转换流程在跑。
 *------------------------------------------------------------------*/
SystemState turnout_request(void)
{
    rf_armed = 0;               // 本次触发已消费，遥控需松手后重新武装
    rf_received = 0;
    blink_active = 0;           // 转换优先：取消可能未完成的提示闪烁，避免与转换闪烁抢 LED
    turnout_start();            // 转换方向由子状态机自动判定
    return SYS_TURNING;
}

/*-------------------------------------------------------------------
 * SYS_INIT：上电自检
 * 复位所有输出 → 读取EEPROM → 校正非法配对计数 → 进入待机
 *------------------------------------------------------------------*/
SystemState sys_st_init(void)
{
    turnout_reset();            // 切断气源 + 停表 + 子状态机复位
    indicator_off();            // 熄灭位置指示灯
    led_off();                  // 熄灭LED
    BUZZER_OFF();               // 关闭蜂鸣器
    event_clear_all();          // 丢弃上电瞬间的抖动事件

    mb_cmd = MB_CMD_NONE;               // 清掉待执行的转岔命令
    last_turn_result = MB_RES_NONE;     // 清掉上次转换结果

    eeprom_read_uchar(0, eeprom_buf, EEPROM_BUF_SIZE);

    /* 全新芯片或数据损坏时配对数为非法值（如0xFF），这里初始化一次，
     * 保证首次配对可用（否则会被判成"已满"而永远存不进去） */
    if((eeprom_buf[EEPROM_PAIR_COUNT] > PAIR_MAX_BYTES) ||
       ((eeprom_buf[EEPROM_PAIR_COUNT] % PAIR_SLOT_BYTES) != 0)) {
        clear_all_pairs();
    }

    /* Modbus 从站地址：全新芯片 EEPROM 为 0xFF（非法），此时用默认值 */
    if((eeprom_buf[EEPROM_SLAVE_ADDR] < MB_ADDR_MIN) ||
       (eeprom_buf[EEPROM_SLAVE_ADDR] > MB_ADDR_MAX)) {
        mb_slave_addr = MB_ADDR_DEFAULT;
    } else {
        mb_slave_addr = eeprom_buf[EEPROM_SLAVE_ADDR];
    }

    return SYS_IDLE;
}

/*-------------------------------------------------------------------
 * SYS_IDLE：待机
 * 三种触发源在这里汇总：
 *   SW1短按   → 配对
 *   SW1长按   → 清空配对
 *   手动按钮  → 转换一次位置
 *   已配对RF  → 转换一次位置
 *------------------------------------------------------------------*/
SystemState sys_st_idle(void)
{
    uint8 cmd;

    show_position();            // 实时刷新位置指示灯

    if(sw1_event == SW1_EVT_SHORT) {
        sw1_event = SW1_EVT_NONE;
        led_on();               // 亮灯表示已进入配对模式
        return SYS_PAIRING;
    }
    if(sw1_event == SW1_EVT_LONG) {
        sw1_event = SW1_EVT_NONE;
        return SYS_CLEARING;
    }

    if(key_event == KEY_EVT_PRESS) {
        key_event = KEY_EVT_NONE;
        return turnout_request();       // 手动按钮：转换一次位置
    }

    /* ---- RS485转岔命令（与手动按钮同等优先级） ----
     * 上位机写命令寄存器时只是"置一个标志"（见 mb_write_reg），
     * 真正启动统一在这里消费，避免三种触发源同时启动转换流程。
     *--------------------------------------------------*/
    if(mb_cmd != MB_CMD_NONE) {
        cmd = mb_cmd;
        mb_cmd = MB_CMD_NONE;           // 先清零，避免同一条命令被重复执行

        if(cmd == MB_CMD_TOGGLE) {
            return turnout_request();   // 切换一次
        }
        if(cmd == MB_CMD_GOTO_A) {
            if(get_position() == POS_A) {
                last_turn_result = MB_RES_SUCCESS;   // 已在A位：不动作，直接报成功
            } else {
                return turnout_request();
            }
        }
        if(cmd == MB_CMD_GOTO_B) {
            if(get_position() == POS_B) {
                last_turn_result = MB_RES_SUCCESS;   // 已在B位：不动作，直接报成功
            } else {
                return turnout_request();
            }
        }
    }

    /* ---- RF触发：必须"重新武装"后才生效 ----
     * 1527遥控器按住不放会连续发帧。若不设防，一次按住会被识别成多次触发，
     * 表现为"转换刚到位、阀立刻又得电"。规则：触发一次后必须等遥控松手
     * （连续 RF_SILENCE_MIN 无新帧）才允许下一次触发。
     *--------------------------------------------------*/
    if(rf_armed == 0) {
        if(rf_silence >= RF_SILENCE_MIN) {
            rf_armed = 1;       // 遥控已松开 → 重新武装
        }
        rf_received = 0;        // 未武装期间收到的帧全部丢弃
    } else if(rf_received) {
        rf_received = 0;
        if(check_rf_match(rf_value)) {
            return turnout_request();   // 已配对遥控：转换一次位置
        }
        /* 未配对的编码直接忽略，不做任何提示 */
    }

    return SYS_IDLE;
}

/*-------------------------------------------------------------------
 * SYS_PAIRING：等待遥控器信号并保存
 * 仅接受指定协议和位长的编码；按 SW1 可随时退出
 *------------------------------------------------------------------*/
SystemState sys_st_pairing(void)
{
    if(rf_received) {
        rf_received = 0;
        if((RFgetReceivedProtocol() == RF_PROTOCOL) &&
           (RFgetReceivedBitlength() == RF_BITLENGTH)) {
            save_rf_pair(rf_value);
            led_blink_start(3, 100);     // 闪3次提示配对成功（非阻塞，主循环驱动）
            return SYS_PAIRING_WAIT;
        }
    }

    if(sw1_event != SW1_EVT_NONE) {     // 按SW1退出配对
        event_clear_all();
        led_off();
        return SYS_IDLE;
    }

    if(key_event == KEY_EVT_PRESS) {    // 配对期间手动按钮无效，直接丢弃
        key_event = KEY_EVT_NONE;
    }

    return SYS_PAIRING;
}

/*-------------------------------------------------------------------
 * SYS_PAIRING_WAIT：等遥控器松手（信号消失）后回待机
 * 防止同一次按住被重复配对
 *------------------------------------------------------------------*/
SystemState sys_st_pairing_wait(void)
{
    if(!RFavailable()) {
        led_off();
        return SYS_IDLE;
    }
    if(sw1_event != SW1_EVT_NONE) {
        event_clear_all();
        led_off();
        return SYS_IDLE;
    }
    return SYS_PAIRING_WAIT;
}

/*-------------------------------------------------------------------
 * SYS_CLEARING：清空全部配对数据，闪灯提示后回待机
 *------------------------------------------------------------------*/
SystemState sys_st_clearing(void)
{
    clear_all_pairs();
    led_blink_start(1, 200);            // 闪1次提示已清空（非阻塞）
    event_clear_all();          // 丢弃清空期间的按键事件
    return SYS_IDLE;
}

/*-------------------------------------------------------------------
 * SYS_TURNING：转换中，驱动子状态机
 * 子状态机回到 TURN_IDLE 即表示本次转换结束
 *
 * 说明：气动系统**没有急停概念** —— 电磁阀一旦断电，气源立即切断，
 *       气缸会停在行程中间，反而更危险；而转换流程本身每步都有超时保护，
 *       超时会自动断电（见 turn_goto_fail）。因此转换过程中不响应 SW1，
 *       按键事件一律丢弃，避免转换一结束就被残留事件误触发配对/清空。
 *------------------------------------------------------------------*/
SystemState sys_st_turning(void)
{
    turnout_fsm_run();

    /* 转换过程中的触发源一律丢弃：
     *   按键事件 —— 避免"转换一结束"就被残留事件再次触发；
     *   RF      —— 避免遥控按住不放或空间干扰在转换期间堆积，
     *              一回到待机就立刻再启动一次（表现为"到位后阀又得电"）。
     * 气动系统没有急停概念：阀一旦断电气源即切断，气缸会停在行程中间，
     * 反而更危险；每步都有超时自动断电保护（见 turn_goto_fail）。 */
    event_clear_all();
    rf_received = 0;
    mb_cmd = MB_CMD_NONE;   // 转换期间的485命令同样丢弃（上位机应先查状态字bit3确认不忙）

    if(turn_state == TURN_IDLE) {
        return SYS_IDLE;            // 子状态机跑完，回待机
    }

    return SYS_TURNING;
}

/*-------------------------------------------------------------------
 * 系统主状态机分发：只做 switch，不含业务逻辑
 *
 *   SYS_INIT ──→ SYS_IDLE
 *                  │  SW1短按 → SYS_PAIRING → SYS_PAIRING_WAIT ─┐
 *                  │  SW1长按 → SYS_CLEARING ───────────────────┤
 *                  │  手动按钮/已配对RF → SYS_TURNING ──────────┴──→ SYS_IDLE
 *------------------------------------------------------------------*/
void sys_fsm_run(void)
{
    switch(sys_state) {
    case SYS_INIT:         sys_state = sys_st_init();         break;
    case SYS_IDLE:         sys_state = sys_st_idle();         break;
    case SYS_PAIRING:      sys_state = sys_st_pairing();      break;
    case SYS_PAIRING_WAIT: sys_state = sys_st_pairing_wait(); break;
    case SYS_CLEARING:     sys_state = sys_st_clearing();     break;
    case SYS_TURNING:      sys_state = sys_st_turning();      break;
    }
}

// ============================================================================
// 第 7 层：无线接收 + 入口
// ============================================================================

/*-------------------------------------------------------------------
 * RF接收：必须在主循环中频繁调用，防止数据丢失
 * 只负责"取数 + 置标志"，是否响应交给状态机决定
 *------------------------------------------------------------------*/
void process_rf(void)
{
    if(RFavailable()) {
        rf_value = RFgetReceivedValue();
        rf_received = 1;
        rf_silence = 0;         // 刚收到帧 → 静默计时清零（说明遥控还按着/有干扰）
        RFresetAvailable();
    }
}

/*-------------------------------------------------------------------
 * 引脚模式配置
 * 与第0层引脚定义一一对应；改引脚功能时这里要同步改
 *------------------------------------------------------------------*/
void gpio_init(void)
{
    /* ★ DE(RE) 外部有 5V 上拉：复位期间 IO 高阻会被拉成"发送态"，
     *   把总线占住，别人发不了。这里第一件事就是拉回接收态，
     *   然后再配推挽输出。 */
    RS485_DIR_RX();

    /* ---- 推挽输出：需要较强驱动能力 ---- */
    P1M1 &= ~0x08;  P1M0 |= 0x08;  // P1.3(OUT1) 电磁阀A
    P1M1 &= ~0x10;  P1M0 |= 0x10;  // P1.4(OUT2) 电磁阀B
    P1M1 &= ~0x20;  P1M0 |= 0x20;  // P1.5(OUT3) 位置A指示灯
    P1M1 &= ~0x40;  P1M0 |= 0x40;  // P1.6(OUT4) 位置B指示灯
    P1M1 &= ~0x80;  P1M0 |= 0x80;  // P1.7(PB1)  蜂鸣器
    P3M1 &= ~0x20;  P3M0 |= 0x20;  // P3.5(RS485_DE) 485收发方向控制（推挽，驱动能力足）

    /* ---- 准双向口：带内部上拉，按键/开关可直接接GND ---- */
    P3M1 &= ~0x01;  P3M0 &= ~0x01;  // P3.0(SW1)        功能按键
    P3M1 &= ~0x02;  P3M0 &= ~0x02;  // P3.1(LED)        板载LED
    P3M1 &= ~0x04;  P3M0 &= ~0x04;  // P3.2(IN1)        手动转换按钮
    P3M1 &= ~0x08;  P3M0 &= ~0x08;  // P3.3(IN2)        位置B到位开关
    P3M1 &= ~0x10;  P3M0 &= ~0x10;  // P3.4(IN3)        位置A到位开关
    P3M1 &= ~0x40;  P3M0 &= ~0x40;  // P3.6(IN4)        未使用
    P3M1 &= ~0x80;  P3M0 &= ~0x80;  // P3.7(IN5)        模式选择
    P5M1 &= ~0x10;  P5M0 &= ~0x10;  // P5.4(PB9)        未使用
}

/*-------------------------------------------------------------------
 * 系统初始化：上电执行一次
 *------------------------------------------------------------------*/
void setup(void)
{
    gpio_init();                // 引脚模式

    /* ---- 状态与变量初始化 ---- */
    sys_state = SYS_INIT;
    turn_state = TURN_IDLE;
    turn_direction = DIR_NONE;
    sw1_tick = 0;
    sw1_long_fired = 0;
    key_tick = 0;
    key_fired = 0;
    fsm_timer = 0;
    blink_tick = 0;
    event_clear_all();
    rf_received = 0;
    rf_armed = 1;               // 上电允许RF触发
    rf_silence = RF_SILENCE_MIN;
    rf_silence_tick = 0;
    fsm_timer_en = 0;

    mb_cmd = MB_CMD_NONE;               // 无待执行的485命令
    last_turn_result = MB_RES_NONE;     // 尚无转换结果
    mb_slave_addr = MB_ADDR_DEFAULT;    // 真实站号在 SYS_INIT 里从EEPROM加载
    mb_addr_pending = 0;                // 无待落盘的站号
    mb_addr_pending_val = MB_ADDR_DEFAULT;
    blink_active = 0;                   // 无闪烁任务

    /* ---- 看门狗：预分频128，约6.9秒超时@24MHz ---- */
    WDT_CONTR = 0x27;

    /* ---- 外设 ---- */
    RCSwitchInit();             // 1527无线接收模块（占用 P1.2 输入捕获）
    uart2_init();               // RS485：串口2 @9600bps（P1.0/P1.1 + P3.5方向控制）

    /* ---- 中断优先级：串口2 / 100us时基 高于 无线捕获 ----
     * 无线解码在 PWMA 捕获中断(PWMA_VECTOR=26)里做，最坏会占用数百微秒~数毫秒。
     * STC8H 里同级中断**不能嵌套**，若串口2 也是默认优先级 0，解码期间串口
     * 只能排队等着；9600bps 下 1 个字节只要 1.04ms，超过就丢字节 → CRC 错 → 整帧丢。
     * 把串口2 和定时器0 提到优先级 1（PWMA 保持 0）后，串口可以打断解码，
     * 收发不再受无线影响；100us 时基同理，保证转换计时准确。
     * 优先级位：PS2/PS2H = IP2/IP2H bit0，PPWMA = IP2 bit2，PT0/PT0H = IP/IPH bit1 */
    IP2  |=  PS2;               // 串口2 优先级 = 1
    IP2H &= ~PS2H;
    PT0   = 1;                  // 定时器0 优先级 = 1
    IPH  &= ~PT0H;

    /* ---- 100us时基 ---- */
    Timer0Init();
    EA = 1;                     // 开总中断
    ET0 = 1;                    // 开定时器0中断
    TR0 = 1;                    // 启动定时器0
}

/*-------------------------------------------------------------------
 * 主循环：喂狗 + 收RF + 跑状态机
 * 保持非阻塞，单次循环耗时越短越好
 *------------------------------------------------------------------*/
void loop(void)
{
    WDT_CONTR = 55;             // 喂狗（含 CLR_WDT 位）

    process_rf();               // 读取无线数据
    modbus_poll();              // RS485：收帧 → 解析 → 应答
    sys_fsm_run();              // 运行系统主状态机
    led_blink_run();            // 闪烁提示（非阻塞，配对/清空提示用）

    /* 站号落盘：EEPROM 扇区擦除要关中断 4~10ms，必须挑"总线已发完 + 不在转换中"
     * 的时刻做。uart2_tx_idle() 保证应答已经完整发出、DE 已收回 ——
     * 否则擦除会把正在发送的应答掐出一段空白，主站会判帧错。
     * sys_state/turn_state 判断保证不会打断道岔转换的 100us 时基。 */
    if(mb_addr_pending && uart2_tx_idle() &&
       (sys_state == SYS_IDLE) && (turn_state == TURN_IDLE)) {
        mb_addr_pending = 0;
        save_slave_addr(mb_addr_pending_val);
    }
}

/*-------------------------------------------------------------------
 * 程序入口
 *------------------------------------------------------------------*/
void main(void)
{
    setup();
    while(1) {
        loop();
    }
}
