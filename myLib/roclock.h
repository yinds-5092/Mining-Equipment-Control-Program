/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef _RCCLOCK_H
#define _RCCLOCK_H

#include <STC8HX.h>

__sfr __at(0x9d) IRCBAND;
__sfr __at(0x9e) LIRTRIM;
__sfr __at(0x9f) IRTRIM;
__sfr __at(0xa6) VRTRIM;

#define CLKSEL CKSEL
#define IRC24MCR  HIRCCR

#ifdef STC8H1K08

#define ID_ROMADDR ((unsigned char code *)0x1ff9)
#define VREF_ROMADDR (*(unsigned int code *)0x1ff7)
#define F32K_ROMADDR (*(unsigned int code *)0x1ff5)
#define T22M_ROMADDR (*(unsigned char code *)0x1ff4) //22.1184MHz
#define T24M_ROMADDR (*(unsigned char code *)0x1ff3) //24MHz
#define T20M_ROMADDR (*(unsigned char code *)0x1ff2) //20MHz
#define T27M_ROMADDR (*(unsigned char code *)0x1ff1) //27MHz
#define T30M_ROMADDR (*(unsigned char code *)0x1ff0) //30MHz
#define T33M_ROMADDR (*(unsigned char code *)0x1fef) //33.1776MHz
#define T35M_ROMADDR (*(unsigned char code *)0x1fee) //35MHz
#define T36M_ROMADDR (*(unsigned char code *)0x1fed) //36.864MHz
#define VRT20M_ROMADDR (*(unsigned char code *)0x1fea) //VRTRIM_20M
#define VRT35M_ROMADDR (*(unsigned char code *)0x1fe9)  //VRTRIM_35M

#endif

#ifdef STC8H8K64U

#define ID_ROMADDR ((unsigned char code *)0xfdf9)
#define VREF_ROMADDR (*(unsigned int code *)0xfdf7)
#define F32K_ROMADDR (*(unsigned int code *)0xfdf5)
#define T22M_ROMADDR (*(unsigned char code *)0xfdf4) //22.1184MHz
#define T24M_ROMADDR (*(unsigned char code *)0xfdf3) //24MHz
#define T27M_ROMADDR (*(unsigned char code *)0xfdf2) //27MHz
#define T30M_ROMADDR (*(unsigned char code *)0xfdf1) //30MHz
#define T33M_ROMADDR (*(unsigned char code *)0xfdf0) //33.1776MHz
#define T35M_ROMADDR (*(unsigned char code *)0xfdef) //35MHz
#define T36M_ROMADDR (*(unsigned char code *)0xfdee) //36.864MHz
#define T40M_ROMADDR (*(unsigned char code *)0xfded) //40MHz
#define T45M_ROMADDR (*(unsigned char code *)0xfdec) //45MHz
#define T48M_ROMADDR (*(unsigned char code *)0xfdeb) //48MHz
#define VRT6M_ROMADDR (*(unsigned char code *)0xfdea) //VRTRIM_6M
#define VRT10M_ROMADDR (*(unsigned char code *)0x1fe9) //VRTRIM_10M
#define VRT27M_ROMADDR (*(unsigned char code *)0xfde8) //VRTRIM_27M
#define VRT44M_ROMADDR (*(unsigned char code *)0x1fe7) //VRTRIM_44M

#endif

