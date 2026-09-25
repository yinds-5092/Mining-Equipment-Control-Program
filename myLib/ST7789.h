/**
 * @file ST7789.h
 * @brief ST7789彩屏驱动头文件
 * @details 
 * @version 0.1
 * @date 2022-08-09
 * @author hhdd
 * @copyright Copyright (c) 2021 TWen51 Technology Co., Ltd.
 */
#ifndef __ST7789_H
#define __ST7789_H	

#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "twenfont.h"
#include <stdlib.h>

/************************************* 用户配置区 *****************************************/ 
#ifndef ST7789_RGB_ORDER			
#define	ST7789_RGB_ORDER			0	//RGB和BGR切换位
#endif
#ifndef USR_LCM_IF
#define USR_LCM_IF 	    			0	//1: 使用LCM模式; 0:使用普通IO口模式
#endif
#define ST7789_USE_8BIT_MODEL   	1 	//定义数据总线是否使用8位模式 0,使用16位模式.1,使用8位模式

//定义LCD的尺寸
#define ST7789_LCD_W 				240
#define ST7789_LCD_H 				320

//IO连接
#ifndef ST7789_DATAPORTH
#define ST7789_DATAPORTH 			P6     //高8位数据口,8位模式下只使用高8位 
#endif
#ifndef ST7789_DATAPORTH_IN
#define ST7789_DATAPORTH_IN	 		{ P6M1=0xff;P6M0=0x00;}		//P6口高阻输入
#endif
#ifndef ST7789_DATAPORTH_OUT
#define ST7789_DATAPORTH_OUT	 	{ P6M1=0x00;P6M0=0xff;}		//P6口推挽输出
#endif

#ifndef ST7789_DATAPORTL
#define ST7789_DATAPORTL 			P2     //低8位数据口,8位模式下只使用高8位
#endif
#ifndef ST7789_DATAPORTL_IN
#define ST7789_DATAPORTL_IN	 		{ P2M1=0xff;P2M0=0x00;}		//P2口高阻输入
#endif
#ifndef ST7789_DATAPORTL_OUT
#define ST7789_DATAPORTL_OUT	 	{ P2M1=0x00;P2M0=0xff;}		//P2口推挽输出
#endif

#ifndef ST7789_RESET
#define ST7789_RESET 				P1_5
#endif
#ifndef ST7789_RESET_OUT
#define ST7789_RESET_OUT			{P1M1&=~0x20;P1M0|=0x20;}	//推挽输出
#endif 

#ifndef ST7789_CS
#define ST7789_CS 					P1_3
#endif
#ifndef ST7789_CS_OUT
#define ST7789_CS_OUT			  	{P1M1&=~0x08;P1M0|=0x08;}	//推挽输出
#endif 

#ifndef ST7789_RS
#define ST7789_RS 					P4_5
#endif
#ifndef ST7789_RS_OUT
#define ST7789_RS_OUT			  	{P4M1&=~0x20;P4M0|=0x20;}//推挽输出
#endif 
#ifndef ST7789_RS_IN
#define ST7789_RS_IN			  	{P4M1|=0x20;P4M0&=~0x20;}//高阻输入
#endif

#ifndef ST7789_WR
#define ST7789_WR 				  	P4_2
#endif
#ifndef ST7789_WR_OUT
#define ST7789_WR_OUT  		  		{P4M1&=~0x04;P4M0|=0x04;}//推挽输出
#endif
#ifndef ST7789_WR_IN
#define ST7789_WR_IN  			  	{P4M1|=0x04;P4M0&=~0x04;}//高阻输入
#endif

#ifndef ST7789_RD
#define ST7789_RD 					P4_4
#endif
#ifndef ST7789_RD_OUT
#define ST7789_RD_OUT   			{P4M1&=~0x10;P4M0|=0x10;}//推挽输出
#endif
#ifndef ST7789_RD_IN
#define ST7789_RD_IN  				{P4M1|=0x10;P4M0&=~0x10;}//高阻输入
#endif

//LCD重要参数集
typedef struct
{
	uint16 width;			//LCD 宽度
	uint16 height;			//LCD 高度
	uint16 id;				//LCD ID
}_st7789_lcd_dev;

//LCD参数
_st7789_lcd_dev  st7789_lcddev;	//管理LCD重要参数

void ST7789_init();	//初始化
void ST7789_direction(uint8 direction);  //屏幕旋转
void ST7789_clear(uint16 color);	//清屏
void ST7789_set_cursor(uint16 x, uint16 y);	//设置光标位置
void ST7789_set_windows(uint16 x0, uint16 y0, uint16 x1, uint16 y1);	//设置显示窗口
void ST7789_draw_point(uint16 x, uint16 y, uint16 color);	//画点
void ST7789_draw_line(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color);	//画线
void ST7789_fill(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color);	//填充
void ST7789_show_char(int16 x,int16 y, uint8 chr, uint16 font_color, uint16 background_color, uint8 size,uint8 mode);	//显示一个字符
void ST7789_show_string(int16 x,int16 y,uint8 *p,uint16 font_color, uint16 background_color,uint8 size,uint8 mode);	//显示字符串

#if defined(ST7789_DRAW_LINE1)
void ST7789_draw_line1(uint16 x0,uint16 y0,uint16 x1,uint16 y1,uint8 size,uint16 color); //画粗线
#endif
#if defined(ST7789_DRAW_RECTANGLE)
void ST7789_draw_rectangle(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color);	//绘制矩形框
#endif
#if defined(ST7789_FILL_RECTANGLE)
void ST7789_fill_rectangle(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color);	//填充矩形
#endif
#if defined(ST7789_DRAW_CIRCLE)
void ST7789_draw_circle(int xc, int yc, int r, uint16 color);	//画圆
#endif
#if defined(ST7789_FILL_CIRCLE)
void ST7789_fill_circle(int xc, int yc, int r, uint16 color);	//填充圆
#endif
#if defined(ST7789_DRAW_TRIANGEL)
void ST7789_draw_triangel(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color);	//绘制三角形
#endif
#if defined(ST7789_FILL_TRIANGEL)
void ST7789_fill_triangel(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color);	//填充三角形
#endif
#if defined(ST7789_DRAW_ROUND_RECT)
void st7789_draw_round_rect(uint8 x0, uint8 y0, uint8 x1, uint8 y1,uint8 radius, uint16 color); //绘制圆角矩形
#endif
#if defined(ST7789_FILL_ROUND_RECT)
void st7789_fill_round_rect(uint8 x0, uint8 y0, uint8 x1, uint8 y1,uint8 radius, uint16 color); //填充圆角矩形
#endif
#if defined(ST7789_SHOW_NUM)
void ST7789_show_num(int16 x,int16 y,int num,uint16 font_color, uint16 background_color,uint8 size, uint8 mode);	//显示数字
void ST7789_show_float(int16 x, int16 y, float num, uint8 precision, uint16 font_color, uint16 background_color, uint8 size, uint8 mode); //显示小数
#endif
#if defined(ST7789_SHOW_FONT12)
void ST7789_show_font12(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode);	//显示12x12汉字
#endif
#if defined(ST7789_SHOW_FONT16)
void ST7789_show_font16(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode);	//显示16x16汉字
#endif
#if defined(ST7789_SHOW_FONT24)
void ST7789_show_font24(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode);	//显示24x24汉字
#endif
#if defined(ST7789_SHOW_FONT32)
void ST7789_show_font32(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode);	//显示32x32汉字
#endif
#if defined(ST7789_DRAW_IMG)
void ST7789_draw_img16(uint16 x0,uint16 y0,uint16 w,uint16 h,const uint8 *p);	//显示bmp图像
#endif

#if (USR_LCM_IF == 1)
void LCM_DMA_init(uint8 xdata *color, uint8 xdata *buf, uint16 len);
#endif

#if defined(ST7789_READ_ENABLE)
uint16 ST7789_read_id();	//读取ID号
uint16 ST7789_read_point(uint16 x, uint16 y);	//读点
#endif

