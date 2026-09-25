/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/
#ifndef _SYSCLOCK_H
#define _SYSCLOCK_H

#include <STC8HX.h>

void sysclock_set_hir_irc(uint8 clkdiv);
void sysclock_set_32k_irc(uint8 clkdiv);
void sysclock_set_xosc(uint8 clkdiv);

//========================================================================
// 描述: 设置系统内部高速时钟.
// 参数: clkdiv:时钟分频系数（0~255）.
// 返回: none.
//========================================================================
void sysclock_set_hir_irc(uint8 clkdiv)
{
    P_SW2 = 0x80;
    HIRCCR = 0x80; //启动内部 IRC
    while (!(HIRCCR & 1)); //等待时钟稳定
    CLKDIV = clkdiv;
    CKSEL = 0x00; //选择内部 IRC ( 默认 )
    P_SW2 = 0x00;
}

//========================================================================
// 描述: 设置系统内部32K IRC时钟.
// 参数: clkdiv:时钟分频系数（0~255）.
// 返回: none.
//========================================================================
void sysclock_set_32k_irc(uint8 clkdiv)
{
    P_SW2 = 0x80;
    IRC32KCR = 0x80; //启动内部 IRC
    while (!(IRC32KCR & 1)); //等待时钟稳定
    CLKDIV = clkdiv;
    CKSEL = 0x03; //选择内部32K
    P_SW2 = 0x00;
}

//========================================================================
// 描述: 设置系统外部晶振时钟.
// 参数: clkdiv:时钟分频系数（0~255）.
// 返回: none.
//========================================================================
void sysclock_set_xosc(uint8 clkdiv)
{
    P_SW2 = 0x80;
	XOSCCR = 0xc0; //启动外部晶振
	while (!(XOSCCR & 1)); //等待时钟稳定
	CLKDIV = clkdiv; //时钟分频
	CKSEL = 0x01; //选择外部晶振
    P_SW2 = 0x00;
}


#endif 