//========================================================================
// 描述: 设置系统内部高速时钟.
// 参数: clkdiv:时钟分频系数（0~255）.
// 返回: none.
//===============================================ss=========================
void rcclock_set_irc()
{
	P_SW2 |= 0x80; //使能访问 XFR，没有冲突不用关闭

//-----------------选择 20MHz-------------
#ifdef IRC_20M
	CLKDIV = 0x04;
	IRTRIM = T20M_ROMADDR;
#ifdef STC8H1K08
	VRTRIM = VRT20M_ROMADDR;
	IRCBAND &= 0xfe;
	CLKDIV = 0x00;
#elif defined STC8H8K64U
	VRTRIM = VRT27M_ROMADDR;
	IRCBAND &= 0xfc;
	IRCBAND |= 0x02;
	CLKDIV = 0x00;
#endif
#endif


//-----------------选择 22.1184MHz-------------
#ifdef IRC_22M_1184	
	CLKDIV = 0x04;
	IRTRIM = T22M_ROMADDR;
#ifdef STC8H1K08
	VRTRIM = VRT20M_ROMADDR;
	IRCBAND &= 0xfe;
	CLKDIV = 0x00;
#elif defined STC8H8K64U
	VRTRIM = VRT27M_ROMADDR;
	IRCBAND &= 0xfc;
	IRCBAND |= 0x02;
	CLKDIV = 0x00;
#endif
#endif


//-----------------选择 24MHz-------------
#ifdef IRC_24M	
	CLKDIV = 0x04;
	IRTRIM = T24M_ROMADDR;
#ifdef STC8H1K08	
	VRTRIM = VRT20M_ROMADDR;
	IRCBAND &= 0xfe;
	CLKDIV = 0x00;
#elif defined STC8H8K64U
	VRTRIM = VRT27M_ROMADDR;
	IRCBAND &= 0xfc;
	IRCBAND |= 0x02;
	CLKDIV = 0x00;
#endif
#endif


//-----------------选择 27MHz-------------
#ifdef IRC_27M
	CLKDIV = 0x04;
	IRTRIM = T27M_ROMADDR;
#ifdef STC8H1K08	
	VRTRIM = VRT35M_ROMADDR;
	IRCBAND |= 0x01;
	CLKDIV = 0x00;
#elif defined STC8H8K64U
	VRTRIM = VRT27M_ROMADDR;
	IRCBAND &= 0xfc;
	IRCBAND |= 0x02;
	CLKDIV = 0x00;
#endif
#endif


//-----------------选择 30MHz-------------
#ifdef IRC_30M
	CLKDIV = 0x04;
	IRTRIM = T30M_ROMADDR;
#ifdef STC8H1K08	
	VRTRIM = VRT35M_ROMADDR;
	IRCBAND |= 0x01;
	CLKDIV = 0x00;
#elif defined STC8H8K64U
	VRTRIM = VRT27M_ROMADDR;
	IRCBAND &= 0xfc;
	IRCBAND |= 0x02;
	CLKDIV = 0x00;
#endif
#endif


//-----------------选择 33.1776MHz-------------
#ifdef IRC_33M_1776
	CLKDIV = 0x04;
	IRTRIM = T33M_ROMADDR;
#ifdef STC8H1K08	
	VRTRIM = VRT35M_ROMADDR;
	IRCBAND |= 0x01;
	CLKDIV = 0x00;
#elif defined STC8H8K64U
	VRTRIM = VRT27M_ROMADDR;
	IRCBAND &= 0xfc;
	IRCBAND |= 0x02;
	CLKDIV = 0x00;
#endif
#endif


//-----------------选择 35MHz-------------
#ifdef IRC_35M
	CLKDIV = 0x04;
	IRTRIM = T35M_ROMADDR;
#ifdef STC8H1K08	
	VRTRIM = VRT35M_ROMADDR;
	IRCBAND |= 0x01;
	CLKDIV = 0x00;
#elif defined STC8H8K64U
	VRTRIM = VRT44M_ROMADDR;
	IRCBAND |= 0x03;
	CLKDIV = 0x00;
#endif
#endif
}


// void rcclock_set_irc(uint8 div)
// {
// 	uint8 i=255;
// 	P_SW2 |= 0x80;
// 	// CLKDIV=0x04;

// #ifdef IRC_22M_1184
// 		IRTRIM=CHIPID11;
// 		VRTRIM=CHIPID23;
// 		IRCBAND=0x02;
//         IRC24MCR = 0x80; //启动内部 IRC
//         while (!(IRC24MCR & 1)); //等待时钟稳定
//         CLKDIV = div;
//         CLKSEL = 0x00; //选择内部 IRC ( 默认 )
// #endif
// #ifdef IRC_24M
// 		CLKDIV=0x04;
// 		IRTRIM=T24M_ROMADDR;
// 		VRTRIM=VRT20M_ROMADDR;
// 		IRCBAND=0x00;
//         // IRC24MCR = 0x80; //启动内部 IRC
//         // while (!(IRC24MCR & 1)); //等待时钟稳定
//         // CLKDIV = div;
//         CLKDIV = 0x00; //选择内部 IRC ( 默认 )
		
// #ifdef PLL_96M
//         USBCLK&=0x1f;
// 		USBCLK|=0xA0;
// 		while(--i);
// 		CLKSEL=0x04;
// #endif
// #ifdef PLL_144M
//         USBCLK&=0x1f;
// 		USBCLK|=0xA0;
// 		while(--i);
// 		CLKSEL=0x84;
// #endif

// #endif

// #ifdef IRC_27M
// 		IRTRIM=CHIPID13;
// 		VRTRIM=CHIPID23;
// 		IRCBAND=0x02;
//         IRC24MCR = 0x80; //启动内部 IRC
//         while (!(IRC24MCR & 1)); //等待时钟稳定
//         CLKDIV = div;
//         CLKSEL = 0x00; //选择内部 IRC ( 默认 )
// #endif

// #ifdef IRC_30M
// 		IRTRIM=CHIPID14;
// 		VRTRIM=CHIPID23;
// 		IRCBAND=0x02;
//         IRC24MCR = 0x80; //启动内部 IRC
//         while (!(IRC24MCR & 1)); //等待时钟稳定
//         CLKDIV = div;
//         CLKSEL = 0x00; //选择内部 IRC ( 默认 )
// #endif
// #ifdef IRC_33M_1776
// 		IRTRIM=CHIPID15;
// 		VRTRIM=CHIPID23;
// 		IRCBAND=0x02;
//         IRC24MCR = 0x80; //启动内部 IRC
//         while (!(IRC24MCR & 1)); //等待时钟稳定
//         CLKDIV = div;
//         CLKSEL = 0x00; //选择内部 IRC ( 默认 )
// #endif

