/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef _ADC_H_
#define _ADC_H_

uint8  setbit = 0;
typedef enum
{
	ADC_P10 = 0,
	ADC_P11,
	ADC_P12, //仅做占位
	ADC_P13,
	ADC_P14,
	ADC_P15,
	ADC_P16,
	ADC_P17,

	ADC_P00,
	ADC_P01,
	ADC_P02,
	ADC_P03,
	ADC_P04,
	ADC_P05,
	ADC_P06,
	ADC_REF = 0x0f, //内部AD 1.19V

	ADC_P30,
	ADC_P31,
	ADC_P32,
	ADC_P33,
	ADC_P34,
	ADC_P35,
	ADC_P36,
	ADC_P37,

} ADC_Name;

//此枚举定义不允许用户修改
typedef enum
{
	ADC_SYSclk_DIV_2 = 0,
	ADC_SYSclk_DIV_4,
	ADC_SYSclk_DIV_6,
	ADC_SYSclk_DIV_8,
	ADC_SYSclk_DIV_10,
	ADC_SYSclk_DIV_12,
	ADC_SYSclk_DIV_14,
	ADC_SYSclk_DIV_16,
	ADC_SYSclk_DIV_18,
	ADC_SYSclk_DIV_20,
	ADC_SYSclk_DIV_22,
	ADC_SYSclk_DIV_24,
	ADC_SYSclk_DIV_26,
	ADC_SYSclk_DIV_28,
	ADC_SYSclk_DIV_30,
	ADC_SYSclk_DIV_32,
} ADC_CLK;


//此枚举定义不允许用户修改
typedef enum    // 枚举ADC通道
{

	ADC_12BIT = 0,    //12位分辨率
	ADC_11BIT,		//11位分辨率
	ADC_10BIT,		//10位分辨率
	ADC_9BIT,    	//9位分辨率
	ADC_8BIT,     	//8位分辨率

}ADC_bit;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC初始化
//  @param      adcn            选择ADC通道
//  @param      speed      		ADC时钟频率
//  @return     void
//  Sample usage:               adc_init(ADC_P10,ADC_SYSclk_DIV_2);//初始化P1.0为ADC功能,ADC时钟频率：SYSclk/2
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADC_Name adcn, ADC_CLK speed, ADC_bit _sbit)
{
	setbit = _sbit;
	ADC_CONTR |= 1 << 7;	//1 ：打开 ADC 电源
	if (adcn > 15) 
	{
		adcn = adcn - 16;
		//IO口需要设置为高阻输入
		P3M0 &= ~(1 << (adcn & 0x07));
		P3M1 |= (1 << (adcn & 0x07));


	}
	else {
		if ((adcn >> 3) == 1) //P0.0
		{
			//IO口需要设置为高阻输入
			P0M0 &= ~(1 << (adcn & 0x07));
			P0M1 |= (1 << (adcn & 0x07));
		}
		else if ((adcn >> 3) == 0) //P1.0	
		{
			//IO口需要设置为高阻输入
			P1M0 &= ~(1 << (adcn & 0x07));
			P1M1 |= (1 << (adcn & 0x07));
		}
	}

	ADCCFG |= speed & 0x0F;	//ADC时钟频率SYSclk/2/speed&0x0F;

	ADCCFG |= 1 << 5;		//转换结果右对齐。 ADC_RES 保存结果的高 2 位， ADC_RESL 保存结果的低 8 位。

}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC转换一次
//  @param      adcn            选择ADC通道
//  @param      resolution      分辨率
//  @return     void
//  Sample usage:               adc_convert(ADC_P10, ADC_10BIT);
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_read(ADC_Name adcn)
{
	uint16 adc_value;
	if (adcn > 15)adcn = adcn - 8;

	ADC_CONTR &= (0xF0);	//清除ADC_CHS[3:0] ： ADC 模拟通道选择位
	ADC_CONTR |= adcn;

	ADC_CONTR |= 0x40;  // 启动 AD 转换
	while (!(ADC_CONTR & 0x20));  // 查询 ADC 完成标志
	ADC_CONTR &= ~0x20;  // 清完成标志


	adc_value = ADC_RES;  //存储 ADC 的 10 位结果的高 2 位
	adc_value <<= 8;
	adc_value |= ADC_RESL;  //存储 ADC 的 10 位结果的低 8 位

	ADC_RES = 0;
	ADC_RESL = 0;

	adc_value >>= setbit;//取多少位


	return adc_value;
}

#endif