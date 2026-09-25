/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/
#ifndef _RGB_H_
#define _RGB_H_

#include <STC8HX.h>
#include "delay.h"

#ifndef RGB_NUMLEDS
#define RGB_NUMLEDS 1 //个数
#endif
xdata uint8 _rgb_buf_R[RGB_NUMLEDS] = { 0 };//颜色缓存
xdata uint8 _rgb_buf_G[RGB_NUMLEDS] = { 0 };
xdata uint8 _rgb_buf_B[RGB_NUMLEDS] = { 0 };

#ifndef	RGB_PIN
#define RGB_PIN			P4_5
#endif 

#ifndef RGB_PIN_MODE
#define RGB_PIN_MODE	{P4M1&=~0x20;P4M0|=0x20;}//推挽输出
#endif

void rgb_init();
void rgb_show(uint8 num, uint8 r, uint8 g, uint8 b);
//========================================================================
// 描述: 延迟5微秒.
// 参数: none.
// 返回: none.
//========================================================================
void rgb_init()
{
	RGB_PIN_MODE;//推挽输出
}

//========================================================================
// 描述: 1码，高电平850ns 低电平400ns 误差正负150ns.
// 参数: none.
// 返回: none.
//========================================================================
void rgb_set_up()
{
	RGB_PIN = 1;
	//经过逻辑分析仪调试的的延时
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	RGB_PIN = 0;
}

//========================================================================
// 描述: 0码，高电平400ns 低电平850ns 误差正负150ns.
// 参数: none.
// 返回: none.
//========================================================================
void rgb_set_down()
{
	RGB_PIN = 1;
	//经过逻辑分析仪调试的的延时
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	RGB_PIN = 0;
}

//========================================================================
// 描述: 发送24位数据.
// 参数: 绿色8位，红色8位，蓝色8位.
// 返回: none.
//========================================================================
void rgb_write(uint8 G8, uint8 R8, uint8 B8)
{
	unsigned int n = 0;
	//发送G8位
	EA = 0;
	for (n = 0; n < 8; n++)
	{

		if ((G8 & 0x80) == 0x80)
		{
			rgb_set_up();
		}
		else
		{
			rgb_set_down();
		}
		G8 <<= 1;
	}
	//发送R8位
	for (n = 0; n < 8; n++)
	{

		if ((R8 & 0x80) == 0x80)
		{
			rgb_set_up();
		}
		else
		{
			rgb_set_down();
		}
		R8 <<= 1;

	}
	//发送B8位
	for (n = 0; n < 8; n++)
	{

		if ((B8 & 0x80) == 0x80)
		{
			rgb_set_up();
		}
		else
		{
			rgb_set_down();
		}
		B8 <<= 1;
	}
	EA = 1;
}

//========================================================================
// 描述: 复位.
// 参数: none.
// 返回: none.
//========================================================================
void rgb_rst()
{
	RGB_PIN = 0;
	delay50us();
}

// //========================================================================
// // 描述: 把24位数据GRB码转RGB.
// // 参数: none.
// // 返回: none.
// //========================================================================
// void Set_Colour(uint8 r, uint8 g, uint8 b)
// {
// 	uint8 i;
// 	for (i = 0; i < RGB_NUMLEDS; i++)
// 	{
// 		_rgb_buf_R[i] = r; //缓冲
// 		_rgb_buf_G[i] = g;
// 		_rgb_buf_B[i] = b;
// 	}
// 	for (i = 0; i < RGB_NUMLEDS; i++)
// 	{
// 		rgb_write(_rgb_buf_G[i], _rgb_buf_R[i], _rgb_buf_B[i]);//发送显示
// 	}
// }

//========================================================================
// 描述: 设置指定点的显示颜色.
// 参数: 第几个RGB灯，R值,G值,B值.
// 返回: none.
//========================================================================
void rgb_set_color(uint8 num, uint8 r, uint8 g, uint8 b)
{
	uint8 i;
	for (i = 0; i < RGB_NUMLEDS; i++)
	{
		_rgb_buf_R[num] = r;//缓冲
		_rgb_buf_G[num] = g;
		_rgb_buf_B[num] = b;
	}
	for (i = 0; i < RGB_NUMLEDS; i++)
	{
		rgb_write(_rgb_buf_G[i], _rgb_buf_R[i], _rgb_buf_B[i]);//发送显示
	}
}

// //========================================================================
// // 描述: 颜色交换24位不拆分发.
// // 参数: none.
// // 返回: none.
// //========================================================================
// void SetPixelColor(uint8 num, uint32 c)
// {
// 	uint8 i;
// 	for (i = 0; i < RGB_NUMLEDS; i++)
// 	{
// 		_rgb_buf_R[num] = (uint8)(c >> 16);
// 		_rgb_buf_G[num] = (uint8)(c >> 8);
// 		_rgb_buf_B[num] = (uint8)(c);
// 	}
// 	for (i = 0; i < RGB_NUMLEDS; i++)
// 	{
// 		rgb_write(_rgb_buf_G[i], _rgb_buf_R[i], _rgb_buf_B[i]);
// 	}
// }

//========================================================================
// 描述: rgb延时.
// 参数: none.
// 返回: none.
//========================================================================
void rgb_hal_delay(unsigned int t)
{
	unsigned int x, y;
	for (x = 114; x > 0; x--)
		for (y = t; y > 0; y--);
}

//========================================================================
// 描述: 延迟5微秒.
// 参数: none.
// 返回: none.
//========================================================================
void rgb_show(uint8 num, uint8 r, uint8 g, uint8 b)
{
	rgb_rst();
	rgb_hal_delay(30);
	rgb_set_color(num,r,g,b);
}

#endif 

