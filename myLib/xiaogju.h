#ifndef _XIAOGJU_H_
#define _XIAOGJU_H_
#if  defined(gj_dadc)||defined(gj_djd)||defined(gj_dgd)

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

} ADP_Name;

//此枚举定义不允许用户修改
typedef enum
{
	ADP_SYSclk_DIV_2 = 0,
	ADP_SYSclk_DIV_4,
	ADP_SYSclk_DIV_6,
	ADP_SYSclk_DIV_8,
	ADP_SYSclk_DIV_10,
	ADP_SYSclk_DIV_12,
	ADP_SYSclk_DIV_14,
	ADP_SYSclk_DIV_16,
	ADP_SYSclk_DIV_18,
	ADP_SYSclk_DIV_20,
	ADP_SYSclk_DIV_22,
	ADP_SYSclk_DIV_24,
	ADP_SYSclk_DIV_26,
	ADP_SYSclk_DIV_28,
	ADP_SYSclk_DIV_30,
	ADP_SYSclk_DIV_32,
} ADP_CLK;


//此枚举定义不允许用户修改
typedef enum    // 枚举ADP通道
{

	ADP_12BIT = 0,    //12位分辨率
	ADP_11BIT,		//11位分辨率
	ADP_10BIT,		//10位分辨率
	ADP_9BIT,    	//9位分辨率
	ADP_8BIT,     	//8位分辨率

}ADP_bit;
#endif
#ifdef gj_dd
void DIAODIAN_HUANXIN(uint16  WKTC);//定时掉电唤醒取值范围1~16000MS.
#endif
#ifdef gj_dgd
float gongdian(uint8 ADW);//功能：根据内部参考电压计算ADC外部参考电压，
#endif
#ifdef gj_djd
float ADCdian(ADP_Name ADPn);//根据内部参考电压计算ADC电压
#endif
#ifdef gj_px
void insertionsort(unsigned char *arr,unsigned int len); //功能：数组插值排序
#endif
#ifdef gj_xs
uint16 uintwei_xie(uint16 shu,unsigned char wei,bit weiC);//功能：二进制数位写入
#endif
#ifdef gj_foca
char* Float_Char( float value, uint8 *array, uint8 decimalPlaces,uint8 line_break); //功能：数字转字符串数组
#endif
#if  defined(gj_dadc)||defined(gj_djd)||defined(gj_dgd)
void ADP_init(ADP_Name ADPn, ADP_CLK speed, ADP_bit _sbit); // ADP初始化
uint16 ADP_read(ADP_Name ADPn,uint8 wei); //ADP转换取均值一次
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADP初始化
//  @param      ADPn            选择ADP通道
//  @param      speed      		ADP时钟频率
//  @return     void
//  Sample usage:               ADP_init(ADP_P10,ADP_SYSclk_DIV_2);//初始化P1.0为ADP功能,ADP时钟频率：SYSclk/2
//-------------------------------------------------------------------------------------------------------------------
void ADP_init(ADP_Name ADPn, ADP_CLK speed, ADP_bit _sbit)
{
	setbit = _sbit;
	ADC_CONTR |= 1 << 7;	//1 ：打开 ADP 电源
	if (ADPn > 15) 
	{
		ADPn = ADPn - 16;
		//IO口需要设置为高阻输入
		P3M0 &= ~(1 << (ADPn & 0x07));
		P3M1 |= (1 << (ADPn & 0x07));


	}
	else {
		if ((ADPn >> 3) == 1) //P0.0
		{
			//IO口需要设置为高阻输入
			P0M0 &= ~(1 << (ADPn & 0x07));
			P0M1 |= (1 << (ADPn & 0x07));
		}
		else if ((ADPn >> 3) == 0) //P1.0	
		{
			//IO口需要设置为高阻输入
			P1M0 &= ~(1 << (ADPn & 0x07));
			P1M1 |= (1 << (ADPn & 0x07));
		}
	}

	ADCCFG |= speed & 0x0F;	//ADC时钟频率SYSclk/2/speed&0x0F;

	ADCCFG |= 1 << 5;		//转换结果右对齐。 ADC_RES 保存结果的高 2 位， ADP_RESL 保存结果的低 8 位。

}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADP转换取均值一次
//  @param      ADPn            选择ADP通道
//  @param      resolution      分辨率
//  @return     void
//  Sample usage:               ADP_convert(ADP_P10, ADP_10BIT);
//-------------------------------------------------------------------------------------------------------------------
uint16 ADP_read(ADP_Name ADPn,uint8 wei)
{
	uint8 y,i,u;
    uint16 shuzu[10];
	uint16 yaogan_max,yaogan_min,yaogan_sum;
	//uint32 ;
	uint16 ADP_value;
	y=wei;
	if (ADPn > 15)ADPn = ADPn - 8;
	for(i=0;i<y;i++)
	{
		ADC_CONTR &= (0xF0);	//清除ADP_CHS[3:0] ： ADP 模拟通道选择位
    	ADC_CONTR |= ADPn;
		ADC_CONTR |= 0x40;  // 启动 AD 转换
		while (!(ADC_CONTR & 0x20));  // 查询 ADP 完成标志
		ADC_CONTR &= ~0x20;  // 清完成标志


		ADP_value = ADC_RES;  //存储 ADP 的 10 位结果的高 2 位
		ADP_value <<= 8;
		ADP_value |= ADC_RESL;  //存储 ADP 的 10 位结果的低 8 位

		ADC_RES = 0;
		ADC_RESL = 0;

		ADP_value >>= setbit;//取多少位
		shuzu[i] = ADP_value;

	}
	for(u=0;u<2;u++)
	{
        yaogan_min = shuzu[0];
		yaogan_max = shuzu[0];
		for(i=0;i<y;i++)
		{
			
			if(yaogan_max < shuzu[i])
			{
			   yaogan_max=shuzu[i];
			}
			if(yaogan_min >= shuzu[i])
			{
				//return i;
			//	return shuzu[0];
				yaogan_min = shuzu[i];
			//	return shuzu[0];
			}
		}
		
		for(i=0;i<y;i++)
		{
			
			if(yaogan_max==shuzu[i])
			{
			   shuzu[i] =shuzu[y-1] ;
			   break;
			}   
		}
		for(i=0;i<y;i++)
		{
			if(yaogan_min==shuzu[i] )
			{
			   shuzu[i] =shuzu[y-2] ;
			   break;
			    
			}   
		}
		y=y-2;
	}
    yaogan_sum=0;
	for(i=0;i<y;i++)
	{
	   yaogan_sum=yaogan_sum+shuzu[i];
	//   if(i==4)
	 //  return 1025;

	}
	yaogan_sum = yaogan_sum/y;
	return yaogan_sum;
}