//颜色
#define ST7789_WHITE         	 	0xFFFF
#define ST7789_BLACK         	 	0x0000	  
#define ST7789_BLUE             	0x001F  
#define ST7789_BRED             	0XF81F
#define ST7789_GBLUE            	0X07FF
#define ST7789_RED           	 	0xF800
#define ST7789_MAGENTA       	 	0xF81F
#define ST7789_GREEN         	 	0x07E0
#define ST7789_CYAN          	 	0x7FFF
#define ST7789_YELLOW        	 	0xFFE0
#define ST7789_BROWN            	0XBC40 //棕色
#define ST7789_BRRED            	0XFC07 //棕红色
#define ST7789_GRAY             	0X8430 //灰色
#define ST7789_DARKBLUE      	 	0X01CF	//深蓝色
#define ST7789_LIGHTBLUE      	 	0X7D7C	//浅蓝色  
#define ST7789_GRAYBLUE       	 	0X5458 //灰蓝色
#define ST7789_LIGHTGREEN     	 	0X841F //浅绿色
#define ST7789_LGRAY            	0XC618 //浅灰色(PANNEL),窗体背景色
#define ST7789_LGRAYBLUE        	0XA651 //浅灰蓝色(中间层颜色)
#define ST7789_LBBLUE           	0X2B12 //浅棕蓝色(选择条目的反色)

//========================================================================
// 描述: LCD写入一个字节数据.
// 参数: HVAL:写入的字节数据.
// 返回: none.
//========================================================================
#if ST7789_USE_8BIT_MODEL
void ST7789_write(uint8 HVAL)
{
	ST7789_CS = 0;
	ST7789_DATAPORTH = HVAL;
	ST7789_WR = 0;	
	_nop_(); _nop_(); _nop_(); _nop_();_nop_(); _nop_();_nop_(); _nop_();_nop_(); _nop_();
	ST7789_WR = 1;
	ST7789_CS = 1;
}
#else
void ST7789_write(uint8 HVAL,uint8 LVAL)
{
	ST7789_CS = 0;
	ST7789_WR = 0;	
	ST7789_DATAPORTH = HVAL;
	ST7789_DATAPORTL = LVAL;
	ST7789_WR = 1;
	ST7789_CS = 1;
}
#endif

//========================================================================
// 描述: LCD写入命令.
// 参数: Reg:写入的命令.
// 返回: none.
//========================================================================
void ST7789_wr_reg(uint16 Reg)	 
{	
	#if (USR_LCM_IF == 0)
	{
		ST7789_RS=0;
		#if ST7789_USE_8BIT_MODEL
		ST7789_write(Reg&0xFF);
		#else
		ST7789_write((Reg>>8)&0xFF,Reg&0xFF);
		#endif
		ST7789_RS=1;
	}
	#else
	{
		#if (ST7789_USE_8BIT_MODEL==0)
		LCMIFDATH = (uint8)(Reg>>8);
		#endif
		LCMIFDATL = (uint8)Reg;
		ST7789_CS = 0;
		LCMIFCR = 0x84; //写命令
		while((LCMIFSTA&0x01)==0);
		LCMIFSTA = 0x00;
		ST7789_CS = 1;		
	}
	#endif
} 

//========================================================================
// 描述: LCD写入数据.
// 参数: Data:写入的数据.
// 返回: none.
//========================================================================
void ST7789_wr_data(uint16 Data)
{
	#if (USR_LCM_IF == 0)
	{
		#if ST7789_USE_8BIT_MODEL
		ST7789_CS = 0;
		ST7789_DATAPORTH = Data&0xFF;
		ST7789_WR = 0;	
		_nop_(); _nop_(); _nop_(); _nop_();_nop_(); _nop_();_nop_(); _nop_();_nop_(); _nop_();
		ST7789_WR = 1;
		ST7789_CS = 1;
		#else
		ST7789_write((Data>>8)&0xFF,Data&0xFF);
		#endif
	}
	#else
	{
		#if (ST7789_USE_8BIT_MODEL==0)
		LCMIFDATH = (uint8)(Data>>8);
		#endif
		LCMIFDATL = (uint8)Data;
		ST7789_CS = 0;
		LCMIFCR = 0x85; //写数据
		while((LCMIFSTA&0x01)==0);
		LCMIFSTA = 0x00;
		ST7789_CS = 1;
	}
	#endif
}

//========================================================================
// 描述: LCD写入16位数据.
// 参数: Data:写入的数据.
// 返回: none.
//========================================================================
void ST7789_wr_data_16bit(uint16 Data)
{
	#if (USR_LCM_IF == 0)
	{
		#if ST7789_USE_8BIT_MODEL
		ST7789_write((Data>>8)&0xFF);
		ST7789_write(Data&0xFF);
		#else
		ST7789_write((Data>>8)&0xFF,Data&0xFF);
		#endif
	}
	#else
	{
		#if ST7789_USE_8BIT_MODEL
		LCMIFDATL = (uint8)(Data>>8)&0xFF;
		ST7789_CS = 0;
		LCMIFCR = 0x85; //写数据
		while((LCMIFSTA&0x01)==0);
		LCMIFSTA = 0x00;
		LCMIFDATL = (uint8)Data&0xFF;
		LCMIFCR = 0x85; //写数据
		while((LCMIFSTA&0x01)==0);
		LCMIFSTA = 0x00;
		ST7789_CS = 1;
		#else
		LCMIFDATH = (uint8)(Data>>8)&0xFF;
		LCMIFDATL = (uint8)Data&0xFF;
		ST7789_CS = 0;
		LCMIFCR = 0x85; //写数据
		while((LCMIFSTA&0x01)==0);
		LCMIFSTA = 0x00;		
		#endif	
	}
	#endif
}

//========================================================================
// 描述: LCD写寄存器.
// 参数: LCD_Reg:写的寄存器地址; LCD_RegValue:写入的值.
// 返回: none.
//========================================================================
void ST7789_writereg(uint16 LCD_Reg, uint16 LCD_RegValue)
{
  	ST7789_wr_reg(LCD_Reg);
	ST7789_wr_data(LCD_RegValue);
}

//========================================================================
// 描述: LCD清屏.
// 参数: color:清屏的颜色.
// 返回: none.
//========================================================================
void ST7789_clear(uint16 color)
{
	uint16 i, j;
	ST7789_set_windows(0, 0, st7789_lcddev.width - 1, st7789_lcddev.height - 1);
	for (i = 0; i < st7789_lcddev.width; i++)
	{
		for (j = 0; j < st7789_lcddev.height; j++)
		{
			ST7789_wr_data_16bit(color);
		}
	}
}

//========================================================================
// 描述: LCD画点.
// 参数: x,y: 坐标.
// 返回: none.
//========================================================================
void ST7789_draw_point(uint16 x, uint16 y, uint16 color)
{
	ST7789_set_windows(x, y, x, y);//设置光标位置 
	ST7789_wr_data_16bit(color);
}

//========================================================================
// 描述: LCD的GPIO配置.
// 参数: none.
// 返回: none.
//========================================================================
void ST7789_set_gpio()
{
	ST7789_CS_OUT;
	ST7789_RESET_OUT;
	ST7789_DATAPORTH_OUT;//P6口设置成推挽输出		
	#if (USR_LCM_IF == 0)
	{
		P3M1|=0x10;P3M0&=~0x10;
		P4M1|=0x34;P4M0&=~0x34;
		
		ST7789_RD_OUT;	  //设置为推挽输出
		ST7789_WR_OUT;
		ST7789_RS_OUT;
	}
	#else
	{
		P4M1&=~0x34;P4M0|=0x34; //P4.2,P4.4,P4.5口设置成推挽输出
		P_SW2 |= 0x80;
		LCMIFCFG = 0x04;	//bit7 1:Enable Interrupt, bit1 0:8bit mode; bit0 0:8080,1:6800
		#if (ST7789_USE_8BIT_MODEL==0)
		LCMIFCFG = 0x06;	//bit7 1:Enable Interrupt, bit1 0:8bit mode; bit0 0:8080,1:6800
		#endif
		LCMIFCFG2 = 0x09;		//RS:P45,RD:P44,WR:P42; Setup Time=2,HOLD Time=1
		LCMIFSTA = 0x00;		
	}
	#endif
}

