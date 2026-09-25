/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef __LED8_H
#define __LED8_H

#include <STC8HX.h>

#ifndef LED8_CONTROL_PIN
#define LED8_CONTROL_PIN        P4_0
#endif

#ifndef LED8_CONTROL_MODE
#define LED8_CONTROL_MODE     {P4M1&=~0x01;P4M0|=0x01;}//推挽输出
#endif 

void led8_enable();     //使能led
void led8_disable();    //禁用led

//========================================================================
// 描述: 使能LED.
// 参数: none.
// 返回: none.
//========================================================================
void led8_enable()
{
    LED8_CONTROL_MODE;//推挽输出
    LED8_CONTROL_PIN = 0;
}

//========================================================================
// 描述: 禁用LED.
// 参数: none.
// 返回: none.
//========================================================================
void led8_disable()
{
    LED8_CONTROL_MODE;//推挽输出
    LED8_CONTROL_PIN = 1;
}

#endif