/**
 * @file rcswitch.h
 * @brief 解析1527无线的头文件
 * @details 支持433M、315M等常用频段的1527协议解析
 *          使用PWMA输入捕获功能，支持可配置输入引脚
 * @version 0.2
 * @date 2024-01-01
 * @author hhdd
 * @copyright Copyright (c) 2021 TWen51 Technology Co., Ltd.
 * 
 * ============================================================
 * 引脚配置说明（用户可在包含本文件前定义以下宏）
 * ============================================================
 * 
 * RCSWITCH_CAPTURE_CH: 选择输入捕获通道 (1=PWM1P, 2=PWM2P, 默认2)
 * 
 * RCSWITCH_PIN_MAP: 选择引脚映射 (0/1/2, 默认0)
 *   PWM1P: 0=P1.0, 1=P2.0, 2=P6.0
 *   PWM2P: 0=P1.2, 1=P2.2, 2=P6.2
 * 
 * 使用示例：
 *   // 使用PWM2P/P1.2（默认）
 *   #define RCSWITCH_CAPTURE_CH  2
 *   #define RCSWITCH_PIN_MAP     0
 *   
 *   // 使用PWM1P/P2.0
 *   #define RCSWITCH_CAPTURE_CH  1
 *   #define RCSWITCH_PIN_MAP     1
 * ============================================================
 */
#ifndef _RCSWITCH_H_
#define _RCSWITCH_H_

#include <stdlib.h>

// ========== 引脚配置（用户可自定义） ==========

#ifndef RCSWITCH_CAPTURE_CH
#define RCSWITCH_CAPTURE_CH     2   // 默认使用PWM2P
#endif

#ifndef RCSWITCH_PIN_MAP
#define RCSWITCH_PIN_MAP        0   // 默认第0组引脚
#endif

// ========== 根据配置自动生成硬件参数 ==========

#if RCSWITCH_CAPTURE_CH == 1
  // PWM1P (CC1S=01, IC1 mapped on TI1FP1)
  #define _CC1S_VALUE          0x01
  #define _CC1IF_BIT           0x02   // CC1IF中断标志位
  #if RCSWITCH_PIN_MAP == 0
    // P1.0
    #define _RCSWITCH_PS_VAL   0
    #define _RCSWITCH_PIN_MODE {P1M1|=0x01;P1M0&=~0x01;}
  #elif RCSWITCH_PIN_MAP == 1
    // P2.0
    #define _RCSWITCH_PS_VAL   1
    #define _RCSWITCH_PIN_MODE {P2M1|=0x01;P2M0&=~0x01;}
  #elif RCSWITCH_PIN_MAP == 2
    // P6.0
    #define _RCSWITCH_PS_VAL   2
    #define _RCSWITCH_PIN_MODE {P6M1|=0x01;P6M0&=~0x01;}
  #else
    #error "RCSWITCH_PIN_MAP for PWM1P must be 0, 1, or 2"
  #endif
#elif RCSWITCH_CAPTURE_CH == 2
  // PWM2P (CC1S=10, IC1 mapped on TI2FP1)
  #define _CC1S_VALUE          0x02
  #define _CC1IF_BIT           0x02   // CC1IF中断标志位
  #if RCSWITCH_PIN_MAP == 0
    // P1.2
    #define _RCSWITCH_PS_VAL   0
    #define _RCSWITCH_PIN_MODE {P1M1|=0x04;P1M0&=~0x04;}
  #elif RCSWITCH_PIN_MAP == 1
    // P2.2
    #define _RCSWITCH_PS_VAL   1
    #define _RCSWITCH_PIN_MODE {P2M1|=0x04;P2M0&=~0x04;}
  #elif RCSWITCH_PIN_MAP == 2
    // P6.2
    #define _RCSWITCH_PS_VAL   2
    #define _RCSWITCH_PIN_MODE {P6M1|=0x04;P6M0&=~0x04;}
  #else
    #error "RCSWITCH_PIN_MAP for PWM2P must be 0, 1, or 2"
  #endif
#else
  #error "RCSWITCH_CAPTURE_CH must be 1 or 2"
#endif

// 向后兼容：如果用户未定义BLUERF_RECV_PIN/BLUERF_RECV_PIN_OUT，则自动生成
#ifndef BLUERF_RECV_PIN
  #if RCSWITCH_CAPTURE_CH == 1
    #if RCSWITCH_PIN_MAP == 0
      #define BLUERF_RECV_PIN         P1_0
    #elif RCSWITCH_PIN_MAP == 1
      #define BLUERF_RECV_PIN         P2_0
    #elif RCSWITCH_PIN_MAP == 2
      #define BLUERF_RECV_PIN         P6_0
    #endif
  #elif RCSWITCH_CAPTURE_CH == 2
    #if RCSWITCH_PIN_MAP == 0
      #define BLUERF_RECV_PIN         P1_2
    #elif RCSWITCH_PIN_MAP == 1
      #define BLUERF_RECV_PIN         P2_2
    #elif RCSWITCH_PIN_MAP == 2
      #define BLUERF_RECV_PIN         P6_2
    #endif
  #endif
