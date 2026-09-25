/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef __NTC_H
#define __NTC_H

#include <STC8HX.h>
#include "delay.h"
#include "ADC.h"

#define NTC_VREF 3.3//431基准电压实际测试值

#ifndef  NTC_ADC_PIN
#define  NTC_ADC_PIN    ADC_P04
#endif 

void ntc_init();        //ntc初始化
float ntc_read_temp();  //获取ntc温度

//ntc 型号 QN0603X103J3380HB
//数据放大1000倍方便整数计算
code uint16 ntc[51]=
{
42889,//-10
40967,
39142,
37408,
35761,
34196,
32707,
31291,
29945,
28664,
27445,//0
26283,
25177,
24124,
23121,
22165,
21253,
20384,
19555,
18764,
18010,
17290,
16602,
15946,
15319,
14720,
14148,
13601,
13078,
12578,
12099,
11642,
11204,
10785,
10384,
10000,
9632, 
9280, 
8943, 
8619, 
8309, 
8012, 
7727, 
7453, 
7191, 
6939, 
6698, 
6466, 
6243, 
6029, 
5824 //40
};
//========================================================================
// 描述: ntc初始化.
// 参数: none.
// 返回: none.
//========================================================================
void ntc_init()
{
    adc_init(NTC_ADC_PIN, ADC_SYSclk_DIV_2, ADC_12BIT);//ADC初始化，三个参数ADC引脚，时钟分频双数2-32，输出值位数12BIT最大分率-12位的ADC输出12位，10位的输出10位
}

//========================================================================
// 描述: ntc从电阻分度表格里查找最靠近的位置.
// 参数: 电阻值*1000.
// 返回: 分度表格里最靠近的位置.
//========================================================================
uint8 ntc_find_r(uint16 data)
{
    uint8  start_index, mid = 0, end_index;
    uint16  dat;
    
    start_index = 0;
    end_index = 50;
      
    // 2. 开始二分法查找
    while (start_index < end_index)
    {
        mid = (end_index + start_index) >> 1;
        dat = ntc[mid];
        
        if (data < dat)
        {
            start_index = mid + 1;
        }
        else if (data > dat)
        {
            if (mid)
            {
                end_index = mid;
            }
            else
            {
                break;
            }
        }
        else
        {
            return (mid);
        }
    }
    
    return (mid);
}

//========================================================================
// 描述: 根据查表结果计算温度.
// 参数: 电阻值*1000.
// 返回: 温度.
//========================================================================
float ntc_get_value(uint16 data)
{
  uint8 index;
  float temp;
  data = (data > 42889) ? 42889 : data;
  data = (data < 5824) ? 5824 : data;
  
  index = ntc_find_r(data);

  if(ntc[index]>data)
  {
    temp = index + (float)(ntc[index]-data)/(ntc[index]-ntc[index+1]);//线性插值
    
    return temp-9;//偏移
  }
  else if(ntc[index]<data)
  {
    temp =  index-(float)(data-ntc[index])/(ntc[index-1]-ntc[index]);//线性插值
    return temp-11;//偏移
  }
  else
  {
    return index-10;
  }
}

//========================================================================
// 描述: 获取ntc温度.
// 参数: None.
// 返回: 温度.
//========================================================================
float ntc_read_temp()
{
    uint16 adc,sum=0;
    uint8 i;
    float V,R,T;
    for(i=0;i<10;i++)
    {
        sum+=adc_read(NTC_ADC_PIN);
    }
    adc = sum/10;//采样十次取平均值
    V=adc*NTC_VREF/4096;
    R=V*10/(NTC_VREF-V);
    T = ntc_get_value(R*1000);
    T = (int)(10.0 * T + 0.5) / 10.0;//保留一位小数
    return T*1.0;
}

#endif
