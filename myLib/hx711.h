/*************  技术支持与购买说明    **************
产品主页：http://tw51.haohaodada.com
淘宝搜索：天问51，可购买。目前基础版99元，带彩屏标准备版149元，旗舰版299
技术支持QQ群一：1138055784
******************************************/

#ifndef __HX711_H
#define __HX711_H

#ifndef     HX711_SCK_PIN
#define     HX711_SCK_PIN       P1_0
#endif

#ifndef     HX711_SCK_MODE
#define     HX711_SCK_MODE      {P1M1&=~0x01;P1M0&=~0x01;}    //双向IO口
#endif

#ifndef     HX711_DT_PIN
#define     HX711_DT_PIN        P1_1
#endif

#ifndef     HX711_DT_MODE
#define     HX711_DT_MODE       {P1M1&=~0x02;P1M0&=~0x02;}    //双向IO口
#endif

uint32 _hx711_gross_weight; //毛重全局变量

void hx711_init();  //初始化
uint32 hx711_read_count();  //读取hx711传感器数据
void hx711_get_gross_weight();  //获取毛重（传感器值）
// uint32 hx711_get_net_weight_original(); //获取净重（传感器值）
float hx711_get_net_weight(uint16 ratio);   //获取净重

//========================================================================
// 描述: HX711初始化.
// 参数: none.
// 返回: none.
//========================================================================
void hx711_init()
{
#ifndef _C51
    HX711_SCK_MODE;
    HX711_DT_MODE;
#endif
    HX711_SCK_PIN = 1;
    HX711_DT_PIN = 1;
}

//========================================================================
// 描述: HX711读取原始数据.
// 参数: none.
// 返回: none.
//========================================================================
uint32 hx711_read_count()
{
	uint32 count = 0;
	uint8 i=0;

	HX711_SCK_PIN = 0;					//ADSK=0; 使能AD（PD_SCK 置低）
	while(HX711_DT_PIN); 				//AD转换未结束则等待，否则开始读取
	for (i=0;i<24;i++)
	{
		HX711_SCK_PIN = 1;				// ADSK=1; PD_SCK 置高（发送脉冲）
        count=count<<1;					//下降沿来时变量Count左移一位，右侧补零
        HX711_SCK_PIN = 0;				//ADSK=0; PD_SCK 置低
        if(HX711_DT_PIN)count++;
	}
	HX711_SCK_PIN = 1;					// ADSK=1;
	count=count^0x800000;				//第25个脉冲下降沿来时，转换数据
	HX711_SCK_PIN = 0;					//ADSK=0;
	return(count);
}

//========================================================================
// 描述: HX711获取毛重.
// 参数: none.
// 返回: none.
//========================================================================
void hx711_get_gross_weight()
{
    uint8 i;
    for(i=0;i<5;i++)
    {
        _hx711_gross_weight += hx711_read_count();
    }
    _hx711_gross_weight /= 5; 
}

//========================================================================
// 描述: HX711获取净重(原始值，内部调用).
// 参数: none.
// 返回: none.
//========================================================================
uint32 hx711_get_net_weight_original()
{
    return (hx711_read_count() - _hx711_gross_weight);
}

//========================================================================
// 描述: HX711获取净重.
// 参数: ratio:比例系数（需用户标定）.
// 返回: 净重.
//========================================================================
float hx711_get_net_weight(uint16 ratio)
{
    return (float)hx711_get_net_weight_original()/(float)ratio;
}

#endif  //HX711.h