#endif

#ifndef BLUERF_RECV_PIN_OUT
#define BLUERF_RECV_PIN_OUT     _RCSWITCH_PIN_MODE
#endif

#define RCSWITCH_MAX_CHANGES    67    //最大数据长度

/* 固定解码协议。
 * 原实现在中断里 for(i=1..13) 逐个试协议，遇到噪声帧要把 13 个协议全试一遍，
 * 耗时 2~4ms。本中断与串口2 同为默认优先级，同级不能嵌套，这段时间会把串口
 * 完全屏蔽；9600bps 下 1 个字节只要 1.04ms，必然丢字节 → CRC 错 → 整帧丢弃。
 * 工程上只接受 RF_PROTOCOL（1527 = 协议1），这里固定住，解码耗时降到 ~200us。
 * ★ 若以后要支持多种遥控器，务必把解码搬到主循环，不要放回中断里。 */
#ifdef RF_PROTOCOL
  #define RCSWITCH_FIXED_PROTOCOL   RF_PROTOCOL
#else
  #define RCSWITCH_FIXED_PROTOCOL   1
#endif

typedef struct{
    uint8_t _high;
    uint8_t _low;
}RF_HighLow;

typedef struct{
    uint16 pulseLength;  //同步头高电平脉宽(us)
    RF_HighLow syncFactor; /* 同步头的高电平和低电平脉宽比 */
    RF_HighLow zero;  /* 数据0的高电平和低电平脉宽比 */
    RF_HighLow one;   /* 数据1的高电平和低电平脉宽比 */
    uint8 invertedSignal;  //如果为真，则交换所有传输中_high和_low的逻辑。
}RF_Protocol;

code RF_Protocol rf_recv_proto[] = {
  { 350, {  1, 31 }, {  1,  3 }, {  3,  1 }, 0 },    // protocol 1
  { 650, {  1, 10 }, {  1,  2 }, {  2,  1 }, 0 },    // protocol 2
  { 100, { 30, 71 }, {  4, 11 }, {  9,  6 }, 0 },    // protocol 3
  { 380, {  1,  6 }, {  1,  3 }, {  3,  1 }, 0 },    // protocol 4
  { 500, {  6, 14 }, {  1,  2 }, {  2,  1 }, 0 },    // protocol 5
  { 450, { 23,  1 }, {  1,  2 }, {  2,  1 }, 1 },     // protocol 6 (HT6P20B)
  { 150, {  2, 62 }, {  1,  6 }, {  6,  1 }, 0 },    // protocol 7 (HS2303-PT, i. e. used in AUKEY Remote)
  { 200, {  3, 130}, {  7, 16 }, {  3,  16}, 0},     // protocol 8 Conrad RS-200 RX
  { 200, { 130, 7 }, {  16, 7 }, { 16,  3 }, 1},      // protocol 9 Conrad RS-200 TX
  { 365, { 18,  1 }, {  3,  1 }, {  1,  3 }, 1 },     // protocol 10 (1ByOne Doorbell)
  { 270, { 36,  1 }, {  1,  2 }, {  2,  1 }, 1 },     // protocol 11 (HT12E)
  { 320, { 36,  1 }, {  1,  2 }, {  2,  1 }, 1 },     // protocol 12 (SM5212)
  {  20, { 239, 78}, { 20, 35 }, { 35, 20 }, 0 },    // protocol 13 Dooya DC1600
};

enum {
   numRecvProto = sizeof(rf_recv_proto) / sizeof(rf_recv_proto[0])
};

typedef struct{
    uint8  Rn;
    uint32 PeriT1;
    uint32 OldPeriT1;
    uint32 PeriT2;
    uint32 OldPeriT2;
    uint16 Update;       //溢出次数
    uint32 PeriTa;      //低电平时间
    uint32 PeriTb;      //高电平时间
}RFInter;

RFInter xdata rfinter;

volatile uint32 xdata RFnReceivedValue;     //接收到的数据
volatile uint16 xdata RFnReceivedBitlength; //接收到的数据长度
volatile uint16  xdata RFnReceivedDelay;     //1T的值
volatile uint8 xdata  RFnReceivedProtocol;   //协议
volatile uint16   RFnReceiveTolerance = 80;     //高点电平允许的误差，这里为1T的百分之RFnReceiveTolerance
uint32 code RFnSeparationLimit = 4600;   //检测到同步头或者脉宽的最小阈值
uint32 xdata rf_timings[RCSWITCH_MAX_CHANGES];  //接收数据缓存