//========================================================================
// 描述: LCD初始化.
// 参数: none.
// 返回: none.
//========================================================================
void ST7789_init()
{
	P1M1|=0x01;P1M0&=~0x01;//P10高阻输入
	ST7789_set_gpio();
	delay(50);
	ST7789_RESET = 0;
	delay(50);
	ST7789_RESET = 1;
	delay(50);				//初始化之前复位

	//*************2.4inch ST7789初始化**********//	
	ST7789_wr_reg(0x11);   //sleep out
	delay(120); 
	//----------------display and color format setting------------------//
    ST7789_wr_reg(0x36);
    ST7789_wr_data(0x00);
    ST7789_wr_reg(0x3a);
    ST7789_wr_data(0x05);
	//----------------------ST7789 Frame rate setting--------------------//
    ST7789_wr_reg(0xb2);
    ST7789_wr_data(0x0c);
    ST7789_wr_data(0x0c);
    ST7789_wr_data(0x00);
    ST7789_wr_data(0x33);
    ST7789_wr_data(0x33);
    ST7789_wr_reg(0xb7);
    ST7789_wr_data(0x35);
    //----------------------ST7789 Power setting--------------------------//
    ST7789_wr_reg(0xbb);
    ST7789_wr_data(0x1c);
    ST7789_wr_reg(0xc0);
    ST7789_wr_data(0x2c);
    ST7789_wr_reg(0xc2);
    ST7789_wr_data(0x01);
    ST7789_wr_reg(0xc3);
    ST7789_wr_data(0x0b);
    ST7789_wr_reg(0xc4);
    ST7789_wr_data(0x20);
    ST7789_wr_reg(0xc6);
    ST7789_wr_data(0x0f);
    ST7789_wr_reg(0xd0);
    ST7789_wr_data(0xa4);
    ST7789_wr_data(0xa1);
    //-----------------------ST7789 gamma setting--------------------------//
    ST7789_wr_reg(0xe0);
    ST7789_wr_data(0xd0);
    ST7789_wr_data(0x00);
    ST7789_wr_data(0x03);
    ST7789_wr_data(0x09);
    ST7789_wr_data(0x13);
    ST7789_wr_data(0x1c);
    ST7789_wr_data(0x3a);
    ST7789_wr_data(0x55);
    ST7789_wr_data(0x48);
    ST7789_wr_data(0x18);
    ST7789_wr_data(0x12);
    ST7789_wr_data(0x0e);
    ST7789_wr_data(0x19);
    ST7789_wr_data(0x1e);
    ST7789_wr_reg(0xe1);
    ST7789_wr_data(0xd0);
    ST7789_wr_data(0x00);
    ST7789_wr_data(0x03);
    ST7789_wr_data(0x09);
    ST7789_wr_data(0x05);
    ST7789_wr_data(0x25);
    ST7789_wr_data(0x3a);
    ST7789_wr_data(0x55);
    ST7789_wr_data(0x50);
    ST7789_wr_data(0x3d);
    ST7789_wr_data(0x1c);
    ST7789_wr_data(0x1d);
    ST7789_wr_data(0x1d);
    ST7789_wr_data(0x1e);
    ST7789_wr_reg(0x29);
	ST7789_direction(0);//设置LCD显示方向 
	//	LCD_BL=1;//点亮背光	 	 
}

//========================================================================
// 描述: LCM_DMA初始化.
// 参数: color:写入彩屏的数据的地址; buf:从彩屏读出时存入的地址; len:长度.
// 返回: none.
//========================================================================
#if (USR_LCM_IF == 1)
void LCM_DMA_init(uint16  *color, uint16  *buf, uint16 len)
{
	uint16 tx_addr = color;
	uint16 rx_addr = buf;
	
	P_SW2 |= 0x80;
	DMA_LCM_AMTH = (len&0xff00)>>8; //设置传输字节数高位
	DMA_LCM_AMT = (uint8)len; //低位
	
	DMA_LCM_TXAH = (tx_addr&0xff00)>>8 ;  //写入彩屏的数据的地址
	DMA_LCM_TXAL = (uint8)(tx_addr);
	
	DMA_LCM_RXAH = (rx_addr&0xff00)>>8; //从彩屏读出时存入的地址
	DMA_LCM_RXAL = (uint8)(rx_addr);
	
	DMA_LCM_STA = 0x00;
	DMA_LCM_CFG = 0x82;  //允许LCM_DMA中断
	DMA_LCM_CR = 0x00;	
}
#endif

//========================================================================
// 描述: LCD设置窗口.
// 参数: x0,y0:起始坐标; x1,y1:终点坐标.
// 返回: none.
//========================================================================
void ST7789_set_windows(uint16 x0, uint16 y0, uint16 x1, uint16 y1)
{
	ST7789_wr_reg(0x2A);
	ST7789_wr_data(x0 >> 8);
	ST7789_wr_data(0x00FF & x0);
	ST7789_wr_data(x1 >> 8);
	ST7789_wr_data(0x00FF & x1);

	ST7789_wr_reg(0x2B);
	ST7789_wr_data(y0 >> 8);
	ST7789_wr_data(0x00FF & y0);
	ST7789_wr_data(y1 >> 8);
	ST7789_wr_data(0x00FF & y1);

	ST7789_wr_reg(0x2c);	//开始写入GRAM						
}

//========================================================================
// 描述: LCD设置光标.
// 参数: x:横坐标; y:纵坐标;
// 返回: none.
//========================================================================
void ST7789_set_cursor(uint16 x, uint16 y)
{
	ST7789_set_windows(x, y, x, y);
}

//========================================================================
// 描述: 设置显示方向.
// 参数: direction:0~3;
// 返回: none.
//========================================================================
void ST7789_direction(uint8 direction)
{
	switch (direction) {
		case 0:
			st7789_lcddev.width = ST7789_LCD_W;
			st7789_lcddev.height = ST7789_LCD_H;
			/* BIT7:MY; BIT6:MX; BIT5:MV(行列交换); BIT4:ML; BIT3:0,RGB,1,BGR; BIT2:MH */
			ST7789_writereg(0x36, (0<<7)|(0<<6)|(0<<5)|(0<<4)|(ST7789_RGB_ORDER<<3)); 
			break;
		case 1:
			st7789_lcddev.width = ST7789_LCD_H;
			st7789_lcddev.height = ST7789_LCD_W;
			ST7789_writereg(0x36, (1<<7)|(0<<6)|(1<<5)|(0<<4)|(ST7789_RGB_ORDER<<3));
			break;
		case 2:
			st7789_lcddev.width = ST7789_LCD_W;
			st7789_lcddev.height = ST7789_LCD_H;
			ST7789_writereg(0x36, (1<<7)|(1<<6)|(0<<5)|(0<<4)|(ST7789_RGB_ORDER<<3));
			break;
		case 3:
			st7789_lcddev.width = ST7789_LCD_H;
			st7789_lcddev.height = ST7789_LCD_W;
			ST7789_writereg(0x36, (0<<7)|(1<<6)|(1<<5)|(0<<4)|(ST7789_RGB_ORDER<<3)); 
			break;
		default:
			break;
	}
}

//========================================================================
// 描述: LCD填充.
// 参数: sx，sy:起始坐标; x1,y1:终止坐标; color:颜色
// 返回: none.
//========================================================================
void ST7789_fill(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color)
{
	uint16 i, j;
	uint16 width = x1 - x0 + 1; 		//得到填充的宽度
	uint16 height = y1 - y0 + 1;		//高度
	ST7789_set_windows(x0, y0, x1, y1);//设置显示窗口
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
			ST7789_wr_data_16bit(color);	//写入数据 	 
	}
	ST7789_set_windows(0, 0, st7789_lcddev.width - 1, st7789_lcddev.height - 1);//恢复窗口设置为全屏
}

