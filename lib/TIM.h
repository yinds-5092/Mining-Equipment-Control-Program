/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef _TIM_H_
#define _TIM_H_

//此枚举定义不允许用户修改
typedef enum    // 枚举ADC通道
{
	CTIM0_P34 = 0,
	CTIM1_P35,
	CTIM2_P12,
	CTIM3_P04,
	CTIM4_P06,
}CTIMN_name;


//此枚举定义不允许用户修改
typedef enum    // 枚举ADC通道
{
	TIM0,
	TIM1,
	TIM2,
	TIM3,
	TIM4,
}TIMN_name;


#define TIM2_CLEAR_FLAG		AUXINTIF &= ~0x01; 
#define TIM3_CLEAR_FLAG		AUXINTIF &= ~0x02; // 清中断标志
#define TIM4_CLEAR_FLAG		AUXINTIF &= ~0x04; // 清中断标志

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器初始化作为外部计数
//  @param      tim_n           选择模块
//  @return     void
//  @since      v1.0
//  Sample usage:               ctimer_count_init(CTIM0_P34);		//初始化定时器0，外部输入为P3.4引脚
//  @note                       串口1使用定时器1作为波特率发生器，
//								串口2使用定时器2作为波特率发生器，
//								串口3使用定时器3作为波特率发生器，
//								串口4使用定时器4作为波特率发生器，
//                              STC8G仅有定时器0-定时器4，这5个定时器。
//								编码器采集数据也需要定时器作为外部计数。
//-------------------------------------------------------------------------------------------------------------------
void timer_count_init(CTIMN_name tim_n)
{

	switch (tim_n)
	{
	case CTIM0_P34:
	{
		TL0 = 0;
		TH0 = 0;
		TMOD |= 0x04; //外部计数模式
		TR0 = 1; //启动定时器
		break;
	}

	case CTIM1_P35:
	{
		TL1 = 0x00;
		TH1 = 0x00;
		TMOD |= 0x40; // 外部计数模式
		TR1 = 1; // 启动定时器
		break;
	}

	case CTIM2_P12:
	{
		T2L = 0x00;
		T2H = 0x00;
		AUXR |= 0x18; // 设置外部计数模式并启动定时器
		break;
	}

	case CTIM3_P04:
	{
		T3L = 0;
		T3H = 0;
		T4T3M |= 0x0c; // 设置外部计数模式并启动定时器
		break;
	}

	case CTIM4_P06:
	{
		T4L = 0;
		T4H = 0;
		T4T3M |= 0xc0; // 设置外部计数模式并启动定时器
		break;
	}

	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取计数数值
//  @param      countch     计数通道号及引脚
//  @return     uint32      返回计数值
//  Sample usage:           num = ctimer_count_read(CTIM0_P34);     
//-------------------------------------------------------------------------------------------------------------------
uint16 timer_count_read(CTIMN_name tim_n)
{
	uint16 dat;

	switch (tim_n)
	{
	case CTIM0_P34:
	{
		dat = (uint8)TH0 << 8;
		dat = ((uint8)TL0) | dat;
		break;
	}
	case CTIM1_P35:
	{
		dat = (uint8)TH1 << 8;
		dat = ((uint8)TL1) | dat;
		break;
	}
	case CTIM2_P12:
	{
		dat = (uint8)T2H << 8;
		dat = ((uint8)T2L) | dat;
		break;
	}
	case CTIM3_P04:
	{
		dat = (uint8)T3H << 8;
		dat = ((uint8)T3L) | dat;
		break;
	}
	case CTIM4_P06:
	{
		dat = (uint8)T4H << 8;
		dat = ((uint8)T4L) | dat;
		break;
	}

	}


	return dat;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      清除计数数值
//  @param      countch     计数通道号及引脚
//  @return     void      
//  Sample usage:           ctimer_count_clean(CTIM0_P34);     
//-------------------------------------------------------------------------------------------------------------------
void timer_count_clean(CTIMN_name tim_n)
{
	switch (tim_n)
	{
	case CTIM0_P34:
	{
		TR0 = 0;
		TH0 = 0;
		TL0 = 0;
		TR0 = 1;
		break;
	}
	case CTIM1_P35:
	{
		TR1 = 0;
		TH1 = 0;
		TL1 = 0;
		TR1 = 1;
		break;
	}
	case CTIM2_P12:
	{
		AUXR &= ~(1 << 4);
		T2H = 0;
		T2L = 0;
		AUXR |= 1 << 4;
		break;
	}
	case CTIM3_P04:
	{
		T4T3M &= ~(1 << 3);
		T3H = 0;
		T3L = 0;
		T4T3M |= (1 << 3);
		break;
	}
	case CTIM4_P06:
	{
		T4T3M &= ~(1 << 7);
		T4H = 0;
		T4L = 0;
		T4T3M |= (1 << 7);
		break;
	}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器周期中断
//  @param      tim_n      定时器通道号
//  @param      time_ms    时间(ms)
//  @return     void      
//  Sample usage:          pit_timer_ms(TIM_0, 10)
//						   使用定时器0做周期中断，时间10ms一次。
//-------------------------------------------------------------------------------------------------------------------
void timer_us(TIMN_name tim_n, uint16 time_us)
{
	uint16 temp;
	temp = sys_clk / 12000;
	temp = ((float)temp / 1000) * time_us;

	temp = 65536 - temp;
	


	if (TIM0 == tim_n)
	{
		TMOD |= 0x00; 	// 模式 0
		TL0 = temp;
		TH0 = temp >> 8;
		TR0 = 1; 		// 启动定时器
		ET0 = 1; 		// 使能定时器中断
	}
	else if (TIM1 == tim_n)
	{
		TMOD |= 0x00; // 模式 0
		TL1 = temp;
		TH1 = temp >> 8;
		TR1 = 1; // 启动定时器
		ET1 = 1; // 使能定时器中断
	}
	else if (TIM2 == tim_n)
	{
		T2L = temp;
		T2H = temp >> 8;
		AUXR |= 0x10; // 启动定时器
		IE2 |= 0x04; // 使能定时器中断
	}
	else if (TIM3 == tim_n)
	{
		T3L = temp;
		T3H = temp >> 8;
		T4T3M |= 0x08; // 启动定时器
		IE2 |= 0x20; // 使能定时器中断
	}
	else if (TIM4 == tim_n)
	{
		T4L = temp;
		T4H = temp >> 8;
		T4T3M |= 0x80; // 启动定时器
		IE2 |= 0x40; // 使能定时器中断
	}
}


#endif