void RCSwitchInit();    //初始化
uint8 RFavailable();    //是否接收到数据
void RFresetAvailable();    //清空接收到的数据标志
uint32 RFgetReceivedValue();    //获取接收到的数据
uint16 RFgetReceivedBitlength();    //获取接收到的数据长度
uint16 RFgetReceivedDelay();    //获取1T的长度
uint8 RFgetReceivedProtocol();  //获取接收的协议类型

/************************************************************************
 * @brief  初始化
 * @param  none
 * @return none
 ***********************************************************************/
void RCSwitchInit()
{
    P_SW2 = 0x80;
    RFnReceivedValue = 0;
    BLUERF_RECV_PIN_OUT;

    // 配置PWMA引脚映射
    // C1PS[1:0] = bits[1:0], C2PS[1:0] = bits[3:2]
    #if RCSWITCH_CAPTURE_CH == 1
        PWMA_PS = (PWMA_PS & ~0x03) | (_RCSWITCH_PS_VAL & 0x03);
    #elif RCSWITCH_CAPTURE_CH == 2
        PWMA_PS = (PWMA_PS & ~0x0C) | ((_RCSWITCH_PS_VAL & 0x03) << 2);
    #endif

    PWMA_PSCRH = 0x00;
    PWMA_PSCRL = 0xEF;   //PWMA预分频（高字节先写入）

    PWMA_CCMR1 = _CC1S_VALUE;	//输入，IC1映射到对应TIxFP1上
    PWMA_CCER1 |= 0x00;	//设置捕获极性为OC1的上升沿

    PWMA_CCER1 = 0x01;	//使能OC1上的捕获功能

    PWMA_IER |= 0x02;	//使能CC1捕获中断	
    PWMA_IER |= 0x01; //使能更新中断

    PWMA_CR1 = 0;
    PWMA_CR1 |= 0x04;
    PWMA_CR1 |= 0x01;
    EA = 1;
}

/*************************************************************************
 * @fn      setReceiveTolerance
 * @brief   设置接收的容许公差(1T的百分之nPercent).
 * @param   interrupt - 中断号        
 * @return  none
 *************************************************************************/
void setReceiveTolerance(uint16 nPercent) 
{
    RFnReceiveTolerance = nPercent;
}

/*************************************************************************
 * @fn      RFavailable
 * @brief   是否接收到数据.
 * @param   none       
 * @return  1:接收到数据；0：没有接收到数据
 *************************************************************************/
uint8 RFavailable() 
{
    if(RFnReceivedValue != 0)
    {
        return 1;
    }
    return 0;
}

/*************************************************************************
 * @fn      RFresetAvailable
 * @brief   接收到数据标志清零.
 * @param   none       
 * @return  none
 *************************************************************************/
void RFresetAvailable() 
{
    RFnReceivedValue = 0;
}

/*************************************************************************
 * @fn      RFgetReceivedValue
 * @brief   获取接收到的数据.
 * @param   none       
 * @return  接收到的数据
 *************************************************************************/
uint32 RFgetReceivedValue() 
{
  return RFnReceivedValue;
}

/*************************************************************************
 * @fn      getReceivedBitlength
 * @brief   获取接收到的数据位数长度.
 * @param   none       
 * @return  数据长度
 *************************************************************************/
uint16 RFgetReceivedBitlength() 
{
  return RFnReceivedBitlength;
}

/*************************************************************************
 * @fn      getReceivedDelay
 * @brief   获取1T的长度
 * @param   none       
 * @return  none
 *************************************************************************/
uint16 RFgetReceivedDelay() 
{
  return RFnReceivedDelay;
}

/*************************************************************************
 * @fn      getReceivedProtocol
 * @brief   获取接收的协议类型
 * @param   none       
 * @return  none
 *************************************************************************/
uint8 RFgetReceivedProtocol() 
{
  return RFnReceivedProtocol;
}

/************************************************************************
 * @brief  数据处理函数
 * @param  none
 * @return none
 ***********************************************************************/