//========================================================================
// 描述: LCD绘制线段.
// 参数: x1，y1:起始坐标; x2,y2:终止坐标;color:颜色.
// 返回: none.
//========================================================================
void ST7789_draw_line(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color)
{
	int t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x1 - x0; //计算坐标增量 
	delta_y = y1 - y0;
	uRow = x0;
	uCol = y0;
	if (delta_x > 0)incx = 1; //设置单步方向 
	else if (delta_x == 0)incx = 0;//垂直线 
	else { incx = -1; delta_x = -delta_x; }
	if (delta_y > 0)incy = 1;
	else if (delta_y == 0)incy = 0;//水平线 
	else { incy = -1; delta_y = -delta_y; }
	if (delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴 
	else distance = delta_y;
	for (t = 0; t <= distance + 1; t++)//画线输出 
	{
		ST7789_draw_point(uRow, uCol, color);//画点 
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

//========================================================================
// 描述: 画垂直线.
// 参数: x,y:起始坐标; h:长度.
// 返回: none.
//========================================================================
void ST7789_draw_vline(uint16 x, uint16 y, uint16 h,uint16 color)
{
    ST7789_draw_line(x, y, x, y+h-1, color);
}

//========================================================================
// 描述: 画水平线.
// 参数: x,y:起始坐标; w:长度.
// 返回: none.
//========================================================================
void ST7789_draw_hline(uint16 x, uint16 y, uint16 w,uint16 color)
{
    ST7789_draw_line(x, y, x + w - 1, y, color);
}

//========================================================================
// 描述: LCD绘制一条粗线.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; size:线的粗细(0~2); color:颜色.
// 返回: none.
//========================================================================
#if defined(ST7789_DRAW_LINE1)
void ST7789_draw_line1(uint16 x0,uint16 y0,uint16 x1,uint16 y1,uint8 size,uint16 color)
{
	int t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x1-x0; //计算坐标增量
	delta_y=y1-y0; 
	uRow=x0; 
	uCol=y0; 
	if(delta_x>0)incx=1; //设置单步方向
	else if(delta_x==0)incx=0;//垂直线
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出
	{  
		if(size==0)ST7789_draw_point(uRow,uCol,color);//画点
		if(size==1)
		{
			ST7789_draw_point(uRow,uCol,color);   
			ST7789_draw_point(uRow+1,uCol,color);    
			ST7789_draw_point(uRow,uCol+1,color);    
			ST7789_draw_point(uRow+1,uCol+1,color);   
		}
		if(size==2)
		{
			ST7789_draw_point(uRow,uCol,color);
			ST7789_draw_point(uRow+1,uCol,color);
			ST7789_draw_point(uRow,uCol+1,color);
			ST7789_draw_point(uRow+1,uCol+1,color);
			ST7789_draw_point(uRow-1,uCol+1,color);
			ST7789_draw_point(uRow+1,uCol-1,color);
			ST7789_draw_point(uRow-1,uCol-1,color);
			ST7789_draw_point(uRow-1,uCol,color);
			ST7789_draw_point(uRow,uCol-1,color);
		}
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}				 
}
#endif

//========================================================================
// 描述: LCD绘制矩形框.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; color:颜色;
// 返回: none.
//========================================================================
#if defined(ST7789_DRAW_RECTANGLE)
void ST7789_draw_rectangle(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color)
{
	ST7789_draw_line(x0, y0, x1, y0,color);
	ST7789_draw_line(x0, y0, x0, y1,color);
	ST7789_draw_line(x0, y1, x1, y1,color);
	ST7789_draw_line(x1, y0, x1, y1,color);
}
#endif

//========================================================================
// 描述: 在指定位置填充矩形.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; color:颜色;
// 返回: none.
//========================================================================
#if defined(ST7789_FILL_RECTANGLE)
void ST7789_fill_rectangle(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color)
{
	ST7789_fill(x0, y0, x1, y1, color);
}
#endif

//========================================================================
// 描述: 在指定位置绘制指定大小的圆.
// 参数: xc:圆心的x坐标; yc:圆心的y坐标; color:圆的颜色; r:圆半径; fill:1-填充，0-不填充;
// 返回: none.
//========================================================================
#if defined(ST7789_DRAW_CIRCLE)
void ST7789_draw_circle(int x0, int y0, int r, uint16 color)
{
    int16 f = 1 - r;
    int16 ddF_x = 1;
    int16 ddF_y = -2 * r;
    int16 x = 0;
    int16 y = r;

    ST7789_draw_point(x0, y0 + r, color);
    ST7789_draw_point(x0, y0 - r, color);
    ST7789_draw_point(x0 + r, y0, color);
    ST7789_draw_point(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) 
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ST7789_draw_point(x0 + x, y0 + y, color);
        ST7789_draw_point(x0 - x, y0 + y, color);
        ST7789_draw_point(x0 + x, y0 - y, color);
        ST7789_draw_point(x0 - x, y0 - y, color);
        ST7789_draw_point(x0 + y, y0 + x, color);
        ST7789_draw_point(x0 - y, y0 + x, color);
        ST7789_draw_point(x0 + y, y0 - x, color);
        ST7789_draw_point(x0 - y, y0 - x, color);
    }
}
#endif


//========================================================================
// 描述: 协助填充圆工具.
// 参数: none.
// 返回: none.
//========================================================================
#if defined(ST7789_FILL_CIRCLE) || defined(ST7789_FILL_ROUND_RECT)
void ST7789_fill_circle_helper(int16 x0, int16 y0, int16 r,uint8_t corners, int16 delta,uint16 color) 
{
    int16 f = 1 - r;
    int16 ddF_x = 1;
    int16 ddF_y = -2 * r;
    int16 x = 0;
    int16 y = r;
    int16 px = x;
    int16 py = y;

    delta++; // Avoid some +1's in the loop

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (x < (y + 1)) {
            if (corners & 1)
                ST7789_draw_vline(x0 + x, y0 - y, 2 * y + delta, color);
            if (corners & 2)
                ST7789_draw_vline(x0 - x, y0 - y, 2 * y + delta, color);
        }
        if (y != py) {
            if (corners & 1)
                ST7789_draw_vline(x0 + py, y0 - px, 2 * px + delta, color);
            if (corners & 2)
                ST7789_draw_vline(x0 - py, y0 - px, 2 * px + delta, color);
            py = y;
        }
        px = x;
    }
}
#endif

//========================================================================
// 描述: 在指定位置填充指定大小的圆.
// 参数: xc:圆心的x坐标; yc:圆心的y坐标; r:圆半径; color:颜色;;
// 返回: none.
//========================================================================
#if defined(ST7789_FILL_CIRCLE)
void ST7789_fill_circle(int x0, int y0, int r, uint16 color)
{
	ST7789_draw_vline(x0, y0 - r, 2 * r + 1, color);
	ST7789_fill_circle_helper(x0, y0, r, 3, 0, color);
}
#endif

#if defined(ST7789_DRAW_ROUND_RECT) || defined(ST7789_FILL_ROUND_RECT)
void ST7789_draw_circle_helper(int16 x0, int16 y0, int16 r,uint8_t cornername, uint16 color)
{
    int16 f = 1 - r;
    int16 ddF_x = 1;
    int16 ddF_y = -2 * r;
    int16 x = 0;
    int16 y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) {
            ST7789_draw_point(x0 + x, y0 + y, color);
            ST7789_draw_point(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            ST7789_draw_point(x0 + x, y0 - y, color);
            ST7789_draw_point(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            ST7789_draw_point(x0 - y, y0 + x, color);
            ST7789_draw_point(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            ST7789_draw_point(x0 - y, y0 - x, color);
            ST7789_draw_point(x0 - x, y0 - y, color);
        }
    }
}
#endif

//========================================================================
// 描述: 画圆角矩形.
// 参数: none.
// 返回: none.
//========================================================================
#if defined(ST7789_DRAW_ROUND_RECT)
void ST7789_draw_round_rect(uint8 x0, uint8 y0, uint8 x1, uint8 y1,uint8 radius, uint16 color)
{
    int16 w = abs(x1-x0);
    int16 h = abs(y1-y0);
    int16 max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (radius > max_radius)
        radius = max_radius;
    // smarter version
    ST7789_draw_hline(x0 + radius, y0, w - 2 * radius, color);         // Top
    ST7789_draw_hline(x0 + radius, y0 + h - 1, w - 2 * radius, color); // Bottom
    ST7789_draw_vline(x0, y0 + radius, h - 2 * radius, color);         // Left
    ST7789_draw_vline(x0 + w - 1, y0 + radius, h - 2 * radius, color); // Right
    // draw four corners
    ST7789_draw_circle_helper(x0 + radius, y0 + radius, radius, 1, color);
    ST7789_draw_circle_helper(x0 + w - radius - 1, y0 + radius, radius, 2, color);
    ST7789_draw_circle_helper(x0 + w - radius - 1, y0 + h - radius - 1, radius, 4, color);
    ST7789_draw_circle_helper(x0 + radius, y0 + h - radius - 1, radius, 8, color);
}
#endif

//========================================================================
// 描述: 填充圆角矩形.
// 参数: none.
// 返回: none.
//========================================================================
#if defined(ST7789_FILL_ROUND_RECT)
void ST7789_fill_round_rect(uint8 x0, uint8 y0, uint8 x1, uint8 y1,uint8 radius, uint16 color)
{
    int16 w = abs(x1-x0);
    int16 h = abs(y1-y0);
    int16 max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (radius > max_radius)
        radius = max_radius;
    // smarter version
    ST7789_fill(x0 + radius, y0, x1 - radius, y1, color);

    // draw four corners
    ST7789_fill_circle_helper(x0 + w - radius - 1, y0 + radius, radius, 1, h - 2 * radius - 1, color);
    ST7789_fill_circle_helper(x0 + radius, y0 + radius, radius, 2, h - 2 * radius - 1, color);
}
#endif

//========================================================================
// 描述: 在指定位置绘制一个三角形.
// 参数: x0,y0,x1,y1,x2,y2:三角形三个端点的坐标; color:颜色
// 返回: none.
//========================================================================
#if defined(ST7789_DRAW_TRIANGEL)
void ST7789_draw_triangel(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color)
{
	ST7789_draw_line(x0, y0, x1, y1,color);
	ST7789_draw_line(x1, y1, x2, y2,color);
	ST7789_draw_line(x2, y2, x0, y0,color);
}
#endif

#if defined(ST7789_FILL_TRIANGEL)
//========================================================================
// 描述: 交换函数（内部调用）.
// 参数: a,b:要交换的值的指针.
// 返回: none.
//========================================================================
static void tft_swap(uint16* a, uint16* b)
{
	uint16 tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

//========================================================================
// 描述: 在指定位置填充三角形.
// 参数: x0,y0,x1,y1,x2,y2:三角形三个端点的坐标; color:颜色.
// 返回: none.
//========================================================================
void ST7789_fill_triangel(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color)
{
	uint16 a, b, y, last;
	int dx01, dy01, dx02, dy02, dx12, dy12;
	long sa = 0;
	long sb = 0;
	if (y0 > y1)
	{
		tft_swap(&y0, &y1);
		tft_swap(&x0, &x1);
	}
	if (y1 > y2)
	{
		tft_swap(&y2, &y1);
		tft_swap(&x2, &x1);
	}
	if (y0 > y1)
	{
		tft_swap(&y0, &y1);
		tft_swap(&x0, &x1);
	}
	if (y0 == y2)
	{
		a = b = x0;
		if (x1 < a)
		{
			a = x1;
		}
		else if (x1 > b)
		{
			b = x1;
		}
		if (x2 < a)
		{
			a = x2;
		}
		else if (x2 > b)
		{
			b = x2;
		}
		ST7789_fill(a, y0, b, y0, color);
		return;
	}
	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;

	if (y1 == y2)
	{
		last = y1;
	}
	else
	{
		last = y1 - 1;
	}
	for (y = y0; y <= last; y++)
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		if (a > b)
		{
			tft_swap(&a, &b);
		}
		ST7789_fill(a, y, b, y, color);
	}
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for (; y <= y2; y++)
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if (a > b)
		{
			tft_swap(&a, &b);
		}
		ST7789_fill(a, y, b, y, color);
	}
}
#endif

//========================================================================
// 描述: 在指定位置显示一个字符.
// 参数: x:起始x坐标; y:起始y坐标; chr:显示的字符; font_color:显示字符的颜色值; 
//       background_color:显示字符的背景色; size:显示字符的大小; mode:0-无叠加，1-叠加:	  
// 返回: none.
//========================================================================
void ST7789_show_char(int16 x,int16 y, uint8 chr, uint16 font_color, uint16 background_color, uint8 size,uint8 mode)
{  
	uint8 code * temp=NULL;
	uint8 j,i;
	uint8 row_bit; //保存一行所占用的位数（不足一字节以一字节计）
	uint16 z;
	chr=chr-' ';  //得到偏移后的值
	if(size==12){
		row_bit = 8;
		#if defined(ASC2_12)
		temp = &asc2_1206[chr][0]; //调用1206字体
		#endif
	}else if(size==16){
		row_bit = 8;
		#if defined(ASC2_16)
		temp = &asc2_1608[chr][0]; //调用1608字体 
		#endif
	}else if(size==24){
		row_bit = 16;
		#if defined(ASC2_24)
		temp = &asc2_2412[chr][0]; //调用2412字体 
		#endif
	}else if(size==32){
		row_bit = 16;
		#if defined(ASC2_32)
		temp = &asc2_3216[chr][0]; //调用3216字体 
		#endif
	}else{
		return;	//不存在该字体则直接返回
	} 
	if(x < 0)
	{
		for(j=0;j<size;j++)
		{
			for(i=0;i<size/2;i++)
		    {   
				if(((x + i) >= 0))
				{   
					if(!mode) //非叠加方式
					{
						z = j*row_bit+i; 
						if(temp[z/8]&(0x01<<(z%8))){
							ST7789_set_cursor(x+i,y+j);
							ST7789_wr_data_16bit(font_color);
						}
						else{
							ST7789_set_cursor(x+i,y+j);
							ST7789_wr_data_16bit(background_color); 
						}
					}
					else
					{       
						z = j*row_bit+i;      
						if(temp[z/8]&(0x01<<(z%8))){
							ST7789_set_cursor(x+i,y+j);//画一个点 
							ST7789_wr_data_16bit(font_color);	
						}
					}
				}
		    }
		}	
	}else{
		for(j=0;j<size;j++)
		{
			for(i=0;i<size/2;i++)
		    {      
				if(!mode) //非叠加方式
				{
					z = j*row_bit+i; 
					if(temp[z/8]&(0x01<<(z%8))){
						ST7789_set_cursor(x+i,y+j);
						ST7789_wr_data_16bit(font_color);
					}
					else{
						ST7789_set_cursor(x+i,y+j);
						ST7789_wr_data_16bit(background_color); 
					}
				}
				else
				{    
					z = j*row_bit+i; 
					if(temp[z/8]&(0x01<<(z%8))){
						ST7789_set_cursor(x+i,y+j);//画一个点 
						ST7789_wr_data_16bit(font_color);	
					}
				}
		    }
		}	
	}
	ST7789_set_windows(0,0,st7789_lcddev.width-1,st7789_lcddev.height-1);//恢复窗口为全屏	   	 	  
}

//========================================================================
// 描述: 在指定位置显示字符串.
// 参数: x:起始x坐标; y:起始y坐标; p:要显示的字符串; font_color:字符串的颜色值; background_color:背景色
//       size:显示字符的大小（12或16）;  mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
void ST7789_show_string(int16 x,int16 y,uint8 *p,uint16 font_color, uint16 background_color,uint8 size,uint8 mode)
{            
    while((*p<='~')&&(*p>=' '))		//判断是不是非法字符	
    {       
        ST7789_show_char(x,y,*p,font_color,background_color,size,mode);
        x+=size/2;
        p++;
    }  
} 

//========================================================================
// 描述: 获得m的n次幂.
// 参数: m,n:
// 返回: none.
//========================================================================
// uint32 tft_mypow(uint8 m,uint8 n)
// {
// 	uint32 result=1;	 
// 	while(n--)result*=m;    
// 	return result;
// }

//========================================================================
// 描述: 在指定位置显示数字.
// 参数: x:三角形边的起始x坐标; y:三角形边的起始y坐标; num:要显示的数字(0~4294967295);
//       len:长度; font_color:字符串的颜色值; background_color:背景色; size:显示的大小 mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
// void ST7789_show_num(uint16 x,uint16 y,uint32 num,uint8 len,uint16 font_color, uint16 background_color,uint8 size, uint8 mode)
// {         	
// 	uint8 t,temp;
// 	uint8 enshow=0;						   
// 	for(t=0;t<len;t++)
// 	{
// 		temp=(num/tft_mypow(10,len-t-1))%10;
// 		if(enshow==0&&t<(len-1))
// 		{
// 			if(temp==0)
// 			{
// 				ST7789_show_char(x+(size/2)*t,y,' ',font_color,background_color,size,mode);
// 				continue;
// 			}else enshow=1;  
// 		}
// 	 	ST7789_show_char(x+(size/2)*t,y,temp+'0',font_color,background_color,size,mode); 
// 	}
// }

//========================================================================
// 描述: 在指定位置显示数字.
// 参数: x:三角形边的起始x坐标; y:三角形边的起始y坐标; num:要显示的数字(0~4294967295);
//       len:长度; font_color:字符串的颜色值; background_color:背景色; size:显示的大小 mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
#if defined(ST7789_SHOW_NUM)
void ST7789_show_num(int16 x, int16 y, int num, uint16 font_color, uint16 background_color, uint8 size, uint8 mode)
{
	char men[10];
	sprintf(men,"%d",num);
	ST7789_show_string(x,y,men,font_color,background_color,size,mode);
} 
#endif

//========================================================================
// 描述: 在指定位置显示小数.
// 参数: x:起始x坐标; y:起始y坐标; num:要显示的数字;precision:精度;
//       font_color:字符串的颜色值; background_color:背景色; size:显示的大小（12/16） mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
#if defined(ST7789_SHOW_NUM)
#if defined(_STC32G) || defined(_STC16)
void ST7789_show_float(int16 x, int16 y, float num, uint8 precision, uint16 font_color, uint16 background_color, uint8 size, uint8 mode)
{
    char men[15];
	switch(precision)
	{
		case 1:
			sprintf(men,"%.1f",num);	//保留1位精度
		break;
					
		case 2:
			sprintf(men,"%.2f",num);	//保留2位精度
		break;
					
		case 3:
			sprintf(men,"%.3f",num);	//保留3位精度
		break;
					
		case 4:
			sprintf(men,"%.4f",num);	//保留4位精度
		break;
					
		default:
			sprintf(men,"%.1f",num);	//保留1位精度
		break;
	}
	ST7789_show_string(x,y,men,font_color,background_color,size,mode);
}
#else 
void ST7789_show_float(int16 x, int16 y, float num, uint8 precision, uint16 font_color, uint16 background_color, uint8 size, uint8 mode)
{
    uint8 i;
	uint8 float_bit;
    int16 int_part;
	int_part = (int16)num/1;

    if(num>=0)			//整数部分
    {
		ST7789_show_num( x, y, int_part, font_color, background_color, size, mode);
        for(i=1;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
		num = num - int_part;
		ST7789_show_char(x+(size/2)*i, y, '.', font_color, background_color, size, mode);	//显示小数点
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				ST7789_show_num(x+(size/2)*(i+1), y, (int16)float_bit, font_color, background_color, size, mode);	
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				ST7789_show_num(x+(size/2)*(i+1), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*100)%10;
				ST7789_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				ST7789_show_num(x+(size/2)*(i+1), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*100)%10;
				ST7789_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*1000)%10;
				ST7789_show_num(x+(size/2)*(i+3), y, (int16)float_bit, font_color, background_color, size, mode);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				ST7789_show_num(x+(size/2)*(i+1), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				ST7789_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				ST7789_show_num(x+(size/2)*(i+3), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				ST7789_show_num(x+(size/2)*(i+4), y, (int16)float_bit, font_color, background_color, size, mode);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				ST7789_show_num(x+(size/2)*(i+1), y, (int16)float_bit, font_color, background_color, size, mode);
			break;//保留1位精度
		}
    }
    else
    {
		i = 0;
		if(int_part == 0)
		{
			ST7789_show_char(x, y, '-', font_color, background_color, size, mode);	//显示负号
			i++;
		}
		ST7789_show_num( x+(size/2)*i, y, int_part, font_color, background_color, size, mode);
        for(i=1;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
		num = -(num - int_part);
		ST7789_show_char( x+(size/2)*(i+1), y, '.', font_color, background_color, size, mode);
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				ST7789_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);	
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				ST7789_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*100)%10;
				ST7789_show_num(x+(size/2)*(i+3), y, (int16)float_bit, font_color, background_color, size, mode);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				ST7789_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*100)%10;
				ST7789_show_num(x+(size/2)*(i+3), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*1000)%10;
				ST7789_show_num(x+(size/2)*(i+4), y, (int16)float_bit, font_color, background_color, size, mode);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				ST7789_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				ST7789_show_num(x+(size/2)*(i+3), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				ST7789_show_num(x+(size/2)*(i+4), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				ST7789_show_num(x+(size/2)*(i+5), y, (int16)float_bit, font_color, background_color, size, mode);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				ST7789_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
			break;//保留1位精度
		}
    }
} 
#endif
#endif
//========================================================================
// 描述: 在指定位置显示12*12字体汉字.
// 参数: hz:汉字的指针; x:起始x坐标; y:起始y坐标; lenth: 字体的总长度 
//       font_color:显示字符的颜色值; background_color:显示字符的背景色;mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
#if defined(ST7789_SHOW_FONT12)
void ST7789_show_font12(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode)
{
	uint8 zz,i,j;
	int16 zs;
	if(x < 0)
	{
		zs = (0-x) / 12;	
		for(zz = zs; zz < lenth; zz++)		//要显示的字体循环
		{
			for(i = 0;i < 12;i++)
			{
				for(j = 0;j < 12;j++)
				{
					if((zz * 12 + j + x -12) > 239)return;
					if((y + i -12) > 339)return;
					if (!((zz * 12 + x + j) < 0)){
						if(!mode)		//无叠加
						{
							if(hz[zz*24 + i * 2 + j/8] & (0x80 >> (j % 8))){
								ST7789_set_cursor(zz * 12+ x + j,y + i);
								ST7789_wr_data_16bit(font_color);
							}
							else{
								ST7789_set_cursor(zz * 12+ x + j,y + i);
								ST7789_wr_data_16bit(background_color);
							} 
						}
						else
						{
							if(hz[zz*24 + i * 2 + j/8] & (0x80>>(j % 8))){
								ST7789_set_cursor(zz * 12+ x + j,y + i);
								ST7789_wr_data_16bit(font_color);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		for(zz = 0; zz < lenth; zz++)		//要显示的字体循环
		{
			
			for(i = 0;i < 12;i++)
			{
				for(j = 0;j < 12;j++)
				{
					if((zz * 12 + j + x -12) > 239)return;
					if((y + i -12) > 339)return;
					if(!mode)		//无叠加
					{
						if(hz[zz*24 + i * 2 + j/8] & (0x80 >> (j % 8))){
							ST7789_set_cursor(zz * 12+ x + j,y + i);
							ST7789_wr_data_16bit(font_color);
						}
						else{
							ST7789_set_cursor(zz * 12+ x + j,y + i);
							ST7789_wr_data_16bit(background_color);
						}
					}
					else
					{
						if(hz[zz*24 + i * 2 + j/8] & (0x80>>(j % 8))){
							ST7789_set_cursor(zz * 12+ x + j,y + i);
							ST7789_wr_data_16bit(font_color);						
						}
					}
				}
			}
		}
	}
	ST7789_set_windows(0,0,st7789_lcddev.width-1,st7789_lcddev.height-1);//恢复窗口为全屏
}
#endif

//========================================================================
// 描述: 在指定位置显示16*16字体汉字.
// 参数: hz:汉字的指针; x:起始x坐标; y:起始y坐标; lenth: 字体的总长度 
//       font_color:显示字符的颜色值; background_color:显示字符的背景色;mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
#if defined(ST7789_SHOW_FONT16)
void ST7789_show_font16(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode)
{
	uint8 zz,i,j;
	int16 zs;
	if(x < 0)
	{
		zs = (0-x) / 16;	
		for(zz = zs; zz < lenth; zz++)		//要显示的字体循环
		{
			for(i = 0;i < 16;i++)
			{
				for(j = 0;j < 16;j++)
				{
					if((zz * 16 + j + x -16) > 239)return;
					if((y + i -16) > 339)return;
					if (!((zz * 16 + x + j) < 0)){
						if(!mode)		//无叠加
						{
							if(hz[zz*32+i * 2 + j/8] & (0x80 >> (j % 8))){
								ST7789_set_cursor(zz * 16+ x + j,y + i);
								ST7789_wr_data_16bit(font_color);
							}
							else{
								ST7789_set_cursor(zz * 16+ x + j,y + i);
								ST7789_wr_data_16bit(background_color);
							}
						}
						else
						{
							if(hz[zz*32 + i * 2 + j/8] & (0x80>>(j % 8))){
								ST7789_set_cursor(zz * 16+ x + j,y + i);
								ST7789_wr_data_16bit(font_color);							
							}
						}
					}
				}
			}
		}
	}
	else
	{
		for(zz = 0; zz < lenth; zz++)		//要显示的字体循环
		{
			for(i = 0;i < 16;i++)
			{
				for(j = 0;j < 16;j++)
				{
					if((zz * 16 + j + x -16) > 239)return;
					if((y + i -16) > 339)return;

					if(!mode)		//无叠加
					{
						if(hz[zz*32+i * 2 + j/8] & (0x80 >> (j % 8))){
							ST7789_set_cursor(zz * 16+ x + j,y + i);
							ST7789_wr_data_16bit(font_color);
						}
						else{
							ST7789_set_cursor(zz * 16+ x + j,y + i);
							ST7789_wr_data_16bit(background_color);
						}
					}
					else
					{
						if(hz[zz*32 + i * 2 + j/8] & (0x80>>(j % 8))){
							ST7789_set_cursor(zz * 16+ x + j,y + i);
							ST7789_wr_data_16bit(font_color);
						}
					}
				}
			}
		}
	}
	ST7789_set_windows(0,0,st7789_lcddev.width-1,st7789_lcddev.height-1);//恢复窗口为全屏
}
#endif

//========================================================================
// 描述: 在指定位置显示24*24字体汉字.
// 参数: hz:汉字的指针; x:起始x坐标; y:起始y坐标; lenth: 字体的总长度 
//       font_color:显示字符的颜色值; background_color:显示字符的背景色;mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
#if defined(ST7789_SHOW_FONT24)
void ST7789_show_font24(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode)
{
	uint8 zz,i,j;
	int16 zs;
	if(x < 0)
	{
		zs = (0-x) / 24;	
		for(zz = zs; zz < lenth; zz++)		//要显示的字体循环
		{
			for(i = 0;i < 24;i++)
			{
				for(j = 0;j < 24;j++)
				{
					if((zz * 24 + j + x -24) > 239)return;
					if((y + i -24) > 339)return;
					if (!((zz * 24 + x + j) < 0))
					{
						if(!mode)		//无叠加
						{
							if(hz[zz*72+i * 3 + j/8] & (0x80 >> (j % 8))){
								ST7789_set_cursor(zz * 24+ x + j,y + i);
								ST7789_wr_data_16bit(font_color);
							}
							else {
								ST7789_set_cursor(zz * 24+ x + j,y + i);
								ST7789_wr_data_16bit(background_color);
							}
						}
						else
						{
							if(hz[zz*72 + i * 3 + j/8] & (0x80>>(j % 8))){
								ST7789_set_cursor(zz * 24+ x + j,y + i);
								ST7789_wr_data_16bit(font_color);	
							}
						}
					}
				}
			}
		}
	}
	else
	{
		for(zz = 0; zz < lenth; zz++)		//要显示的字体循环
		{
			for(i = 0;i < 24;i++)
			{
				for(j = 0;j < 24;j++)
				{
					if((zz * 24 + j + x -24) > 239)return;
					if((y + i -24) > 339)return;

					if(!mode)		//无叠加
					{
						if(hz[zz*72+i * 3 + j/8] & (0x80 >> (j % 8))){
							ST7789_set_cursor(zz * 24+ x + j,y + i);
							ST7789_wr_data_16bit(font_color);
						}
						else{
							ST7789_set_cursor(zz * 24+ x + j,y + i);
							ST7789_wr_data_16bit(background_color);
						}
					}
					else
					{
						if(hz[zz*72 + i * 3 + j/8] & (0x80>>(j % 8))){
							ST7789_set_cursor(zz * 24+ x + j,y + i);
							ST7789_wr_data_16bit(font_color);	
						}
					}
				}
			}
		}
	}
	ST7789_set_windows(0,0,st7789_lcddev.width-1,st7789_lcddev.height-1);//恢复窗口为全屏
}
#endif

//========================================================================
// 描述: 在指定位置显示32*32字体汉字.
// 参数: hz:汉字的指针; x:起始x坐标; y:起始y坐标; lenth: 字体的总长度 
//       font_color:显示字符的颜色值; background_color:显示字符的背景色;mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
#if defined(ST7789_SHOW_FONT32)
void ST7789_show_font32(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode)
{
	uint8 zz,i,j;
	int16 zs;
	if(x < 0)
	{
		zs = (0-x) / 32;	
		for(zz = zs; zz < lenth; zz++)		//要显示的字体循环
		{
			for(i = 0;i < 32;i++)
			{
				for(j = 0;j < 32;j++)
				{
					if((zz * 34 + j + x -34) > 239)return;
					if((y + i -34) > 339)return;
					if (!((zz * 34 + x + j) < 0)){
						if(!mode)		//无叠加
						{
							if (hz[zz * 128 + j * 4 + i/ 8] & (0x80 >> (i % 8))){
								ST7789_set_cursor(zz * 32+ x + j,y + i);
								ST7789_wr_data_16bit(font_color);
							}
							else{ 
								ST7789_set_cursor(zz * 32+ x + j,y + i);
								ST7789_wr_data_16bit(background_color);
							}
						}
						else
						{
							if (hz[zz * 128 + j * 4 + i/ 8] & (0x80 >> (i % 8))){
								ST7789_set_cursor(zz * 32+ x + j,y + i);
								ST7789_wr_data_16bit(font_color);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		for(zz = 0; zz < lenth; zz++)		//要显示的字体循环
		{
			ST7789_set_windows(x+32*zz,y,x+32*zz+32-1,y+32-1);
			for(i = 0;i < 32;i++)
			{
				for(j = 0;j < 32;j++)
				{
					if((zz * 32 + j + x -32) > 239)return;
					if((y + i -32) > 339)return;

					if(!mode)		//无叠加
					{
						if (hz[zz * 128 + j * 4 + i/ 8] & (0x80 >> (i % 8))){
							ST7789_set_cursor(zz * 32+ x + j,y + i);
							ST7789_wr_data_16bit(font_color);
						}
						else{
							ST7789_set_cursor(zz * 32+ x + j,y + i);
							ST7789_wr_data_16bit(background_color);
						}
					}
					else
					{
						if (hz[zz * 128 + j * 4 + i/ 8] & (0x80 >> (i % 8))){
							ST7789_set_cursor(zz * 32+ x + j,y + i);
							ST7789_wr_data_16bit(font_color);
						}
					}
				}
			}
		}
	}
	ST7789_set_windows(0,0,st7789_lcddev.width-1,st7789_lcddev.height-1);//恢复窗口为全屏
}
#endif

//========================================================================
// 描述: 显示16位的BMP图像.
// 参数: x0:起始x坐标; y0:起始y坐标;w:图片的宽度; h:图片的高度; p:图像数组的起始地址
// 返回: none.
//========================================================================
#if defined(ST7789_DRAW_IMG)
void ST7789_draw_img16(uint16 x,uint16 y,uint16 w,uint16 h,const uint8 *p)
{
  	int i; 
	uint8 picH,picL; 
	ST7789_set_windows(x,y,x+w-1,y+h-1);				//窗口设置
    for(i=0;i<(w*h);i++)
	{	
	 	picL=*(p+i*2);		//数据低位在前
		picH=*(p+i*2+1);				
		ST7789_wr_data_16bit(picH<<8|picL);  						
	}	
	ST7789_set_windows(0,0,st7789_lcddev.width-1,st7789_lcddev.height-1);//恢复显示窗口为全屏
}
#endif

/**********************************************************************************’
 * 			ST7789读寄存器函数，通过ST7789_READ_ENABLE宏定义使能
 *
 ************************************************************************************/
#if defined(ST7789_READ_ENABLE)
//========================================================================
// 描述: LCD读取16位数据.
// 参数: none.
// 返回: 读取到的16位数据.
//========================================================================
uint16 ST7789_read()
{
	uint16 d;
	#if (USR_LCM_IF == 0)
	{
		#if ST7789_USE_8BIT_MODEL
		ST7789_CS = 0;
		ST7789_RD = 0;
		delay1us(); //delay 1 us
		d = ST7789_DATAPORTH;
		d = (d<<8);
		ST7789_RD = 1;
		ST7789_CS = 1;
		#else
		ST7789_CS = 0;
		ST7789_RD = 0;
		delay1us(); //delay 1 us
		d = ST7789_DATAPORTH;
		d = (d<<8) | ST7789_DATAPORTL;
		ST7789_RD = 1;
		ST7789_CS = 1;
		#endif
	}
	#else
	{
		#if ST7789_USE_8BIT_MODEL
		ST7789_CS = 0;
		LCMIFCR = 0x87; //读数据
		while((LCMIFSTA&0x01)==0);
		LCMIFSTA = 0x00;
		d = LCMIFDATL;
		d = (d<<8);
		ST7789_CS = 1;
		#else
		ST7789_CS = 0;
		LCMIFCR = 0x87; //读数据
		while((LCMIFSTA&0x01)==0);
		LCMIFSTA = 0x00;
		d = LCMIFDATH;
		d = (d<<8) | LCMIFDATL;
		ST7789_CS = 1;
		#endif
	}
	#endif
	return d;
}

//========================================================================
// 描述: LCD读取数据.
// 参数: none.
// 返回: 读取到的数据.
//========================================================================
uint16 ST7789_rd_data()
{
	#if ST7789_USE_8BIT_MODEL
	return (ST7789_read()>>8);
	#else
	return ST7789_read();
	#endif
}

//========================================================================
// 描述: 将RGB转换为565格式.
// 参数: r,g,b:需要转换的RGB值.
// 返回: 转换完成的565格式的16位数值.
//========================================================================
uint16 tft_color_to_565(uint8 r, uint8 g, uint8 b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

//========================================================================
// 描述: LCD读取565格式的16位数值.
// 参数: none.
// 返回: 565格式的数据.
//========================================================================
uint16 ST7789_rd_data_16bit()
{
	uint16 r,g,b;

	#if (USR_LCM_IF == 0)
	{
		ST7789_RS = 1;
		ST7789_CS = 0;
		//dummy data
		r = ST7789_read();
		//8bit:red data
		//16bit:red and green data
		r = ST7789_read();
		//8bit:green data
		//16bit:blue data
		g = ST7789_read();
		#if ST7789_USE_8BIT_MODEL	
		g = ST7789_read();
		r >>= 8;
		g >>= 8;
		b >>= 8;
		#else
		b = g>>8;
		r = r>>8;
		g = r&0xFF;
		#endif
		ST7789_CS = 1;
	}
	#else
	{	
		ST7789_CS = 0;
		//dummy data
		r = ST7789_read();
		//8bit:red data
		//16bit:red and green data
		r = ST7789_read();
		//8bit:green data
		//16bit:blue data
		g = ST7789_read();
		#if ST7789_USE_8BIT_MODEL	
		b = ST7789_read();
		r >>= 8;
		g >>= 8;
		b >>= 8;
		#else
		b = g>>8;
		r = r>>8;
		g = r&0xFF;
		#endif
		ST7789_CS = 1;		
	}
	#endif
	return tft_color_to_565((uint8)r, (uint8)g, (uint8)b);
}

//========================================================================
// 描述: LCD读寄存器.
// 参数: LCD_Reg:读的寄存器地址; Rval:读取到的值; n:需要连续读取的数量.
// 返回: none.
//========================================================================
void ST7789_readreg(uint8 LCD_Reg, uint8* Rval, int n)
{
	ST7789_wr_reg(LCD_Reg);
	ST7789_DATAPORTH = 0xFF; //拉高P6
	ST7789_DATAPORTH_IN;	//P6口设置成输入口
	#if (ST7789_USE_8BIT_MODEL==0)
	ST7789_DATAPORTL = 0xFF; //拉高P2
	ST7789_DATAPORTL_IN;	//P2口设置成输入口
	#endif
	
	while (n--)
	{
		*(Rval++) = ST7789_rd_data();
	}

	ST7789_DATAPORTH = 0xFF; //拉高P6
	ST7789_DATAPORTH_OUT;		//P6口设置为推挽输出
	#if (ST7789_USE_8BIT_MODEL==0)
	ST7789_DATAPORTL = 0xFF; //拉高P2
	ST7789_DATAPORTL_OUT;	//P2口设置成输入口
	#endif
}

//========================================================================
// 描述: LCD读点.
// 参数: x,y: 坐标.
// 返回: 读到的颜色值.
//========================================================================
uint16 ST7789_read_point(uint16 x, uint16 y)
{
	uint16 color;
	if (x >= st7789_lcddev.width || y >= st7789_lcddev.height)
	{
		return 0;	//超过了范围,直接返回	
	}
	ST7789_set_cursor(x, y);//设置光标位置 
	ST7789_wr_reg(0x2E);
	ST7789_DATAPORTH = 0xFF; //拉高P6
	ST7789_DATAPORTH_IN;	//P6口设置成输入口
	#if (ST7789_USE_8BIT_MODEL==0)
	ST7789_DATAPORTL = 0xFF; //拉高P2
	ST7789_DATAPORTL_IN;	//P2口设置成输入口
	#endif
	color = ST7789_rd_data_16bit();
	ST7789_DATAPORTH = 0xFF; //拉高P6
	ST7789_DATAPORTH_OUT;//P6口设置成推挽输出
	#if (ST7789_USE_8BIT_MODEL==0)
	ST7789_DATAPORTL = 0xFF; //拉高P2
	ST7789_DATAPORTL_OUT;	//P2口设置成输入口
	#endif
	return color;
}

//========================================================================
// 描述: LCD读取ID.
// 参数: none;
// 返回: 读取到的ID号.
//========================================================================
uint16 ST7789_read_id()
{
	uint8 val[4] = { 0 };
	ST7789_readreg(0xD3, val, 4);
	return (val[2] << 8) | val[3];
}
#endif	//ST7789_READ_ENABLE



#endif  //tftlcd.h