// #ifdef IRC_35M
// 		IRTRIM=CHIPID16;
// 		VRTRIM=CHIPID24;
// 		IRCBAND=0x03;
//         IRC24MCR = 0x80; //启动内部 IRC
//         while (!(IRC24MCR & 1)); //等待时钟稳定
//         CLKDIV = div;
//         CLKSEL = 0x00; //选择内部 IRC ( 默认 )
// #endif

// #ifdef IRC_36M_864
// 		IRTRIM=CHIPID17;
// 		VRTRIM=CHIPID24;
// 		IRCBAND=0x03;
//         IRC24MCR = 0x80; //启动内部 IRC
//         while (!(IRC24MCR & 1)); //等待时钟稳定
//         CLKDIV = div;
//         CLKSEL = 0x00; //选择内部 IRC ( 默认 )
// #endif

// #ifdef IRC_40M
// 		IRTRIM=CHIPID18;
// 		VRTRIM=CHIPID24;
// 		IRCBAND=0x03;
//         IRC24MCR = 0x80; //启动内部 IRC
//         while (!(IRC24MCR & 1)); //等待时钟稳定
//         CLKDIV = div;
//         CLKSEL = 0x00; //选择内部 IRC ( 默认 )
// #endif

// #ifdef IRC_44M_2368
// 		IRTRIM=CHIPID19;
// 		VRTRIM=CHIPID24;
// 		IRCBAND=0x03;
//         IRC24MCR = 0x80; //启动内部 IRC
//         while (!(IRC24MCR & 1)); //等待时钟稳定
//         CLKDIV = div;
//         CLKSEL = 0x00; //选择内部 IRC ( 默认 )
// #endif

// #ifdef IRC_48M
// 		IRTRIM=CHIPID20;
// 		VRTRIM=CHIPID24;
// 		IRCBAND=0x03;
//         IRC24MCR = 0x80; //启动内部 IRC
//         while (!(IRC24MCR & 1)); //等待时钟稳定
//         CLKDIV = div;
//         CLKSEL = 0x00; //选择内部 IRC ( 默认 )
// #ifdef PLL_96M
//         USBCLK&=0x1f;
// 		USBCLK|=0xC0;
// 		while(--i);
// 		CLKSEL=0x04;
// #endif
// #ifdef PLL_144M
//         USBCLK&=0x1f;
// 		USBCLK|=0xC0;
// 		while(--i);
// 		CLKSEL=0x84;
// #endif

// #endif

// #ifdef X_11M_0592
// 	XOSCCR = 0xc0; //启动外部晶振
// 	while (!(XOSCCR & 1)); //等待时钟稳定
// 	CLKDIV = div; //时钟分频
// 	CLKSEL = 0x01; //选择外部晶振
// #endif

// #ifdef X_12M
// 	XOSCCR = 0xc0; //启动外部晶振
// 	while (!(XOSCCR & 1)); //等待时钟稳定
// 	CLKDIV = div; //时钟分频
// 	CLKSEL = 0x01; //选择外部晶振
// #ifdef PLL_96M
//         USBCLK&=0x1f;
// 		USBCLK|=0x80;
// 		while(--i);
// 		CLKSEL=0x05;
// #endif
// #ifdef PLL_144M
//         USBCLK&=0x1f;
// 		USBCLK|=0x80;
// 		while(--i);
// 		CLKSEL=0x85;
// #endif
// #endif



// #ifdef X_24M
// 	XOSCCR = 0xc0; //启动外部晶振
// 	while (!(XOSCCR & 1)); //等待时钟稳定
// 	CLKDIV = div; //时钟分频
// 	CLKSEL = 0x01; //选择外部晶振
// #ifdef PLL_96M
//         USBCLK&=0x1f;
// 		USBCLK|=0xA0;
// 		while(--i);
// 		CLKSEL=0x05;
// #endif
// #ifdef PLL_144M
//         USBCLK&=0x1f;
// 		USBCLK|=0xA0;
// 		while(--i);
// 		CLKSEL=0x85;
// #endif
// #endif
// #ifdef X_32K
// 	XOSCCR = 0xc0; //启动外部晶振
// 	while (!(XOSCCR & 1)); //等待时钟稳定
// 	CLKDIV = div; //时钟分频
// 	CLKSEL = 0x01; //选择外部晶振
// #endif
// #ifdef IRC_32K
//     IRC32KCR = 0x80; //启动内部 IRC
//     while (!(IRC32KCR & 1)); //等待时钟稳定
//     CLKDIV = div;
//     CLKSEL = 0x03; //选择内部32K
 
// #endif
// }




#endif 