uint8 receiveProtocol(const int p, uint16 changeCount) 
{
	const RF_Protocol *pro = &rf_recv_proto[p-1];
	uint32 dat = 0;
	unsigned int i;

	//获取同步头中较长的脉宽
	const unsigned int syncLengthInPulses =  ((pro->syncFactor._low) > (pro->syncFactor._high)) ? (pro->syncFactor._low) : (pro->syncFactor._high);
	//得到同步头比值中一份比值的脉宽，即1T的值
	volatile unsigned int value_1T = rf_timings[0] / syncLengthInPulses;
  //设置容许误差，此处为delay*1
	volatile unsigned int delayTolerance = value_1T * RFnReceiveTolerance / 100;
	//如果pro.invertedSignal为真，则交换所有传输中_high和_low的逻辑。
    const unsigned int firstDataTiming = (pro->invertedSignal) ? (2) : (1);

	for (i = firstDataTiming; i < changeCount - 1; i += 2) {
		dat <<= 1;
		if (abs(rf_timings[i] - value_1T * pro->zero._high) < delayTolerance &&
			abs(rf_timings[i + 1] - value_1T * pro->zero._low) < delayTolerance){
			// zero

		} else if (abs(rf_timings[i]-value_1T * pro->one._high) < delayTolerance &&
					abs(rf_timings[i + 1]-value_1T * pro->one._low) < delayTolerance){
			// one
			dat |= 1;
		} else {
			// Failed
			return 0;
		}
	}

	if (changeCount > 7) {    // ignore very short transmissions: no device sends them, so this must be noise
		RFnReceivedValue = dat;
		RFnReceivedBitlength = (changeCount - 1) / 2;
		RFnReceivedDelay = value_1T;
		RFnReceivedProtocol = p;
		return 1;
	}
	return 0;
}

/************************************************************************
 * @brief  输入捕获中断回调函数
 * @param  none
 * @return none
 ***********************************************************************/
void PWMA_CAP_Callback()
{
    volatile uint32 duration;
  	static uint8 changeCount = 0;
	static uint8 repeatCount = 0;
    uint32 Rn01,Rn02; 
    rfinter.Rn++;   //上升沿或下降沿捕获标志
    if(rfinter.Rn == 1)  //上升沿
    {
      rfinter.OldPeriT1 = PWMA_CCR1H;
      rfinter.OldPeriT1 = (rfinter.OldPeriT1<<8)|PWMA_CCR1L;    //读捕获值
      Rn01 = rfinter.Update;     //溢出次数	
      rfinter.Update=0;        //清溢出次数
      Rn01 <<= 16;              
      rfinter.PeriT1 = rfinter.OldPeriT1 + Rn01;	
      rfinter.PeriTa = (rfinter.PeriT1 - rfinter.OldPeriT2) * 10;	//得出低电平时间
      duration = rfinter.PeriTa;
      PWMA_CCER1 &= ~0x01; //关闭OC1的输入捕获功能
      PWMA_CCER1 |= 0x02;	//设置捕获极性为OC1的下降沿
      PWMA_CCER1 |= 0x01;
    }
    else if(rfinter.Rn==2)     //下降沿
    { 
      rfinter.Rn = 0;	
      rfinter.OldPeriT2 = PWMA_CCR1H;
      rfinter.OldPeriT2 = (rfinter.OldPeriT2<<8)|PWMA_CCR1L;    //读捕获值 
      Rn02 = rfinter.Update;     //溢出次数 
      rfinter.Update = 0;        //清溢出次数
      Rn02<<=16;  	    
      rfinter.PeriT2 = rfinter.OldPeriT2 + Rn02;
      rfinter.PeriTb = (rfinter.PeriT2 - rfinter.OldPeriT1)*10;	//得出高电平时间 
      duration = rfinter.PeriTb;
      PWMA_CCER1 &= ~0x01; //关闭OC1的输入捕获功能
      PWMA_CCER1 &= ~0x02;	//设置捕获极性为OC1的上升沿
      PWMA_CCER1 |= 0x01;
    }	

    //长时间没有电平变化
    if((rfinter.PeriTb > RFnSeparationLimit)||(rfinter.PeriTa > RFnSeparationLimit))
    {
        rfinter.PeriTa = 0;
        rfinter.PeriTb = 0;
        if ((repeatCount==0) || (abs(duration-rf_timings[0]) < 350))
        {
            repeatCount++;
            if (repeatCount == 2) {
                /* 只按固定协议解码一次：中断里不再遍历全部协议，
                 * 保证本中断占用时间远小于串口 1 个字节的时间。 */
                receiveProtocol(RCSWITCH_FIXED_PROTOCOL, changeCount);
                repeatCount = 0;
            }
        }
        changeCount = 0;  			
    }
    //数据溢出
    if (changeCount >= RCSWITCH_MAX_CHANGES) 
    {
        changeCount = 0;
        repeatCount = 0;
    }
    rf_timings[changeCount++] = duration;
}

//输入捕获中断函数
void PWMA_ISR() interrupt 26
{
	if(PWMA_SR1 & _CC1IF_BIT)   //捕获1中断标记
	{
        PWMA_CAP_Callback();
        PWMA_SR1 &= ~_CC1IF_BIT;  //清标记
	}
    if(PWMA_SR1 & 0x01)    //定时器溢出标记
    {
        rfinter.Update++;
        PWMA_SR1 &= ~0x01;
    }
}

#endif