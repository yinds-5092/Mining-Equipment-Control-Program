/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef _PWM_H_
#define _PWM_H_

#include "GPIO.h"

#define PWMA_CCMR1_ADDR  0xfec8	//CCMR2_ADDR = CCMR1_ADDR + 1
#define PWMA_CCR1_ADDR   0xfed5 //CCR2_ADDR = CCR1_ADDR + 2
#define PWMA_CCER1_ADDR  0xfecc //CCER2_ADDR = CCER1_ADDR + 1

#define PWMB_CCMR1_ADDR  0xfee8 //CCMR2_ADDR = CCMR1_ADDR + 1
#define PWMB_CCR1_ADDR   0xfef5	//CCR2_ADDR = CCR1_ADDR + 2
#define	PWMB_CCER1_ADDR  0xfeec //CCER2_ADDR = CCER1_ADDR + 1

typedef enum
{
	PWM1P_P10 = 0x00, PWM1N_P11,
	PWM1P_P20, PWM1N_P21,
	PWM1P_P60, PWM1N_P61,

	PWM2P_P12 = 0x10, PWM2N_P13,
	PWM2P_P22, PWM2N_P23,
	PWM2P_P62, PWM2N_P63,

	PWM3P_P14 = 0x20, PWM3N_P15,
	PWM3P_P24, PWM3N_P25,
	PWM3P_P64, PWM3N_P65,

	PWM4P_P16 = 0x30, PWM4N_P17,
	PWM4P_P26, PWM4N_P27,
	PWM4P_P66, PWM4N_P67,
	PWM4P_P34, PWM4N_P33,

	PWM5_P20 = 0x40,
	PWM5_P17,
	PWM5_P00,
	PWM5_P74,

	PWM6_P21 = 0x50,
	PWM6_P54,
	PWM6_P01,
	PWM6_P75,

	PWM7_P22 = 0x60,
	PWM7_P33,
	PWM7_P02,
	PWM7_P76,

	PWM8_P23 = 0x70,
	PWM8_P34,
	PWM8_P03,
	PWM8_P77,

}PWM_CH;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM_gpio配置
//  @param      pwmch       PWM通道
//  @return     void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void pwm_set_gpio(PWM_CH pwmch)
{
	switch (pwmch)
	{
		case PWM1P_P10:
		{
			pinMode(P10, GPIO_PP);
			break;
		}
		case PWM1N_P11:
		{
			pinMode(P11, GPIO_PP);
			break;
		}
		case PWM1P_P20:
		{
			pinMode(P20, GPIO_PP);
			break;
		}
		case PWM1N_P21:
		{
			pinMode(P21, GPIO_PP);
			break;
		}
		case PWM1P_P60:
		{
			pinMode(P60, GPIO_PP);
			break;
		}
		case PWM1N_P61:
		{
			pinMode(P61, GPIO_PP);
			break;
		}

		case PWM2P_P12:
		{
			pinMode(P12, GPIO_PP);
			break;
		}
		case PWM2N_P13:
		{
			pinMode(P13, GPIO_PP);
			break;
		}
		case PWM2P_P22:
		{
			pinMode(P22, GPIO_PP);
			break;
		}
		case PWM2N_P23:
		{
			pinMode(P23, GPIO_PP);
			break;
		}
		case PWM2P_P62:
		{
			pinMode(P62, GPIO_PP);
			break;
		}
		case PWM2N_P63:
		{
			pinMode(P63, GPIO_PP);
			break;
		}

		case PWM3P_P14:
		{
			pinMode(P14, GPIO_PP);
			break;
		}
		case PWM3N_P15:
		{
			pinMode(P15, GPIO_PP);
			break;
		}
		case PWM3P_P24:
		{
			pinMode(P24, GPIO_PP);
			break;
		}
		case PWM3N_P25:
		{
			pinMode(P25, GPIO_PP);
			break;
		}
		case PWM3P_P64:
		{
			pinMode(P64, GPIO_PP);
			break;
		}
		case PWM3N_P65:
		{
			pinMode(P65, GPIO_PP);
			break;
		}
		case PWM4P_P16:
		{
			pinMode(P16, GPIO_PP);
			break;
		}
		case PWM4N_P17:
		{
			pinMode(P17, GPIO_PP);
			break;
		}
		case PWM4P_P26:
		{
			pinMode(P26, GPIO_PP);
			break;
		}
		case PWM4N_P27:
		{
			pinMode(P27, GPIO_PP);
			break;
		}
		case PWM4P_P66:
		{
			pinMode(P66, GPIO_PP);
			break;
		}
		case PWM4N_P67:
		{
			pinMode(P67, GPIO_PP);
			break;
		}
		case PWM4P_P34:
		{
			pinMode(P34, GPIO_PP);
			break;
		}
		case PWM4N_P33:
		{
			pinMode(P33, GPIO_PP);
			break;
		}


		case PWM5_P20:
		{
			pinMode(P20, GPIO_PP);
			break;
		}
		case PWM5_P17:
		{
			pinMode(P17, GPIO_PP);
			break;
		}
		case PWM5_P00:
		{
			pinMode(P00, GPIO_PP);
			break;
		}
		case PWM5_P74:
		{
			pinMode(P74, GPIO_PP);
			break;
		}
		case PWM6_P21:
		{
			pinMode(P21, GPIO_PP);
			break;
		}
		case PWM6_P54:
		{
			pinMode(P54, GPIO_PP);
			break;
		}
		case PWM6_P01:
		{
			pinMode(P01, GPIO_PP);
			break;
		}
		case PWM6_P75:
		{
			pinMode(P75, GPIO_PP);
			break;
		}
		case PWM7_P22:
		{
			pinMode(P22, GPIO_PP);
			break;
		}
		case PWM7_P33:
		{
			pinMode(P33, GPIO_PP);
			break;
		}
		case PWM7_P02:
		{
			pinMode(P02, GPIO_PP);
			break;
		}
		case PWM7_P76:
		{
			pinMode(P76, GPIO_PP);
			break;
		}
		case PWM8_P23:
		{
			pinMode(P23, GPIO_PP);
			break;
		}
		case PWM8_P34:
		{
			pinMode(P34, GPIO_PP);
			break;
		}
		case PWM8_P03:
		{
			pinMode(P03, GPIO_PP);
			break;
		}
		case PWM8_P77:
		{
			pinMode(P77, GPIO_PP);
			break;
		}
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM初始化
//  @param      pwmch       PWM通道
//  @param      freq        PWM频率64Hz-3MHz)
//  @param      duty        PWM占空比
//  @return     void
//  Sample usage:           
//							pwm_init(PWM0_P00, 100, 5000);     //通道PWM0  引脚P0.0  频率100  占空比5000
//							PWM_DUTY_MAX为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_init(PWM_CH pwmch, uint32 freq, uint16 duty)
{

	uint16 match_temp;
	uint16 period_temp;
	uint16 freq_div = 0;

	P_SW2 |= 0x80;

	//GPIO端口配置
	pwm_set_gpio(pwmch);

	freq_div = ((uint32)(sys_clk / freq)) >> 16;				//分频

	period_temp = (sys_clk / freq) / (freq_div + 1) - 1;		//周期时间

	match_temp = period_temp * ((float)duty / PWM_DUTY_MAX);	//占空比

	if (PWM5_P20 <= pwmch)				//PWM5-8
	{

		PWMB_ENO |= (1 << ((2 * ((pwmch >> 4) - 4))));					//使能输出
		PWMB_PS |= ((pwmch & 0x03) << ((2 * ((pwmch >> 4) - 4))));		//输出脚选择

		// 配置通道输出使能和极性	
		(*(unsigned char volatile xdata*)(PWMB_CCER1_ADDR + (((pwmch >> 4) - 4) >> 1))) |= (1 << (((pwmch >> 4) & 0x01) * 4));

		//通道模式配置
		(*(unsigned char volatile xdata*)(PWMB_CCMR1_ADDR + ((pwmch >> 4) - 4))) |= 0x06 << 4;	//
		(*(unsigned char volatile xdata*)(PWMB_CCMR1_ADDR + ((pwmch >> 4) - 4))) |= 1 << 3;		//PWM模式2

		//设置周期时间（高字节先写入）
		//PWM2_ARR = (uint16)period_temp;
		PWMB_ARRH = period_temp >> 8;
		PWMB_ARRL = period_temp;
		//PWM2_ARR=2000;

		//PWM预分频（高字节先写入）
		PWMB_PSCRH = freq_div >> 8;
		PWMB_PSCRL = freq_div;

		//设置占空比（高字节先写入）
		(*(unsigned char volatile xdata*)(PWMB_CCR1_ADDR + 2 * ((pwmch >> 4) - 4))) = match_temp >> 8;
		(*(unsigned char volatile xdata*)(PWMB_CCR1_ADDR + 2 * ((pwmch >> 4) - 4) + 1)) = match_temp;
		
		PWMB_BKR = 0x80; 	//使能主输出
		PWMB_CR1 = 0x01;	//PWM开始计时
	}
	else
	{
		PWMA_ENO |= (1 << (pwmch & 0x01)) << ((pwmch >> 4) * 2);	//使能输出	
		PWMA_PS |= ((pwmch & 0x07) >> 1) << ((pwmch >> 4) * 2);    //输出脚选择


		// 配置通道输出使能和极性
		(*(unsigned char volatile xdata*)(PWMA_CCER1_ADDR + (pwmch >> 5))) |= (1 << ((pwmch & 0x01) * 2 + ((pwmch >> 4) & 0x01) * 0x04));


		(*(unsigned char volatile xdata*)(PWMA_CCMR1_ADDR + (pwmch >> 4))) |= 0x06 << 4;	
		(*(unsigned char volatile xdata*)(PWMA_CCMR1_ADDR + (pwmch >> 4))) |= 1 << 3;		//PWM模式2

		//设置周期时间（高字节先写入）
		//PWM1_ARR = period_temp;
		PWMA_ARRH = period_temp >> 8;
		PWMA_ARRL = period_temp;

		//PWM预分频（高字节先写入）
		PWMA_PSCRH = freq_div >> 8;
		PWMA_PSCRL = freq_div;

		//设置占空比（高字节先写入）
		(*(unsigned char volatile xdata*)(PWMA_CCR1_ADDR + 2 * (pwmch >> 4))) = match_temp >> 8;
		(*(unsigned char volatile xdata*)(PWMA_CCR1_ADDR + 2 * (pwmch >> 4) + 1)) = match_temp;

		PWMA_BKR = 0x80; 	//使能主输出
		PWMA_CR1 = 0x01;	//PWM开始计时
	}

	//P_SW2 &= 0x7F;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM调整占空比
//  @param      pwmch       PWM引脚
//  @param      duty        PWM占空比
//  @return     void
//  Sample usage:           pwm_duty(PWM0_P00, 5000);     //通道PWM0  引脚P0.0  占空比5000
//							
//-------------------------------------------------------------------------------------------------------------------
void pwm_duty(PWM_CH pwmch, uint16 duty)
{
	uint16 match_temp;

	//	P_SW2 |= 0x80;
	if (PWM5_P20 <= pwmch)				//PWM5-8
	{
		match_temp = PWMB_ARRH;
		match_temp = match_temp << 8;
		match_temp = match_temp + PWMB_ARRL;
		match_temp = match_temp * ((float)duty / PWM_DUTY_MAX);				//占空比
		(*(unsigned char volatile xdata*)(PWMB_CCR1_ADDR + 2 * ((pwmch >> 4) - 4))) = match_temp >> 8;
		(*(unsigned char volatile xdata*)(PWMB_CCR1_ADDR + 2 * ((pwmch >> 4) - 4) + 1)) = match_temp;
	}
	else
	{
		match_temp = PWMA_ARRH;
		match_temp = match_temp << 8;
		match_temp = match_temp + PWMA_ARRL;
		match_temp = match_temp *((float)duty / PWM_DUTY_MAX);				//占空比
		(*(unsigned char volatile xdata*)(PWMA_CCR1_ADDR + 2 * (pwmch >> 4))) = match_temp >> 8;
		(*(unsigned char volatile xdata*)(PWMA_CCR1_ADDR + 2 * (pwmch >> 4) + 1)) = match_temp;
	}
	//	P_SW2 &= ~0x80;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM调整频率和占空比
//  @param      pwmch       PWM通道
//  @param      freq        PWM频率
//  @param      duty        PWM占空比
//  @return     void
//  Sample usage:           pwm_freq(PWM0_P00, 50, 5000);     //通道PWM0  引脚P0.0  频率50  占空比5000
//-------------------------------------------------------------------------------------------------------------------
void pwm_freq_duty(PWM_CH pwmch, uint16 freq, uint16 duty)
{
	uint16 match_temp;
	uint16 period_temp;
	uint16 freq_div = 0;

	freq_div = (sys_clk / freq) >> 15;								//分频
	period_temp = sys_clk / freq / (freq_div + 1);					//频率

	//match_temp = period_temp * duty;
	//match_temp = match_temp / PWM_DUTY_MAX;
	match_temp = period_temp * ((float)duty / PWM_DUTY_MAX);	//占空比

//	P_SW2 |= 0x80;
	if (PWM5_P20 <= pwmch)				//PWM5-8
	{
		//周期（高字节先写入）
		//PWM2_ARR = (uint16)period_temp;
		PWMB_ARRH = period_temp >> 8;
		PWMB_ARRL = period_temp;
		//PWM预分频（高字节先写入）
		PWMB_PSCRH = freq_div>>8;
		PWMB_PSCRL = freq_div;
		//占空比（高字节先写入）
		//(*(unsigned int volatile xdata*)(PWM2_CCR1_ADDR + 2 * ((pwmch >> 4) - 4))) = match_temp;
		(*(unsigned char volatile xdata*)(PWMB_CCR1_ADDR + 2 * ((pwmch >> 4) - 4))) = match_temp >> 8;
		(*(unsigned char volatile xdata*)(PWMB_CCR1_ADDR + 2 * ((pwmch >> 4) - 4) + 1)) = match_temp;
		
	}
	else
	{
		//周期（高字节先写入）
		//PWM2_ARR = (uint16)period_temp;
		PWMA_ARRH = period_temp >> 8;
		PWMA_ARRL = period_temp;
		//PWM预分频（高字节先写入）
		PWMA_PSCR = freq_div;
		//占空比（高字节先写入）
		//(*(unsigned int volatile xdata*)(PWM1_CCR1_ADDR + 2 * (pwmch >> 4))) = match_temp;
		(*(unsigned char volatile xdata*)(PWMA_CCR1_ADDR + 2 * (pwmch >> 4))) = match_temp >> 8;
		(*(unsigned char volatile xdata*)(PWMA_CCR1_ADDR + 2 * (pwmch >> 4) + 1)) = match_temp;
		
	}
	//	P_SW2 &= ~0x80;
}
#endif