#endif
#ifdef gj_djd
//*******************************************************************/
//功能：读ADC电压值
//形参：无
//返回：电压值
//详解：根据内部参考电压计算ADC电压，
//*******************************************************************/
float ADCdian(ADP_Name ADPn)//根据内部参考电压计算ADC电压，
{
	uint16 adjg,adck;
	adjg = ADP_read(ADPn,6);
	adck = ADP_read(ADC_REF,6);
    return 1.19/adck*adjg;
} 
#endif
#ifdef gj_dgd
//*******************************************************************/
//功能：读供电电压(ADC外部参考电压)
//形参：1:adc脚位。2：ADC的实际读取位数。
//返回：电压值
//详解：根据内部参考电压计算ADC外部参考电压，
//*******************************************************************/
float gongdian(uint8 ADW)//功能：根据内部参考电压计算ADC外部参考电压，
{
	uint16 adjg;
	ADP_init(ADC_REF, ADP_SYSclk_DIV_2, ADP_12BIT);
	adjg = ADP_read(ADC_REF,6);
	switch (ADW) {
   case 12:
   return 4096*1.19/adjg;
    break;
   default:
   return 1024*1.19/adjg;
    break;
  }
	
}
#endif

#ifdef gj_xs
//*******************************************************************/
//功能：二进制数位写入
//形参：shu:待操作的数。wei：要写入第几位（从右到左是0——7位）weiC:为0时写入0. 为1时写入1
//返回：无
//详解：
//*******************************************************************/
uint16 uintwei_xie(uint16 shu,unsigned char wei,bit weiC)//功能：二进制数位写入
{
	if(weiC == 0)
	{
		return shu & (~(0x0001<<wei));
	}
	else
	{
		return shu | (0x0001<<wei);
	}
}
#endif
#ifdef gj_dd
//*******************************************************************/
//功能：定时掉电唤醒
//形参：设定唤醒时间取值范围1~16000MS.
//返回：无
//详解：
//*******************************************************************/
void DIAODIAN_HUANXIN(uint16  WKTC)//取值范围1~16000MS.
{
	WKTC = WKTC*2;
	WKTCL = WKTC;
	WKTC >>= 8;
	WKTCH = WKTC;
	_nop_();
	PCON = 0x02;
	_nop_();
}
#endif
#ifdef gj_px
//*******************************************************************/
//功能：数组插值排序
//形参：*arr；待排序数组的指针，len；排序的数的长度
//返回：无
//详解：
//*******************************************************************/
void insertionsort(unsigned char *arr,unsigned int len)//功能：数组插值排序
{
	unsigned int i;
	for(i=1; i<len; i++ )
	{

		signed int key1 = arr[i];
		unsigned int j = i - 1;
		while(key1<arr[j])
		{
			arr[j+1] = arr[j];
			j--;
		}
		if(j != i-1)
		{
			arr[j+1] = key1;
		}
		
		
	}
}
#endif


#ifdef gj_foca

//*******************************************************************/
//功能：数字转字符串数组
//形参：value；待转数值。*array；接收数组指针。decimalPlaces；小数保留精度。
//返回：无
//详解：
//*******************************************************************/
char* Float_Char( float value, uint8 *array, uint8 decimalPlaces,uint8 line_break) //功能：数字转字符串数组
{
    uint32 IntegerPart;
    float DecimalPart;
    uint8 length = 0;
	uint8 temp,start,end,k;

    // 处理负数
    if (value < 0) {
        array[length++] = '-';
        value = (-value); // 将负数转换为正数以处理
    }

    IntegerPart = (uint32)value;
    DecimalPart = value - IntegerPart;

    // 处理整数部分
    if (IntegerPart == 0) { // 整数部分为0
        array[length++] = '0';
    } else {
        // 逆序存储整数部分到数组
      
        start = length;
		do {
        array[length++] = '0' + IntegerPart % 10;
        IntegerPart /= 10;
    } while (IntegerPart != 0);
        // 逆序整数部分数组，使其顺序正确
        end = length - 1;
        while (start < end) {
            temp = array[start];
            array[start++] = array[end];
            array[end--] = temp;
        }
    }

    // 处理小数部分
    if (decimalPlaces > 0) {
        array[length++] = '.'; // 添加小数点
        // 处理小数部分，将小数部分转换为字符串
        for (k = 0; k < decimalPlaces; k++) {
            DecimalPart *= 10;
            array[length++] = (uint32)DecimalPart % 10 + '0';
        }
    }
	if(line_break)
	{
		array[length++] = 0x0d;
		array[length++] = 0x0a;
	}

    array[length] = '\0'; // 添加空字符以结束字符串

    return (char*)array;
}
#endif
#endif