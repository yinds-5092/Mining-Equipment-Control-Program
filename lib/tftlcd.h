/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/
#ifndef __TFTLCD_H
#define __TFTLCD_H	

#include <STC8HX.h>
#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "lcdfont.h"	
#include <stdlib.h>

/////////////////////////////////////用户配置区///////////////////////////////////	 
//支持横竖屏快速定义切换
#ifndef TFT_LCD_USE_HORIZONTAL 
#define TFT_LCD_USE_HORIZONTAL			0   //定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转
#endif

#define TFT_LCD_USE_8BIT_MODEL   	1 	//定义数据总线是否使用8位模式 0,使用16位模式.1,使用8位模式
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#define TFT_LCD_LCD_W 240
#define TFT_LCD_LCD_H 320

//IO连接
#ifndef TFT_LCD_DATAPORTH
#define TFT_LCD_DATAPORTH 			P6     //高8位数据口,8位模式下只使用高8位 
#endif

#ifndef TFT_LCD_DATAPORTH_IN
#define TFT_LCD_DATAPORTH_IN	 	{ P6M1=0xff;P6M0=0x00;}		//P6口高阻输入
#endif

#ifndef TFT_LCD_DATAPORTH_OUT
#define TFT_LCD_DATAPORTH_OUT	 	{ P6M1=0x00;P6M0=0xff;}		//P6口推挽输出
#endif

#define TFT_LCD_DATAPORTL 			P2     //低8位数据口,8位模式下只使用高8位

#ifndef TFT_LCD_RESET
#define TFT_LCD_RESET 				P1_5
#endif

#ifndef TFT_LCD_RESET_OUT
#define TFT_LCD_RESET_OUT			{P1M1&=~0x20;P1M0|=0x20;}	//推挽输出
#endif 

#ifndef TFT_LCD_CS
#define TFT_LCD_CS 					P1_3
#endif

#ifndef TFT_LCD_CS_OUT
#define TFT_LCD_CS_OUT			  {P1M1&=~0x08;P1M0|=0x08;}	//推挽输出
#endif 

#ifndef TFT_LCD_RS
#define TFT_LCD_RS 					P0_3
#endif

#ifndef TFT_LCD_RS_OUT
#define TFT_LCD_RS_OUT			  {P0M1&=~0x08;P0M0|=0x08;}	//推挽输出
#endif 

#ifndef TFT_LCD_WR
#define TFT_LCD_WR 					P1_1
#endif

#ifndef TFT_LCD_WR_OUT
#define TFT_LCD_WR_OUT  			{P1M1&=~0x02;P1M0|=0x02;}	//推挽输出
#endif

#ifndef TFT_LCD_RD
#define TFT_LCD_RD 					P1_0
#endif

#ifndef TFT_LCD_RD_OUT
#define TFT_LCD_RD_OUT   			{P1M1&=~0x01;P1M0|=0x01;}	//推挽输出
#endif

//LCD的画笔颜色和背景色	   
extern uint16  _tft_lcd_point_color;//默认红色    
extern uint16  _tft_lcd_back_color; //背景颜色.默认为白色
//LCD重要参数集
typedef struct
{
	uint16 width;			//LCD 宽度
	uint16 height;			//LCD 高度
	uint16 id;				//LCD ID
	uint8  dir;				//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16	wramcmd;		//开始写gram指令
	uint16  rramcmd;   		//开始读gram指令
	uint16  setxcmd;		//设置x坐标指令
	uint16  setycmd;		//设置y坐标指令	 
}_lcd_dev;

//LCD参数
 _lcd_dev lcddev;	//管理LCD重要参数

void tft_lcd_init();	//初始化
void tft_lcd_clear(uint16 color);	//清屏
void tft_lcd_set_cursor(uint16 x, uint16 y);	//设置光标位置
void tft_lcd_set_windows(uint16 x0, uint16 y0, uint16 x1, uint16 y1);	//设置显示窗口
void tft_lcd_draw_point(uint16 x, uint16 y);	//画点
uint16 tft_lcd_read_point(uint16 x, uint16 y);	//读点
void tft_lcd_direction(uint8 direction);  //屏幕旋转
uint16 tft_lcd_read_id();	//读取ID号
void tft_lcd_fill(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color);	//填充
void tft_lcd_draw_line(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color);	//画线
void tft_lcd_draw_line1(uint16 x0,uint16 y0,uint16 x1,uint16 y1,uint8 size,uint16 color); //画粗线
void tft_lcd_draw_rectangle(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color);	//绘制矩形框
void tft_lcd_fill_rectangle(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color);	//填充矩形
void tft_lcd_draw_circle(int xc, int yc, int r, uint16 color, uint8 fill);	//画圆
void tft_lcd_draw_triangel(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color);	//绘制三角形
void tft_lcd_fill_triangel(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color);	//填充三角形
void tft_lcd_show_char(int16 x,int16 y, uint8 chr, uint16 font_color, uint16 background_color, uint8 size,uint8 mode);	//显示一个字符
void tft_lcd_show_string(int16 x,int16 y,uint8 *p,uint16 font_color, uint16 background_color,uint8 size,uint8 mode);	//显示字符串
void tft_lcd_show_num(int16 x,int16 y,int num,uint16 font_color, uint16 background_color,uint8 size, uint8 mode);	//显示数字
void tft_lcd_show_float(int16 x, int16 y, float num, uint8 precision, uint16 font_color, uint16 background_color, uint8 size, uint8 mode);	//显示小数
void tft_lcd_show_font12(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode);	//显示12x12汉字
void tft_lcd_show_font16(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode);	//显示16x16汉字
void tft_lcd_show_font24(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode);	//显示24x24汉字
void tft_lcd_show_font32(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode);	//显示32x32汉字
void tft_lcd_draw_bmp16(uint16 x0,uint16 y0,uint16 w,uint16 h,const uint8 *p);	//显示bmp图像

//颜色
#define TFT_LCD_WHITE         	 0xFFFF
#define TFT_LCD_BLACK         	 0x0000	  
#define TFT_LCD_BLUE             0x001F  
#define TFT_LCD_BRED             0XF81F
#define TFT_LCD_GBLUE            0X07FF
#define TFT_LCD_RED           	 0xF800
#define TFT_LCD_MAGENTA       	 0xF81F
#define TFT_LCD_GREEN         	 0x07E0
#define TFT_LCD_CYAN          	 0x7FFF
#define TFT_LCD_YELLOW        	 0xFFE0
#define TFT_LCD_BROWN            0XBC40 //棕色
#define TFT_LCD_BRRED            0XFC07 //棕红色
#define TFT_LCD_GRAY             0X8430 //灰色
#define TFT_LCD_DARKBLUE      	 0X01CF	//深蓝色
#define TFT_LCD_LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define TFT_LCD_GRAYBLUE       	 0X5458 //灰蓝色
#define TFT_LCD_LIGHTGREEN     	 0X841F //浅绿色
#define TFT_LCD_LGRAY            0XC618 //浅灰色(PANNEL),窗体背景色
#define TFT_LCD_LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define TFT_LCD_LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


//LCD的画笔颜色和背景色	   
uint16 _tft_lcd_point_color = 0x0000;	//画笔颜色
uint16 _tft_lcd_back_color = 0xFFFF;  	//背景色 
//管理LCD重要参数
//默认为竖屏

//========================================================================
// 描述: LCD写入一个字节数据.
// 参数: HVAL:写入的字节数据.
// 返回: none.
//========================================================================
#if TFT_LCD_USE_8BIT_MODEL
void tft_lcd_write(uint8 HVAL)
{
	TFT_LCD_CS = 0;
	TFT_LCD_DATAPORTH = HVAL;
	TFT_LCD_WR = 0;	
	_nop_(); _nop_();
	_nop_(); _nop_();
	_nop_(); _nop_();
	_nop_(); _nop_();
	_nop_(); _nop_();
	TFT_LCD_WR = 1;
	TFT_LCD_DATAPORTH = 0xff;
	TFT_LCD_CS = 1;
}
#else
void tft_lcd_write(uint8 HVAL,uint8 LVAL)
{
	TFT_LCD_CS = 0;
	TFT_LCD_WR = 0;	
	TFT_LCD_DATAPORTH = HVAL;
	TFT_LCD_DATAPORTL = LVAL;
	TFT_LCD_WR = 1;
	TFT_LCD_CS = 1;
}
#endif

//========================================================================
// 描述: LCD读取16位数据.
// 参数: none.
// 返回: 读取到的16位数据.
//========================================================================
uint16 tft_lcd_read()
{
	uint16 d;
	TFT_LCD_CS = 0;
	TFT_LCD_RD = 0;
	delay1us(); //delay 1 us
	d = TFT_LCD_DATAPORTH;
	d = (d<<8);
	TFT_LCD_RD = 1;
  	TFT_LCD_CS = 1;
	return d;
}

//========================================================================
// 描述: LCD写入命令.
// 参数: Reg:写入的命令.
// 返回: none.
//========================================================================
void tft_lcd_wr_reg(uint16 Reg)	 
{	
	TFT_LCD_RS=0;
	#if TFT_LCD_USE_8BIT_MODEL
	tft_lcd_write(Reg&0xFF);
	#else
	lcd_write((Reg>>8)&0xFF,Reg&0xFF);
	#endif
} 

//========================================================================
// 描述: LCD写入数据.
// 参数: Data:写入的数据.
// 返回: none.
//========================================================================
void tft_lcd_wr_data(uint16 Data)
{
	TFT_LCD_RS=1;
	#if TFT_LCD_USE_8BIT_MODEL
	tft_lcd_write(Data&0xFF);
	#else
	lcd_write((Data>>8)&0xFF,Data&0xFF);
	#endif
}


//========================================================================
// 描述: LCD读取数据.
// 参数: none.
// 返回: 读取到的数据.
//========================================================================
uint16 tft_lcd_rd_data()
{
	TFT_LCD_RS=1;
	#if TFT_LCD_USE_8BIT_MODEL
	return (tft_lcd_read()>>8);
	#else
	return tft_lcd_read();
	#endif
}

//========================================================================
// 描述: LCD写入16位数据.
// 参数: Data:写入的数据.
// 返回: none.
//========================================================================
void tft_lcd_wr_data_16bit(uint16 Data)
{
	TFT_LCD_RS=1;
	#if TFT_LCD_USE_8BIT_MODEL
	tft_lcd_write((Data>>8)&0xFF);
	tft_lcd_write(Data&0xFF);
	#else
	tft_lcd_write((Data>>8)&0xFF,Data&0xFF);
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
uint16 tft_lcd_rd_data_16bit()
{
	uint16 r,g,b;
	TFT_LCD_RS = 1;
	TFT_LCD_CS = 0;
	
	//dummy data
	TFT_LCD_RD = 0;
	delay1us(); //delay 1us
	r = TFT_LCD_DATAPORTH;
	r = (r<<8);
	TFT_LCD_RD = 1;
	
	//8bit:red data
	//16bit:red and green data
	TFT_LCD_RD = 0;
	delay1us(); //delay 1us
	r = TFT_LCD_DATAPORTH;
	r = (r<<8);
	TFT_LCD_RD = 1;
	
	//8bit:green data
	//16bit:blue data
	TFT_LCD_RD = 0;
	delay1us(); //delay 1us
	g = TFT_LCD_DATAPORTH;
	g = (g<<8);
	TFT_LCD_RD = 1;
	
	#if TFT_LCD_USE_8BIT_MODEL	
	TFT_LCD_RD = 0;
	delay1us(); //delay 1us
	b = TFT_LCD_DATAPORTH;
	b = (b<<8);
	TFT_LCD_RD = 1;
	r >>= 8;
	g >>= 8;
	b >>= 8;
	#else
	b = g>>8;
	r = r>>8;
	g = r&0xFF;
	#endif
	TFT_LCD_CS = 1;
	return tft_color_to_565(r, g, b);
}

//========================================================================
// 描述: LCD写寄存器.
// 参数: LCD_Reg:写的寄存器地址; LCD_RegValue:写入的值.
// 返回: none.
//========================================================================
void tft_lcd_writereg(uint16 LCD_Reg, uint16 LCD_RegValue)
{
  	tft_lcd_wr_reg(LCD_Reg);
	tft_lcd_wr_data(LCD_RegValue);
}

//========================================================================
// 描述: LCD读寄存器.
// 参数: LCD_Reg:读的寄存器地址; Rval:读取到的值; n:需要连续读取的数量.
// 返回: none.
//========================================================================
void tft_lcd_readreg(uint8 LCD_Reg, uint8* Rval, int n)
{
	tft_lcd_wr_reg(LCD_Reg);
	TFT_LCD_DATAPORTH = 0xFF; //拉高P6

	//TFT_LCD_DATAPORTL = 0xFF; //拉高P2

	TFT_LCD_DATAPORTH_IN;	//P6口设置成输入口

	while (n--)
	{
		*(Rval++) = tft_lcd_rd_data();
	}

	TFT_LCD_DATAPORTH_OUT;		//P6口设置为推挽输出
	TFT_LCD_DATAPORTH = 0xFF; //拉高P6
	//TFT_LCD_DATAPORTL = 0xFF; //拉高P2

}

//========================================================================
// 描述: LCD写GRAM.
// 参数: none.
// 返回: none.
//========================================================================
void tft_lcd_writeram_prepare()
{
	tft_lcd_wr_reg(lcddev.wramcmd);
}

//========================================================================
// 描述: LCD读GRAM.
// 参数: none.
// 返回: none.
//========================================================================
void tft_lcd_readram_prepare()
{
	tft_lcd_wr_reg(lcddev.rramcmd);
}

//========================================================================
// 描述: LCD清屏.
// 参数: color:清屏的颜色.
// 返回: none.
//========================================================================
void tft_lcd_clear(uint16 color)
{
	uint16 i, j;
	tft_lcd_set_windows(0, 0, lcddev.width - 1, lcddev.height - 1);
	for (i = 0; i < lcddev.width; i++)
	{
		for (j = 0; j < lcddev.height; j++)
		{
			tft_lcd_wr_data_16bit(color);
		}
	}
}

//========================================================================
// 描述: LCD画点.
// 参数: x,y: 坐标.
// 返回: none.
//========================================================================
void tft_lcd_draw_point(uint16 x, uint16 y)
{
	tft_lcd_set_windows(x, y, x, y);//设置光标位置 
	tft_lcd_wr_data_16bit(_tft_lcd_point_color);
}

//========================================================================
// 描述: LCD读点.
// 参数: x,y: 坐标.
// 返回: 读到的颜色值.
//========================================================================
uint16 tft_lcd_read_point(uint16 x, uint16 y)
{
	uint16 color;
	if (x >= lcddev.width || y >= lcddev.height)
	{
		return 0;	//超过了范围,直接返回	
	}
	tft_lcd_set_cursor(x, y);//设置光标位置 
	tft_lcd_readram_prepare();
	TFT_LCD_DATAPORTH = 0xFF; //拉高P6
	//TFT_LCD_DATAPORTL = 0xFF; //拉高P2

	TFT_LCD_DATAPORTH_IN;	//P6口设置成输入口
	color = tft_lcd_rd_data_16bit();
	
	TFT_LCD_DATAPORTH_OUT;//P6口设置成推挽输出

	TFT_LCD_DATAPORTH = 0xFF; //拉高P0
	//TFT_LCD_DATAPORTL = 0xFF; //拉高P2

	return color;
}

//========================================================================
// 描述: LCD的GPIO配置.
// 参数: none.
// 返回: none.
//========================================================================
void tft_lcd_set_gpio()
{
	TFT_LCD_RD_OUT;	  //设置为推挽输出
	TFT_LCD_WR_OUT;
	TFT_LCD_RS_OUT;
	TFT_LCD_CS_OUT;
	TFT_LCD_RESET_OUT;
	TFT_LCD_DATAPORTH_OUT;//P6口设置成推挽输出
}

//========================================================================
// 描述: LCD复位.
// 参数: none.
// 返回: none.
//========================================================================
void tft_lcd_reset()
{
	delay(50);
	TFT_LCD_RESET = 0;
	delay(50);
	TFT_LCD_RESET = 1;
	delay(50);
}

//========================================================================
// 描述: LCD初始化.
// 参数: none.
// 返回: none.
//========================================================================
void tft_lcd_init()
{
	tft_lcd_set_gpio();
	tft_lcd_reset(); //初始化之前复位

	//*************2.4inch ILI9341初始化**********//	
	tft_lcd_wr_reg(0x01);		//softreset
	delay(50); 
	tft_lcd_wr_reg(0x28);		//displayoff

	tft_lcd_wr_reg(0xF6); 		//Interface Control needs EXTC=1 MV_EOR=0, TM=0, RIM=0
	tft_lcd_wr_data(0x01); 
	tft_lcd_wr_data(0x01); 
	tft_lcd_wr_data(0x00); 


	tft_lcd_wr_reg(0xCF);  	 	//Power Control B [00 81 30]
	tft_lcd_wr_data(0x00); 
	tft_lcd_wr_data(0x81); 
	tft_lcd_wr_data(0X30);

	tft_lcd_wr_reg(0xED);  		//Power On Seq [55 01 23 01]
	tft_lcd_wr_data(0x64); 
	tft_lcd_wr_data(0x03); 
	tft_lcd_wr_data(0X12); 
	tft_lcd_wr_data(0X81); 

	tft_lcd_wr_reg(0xE8);  		//Driver Timing A [04 11 7A]
	tft_lcd_wr_data(0x85); 
	tft_lcd_wr_data(0x10); 
	tft_lcd_wr_data(0x78);

	tft_lcd_wr_reg(0xCB);  		//Power Control A [39 2C 00 34 02]
	tft_lcd_wr_data(0x39); 
	tft_lcd_wr_data(0x2C); 
	tft_lcd_wr_data(0x00); 
	tft_lcd_wr_data(0x34); 
	tft_lcd_wr_data(0x02); 

	tft_lcd_wr_reg(0xF7);  		//Pump Ratio [10]
	tft_lcd_wr_data(0x20);

	tft_lcd_wr_reg(0xEA);  		//Driver Timing B [66 00]
	tft_lcd_wr_data(0x00); 
	tft_lcd_wr_data(0x00); 

	tft_lcd_wr_reg(0xB0);    	//RGB Signal [00]
	tft_lcd_wr_data(0x00); 

	tft_lcd_wr_reg(0xB4);    	 //Inversion Control [02] .kbv NLA=1, NLB=1, NLC=1
	tft_lcd_wr_data(0x00); 

	tft_lcd_wr_reg(0xC0);    //Power Control 1 [26]
	tft_lcd_wr_data(0x21);   

	tft_lcd_wr_reg(0xC1);    //Power Control 2 [00]
	tft_lcd_wr_data(0x11);    

	tft_lcd_wr_reg(0xC5);    //VCM control 
	tft_lcd_wr_data(0x3F); 	 //3F
	tft_lcd_wr_data(0x3C); 	 //3C

	tft_lcd_wr_reg(0xC7);    //VCM control2 
	tft_lcd_wr_data(0XB5); 

	tft_lcd_wr_reg(0x36);    // Memory Access Control 
	tft_lcd_wr_data(0x48); 

	tft_lcd_wr_reg(0x3A);    //Pixel read=565, write=565.
	tft_lcd_wr_data(0x55); 	

	tft_lcd_wr_reg(0xB1);  	 //Frame Control [00 1B]
	tft_lcd_wr_data(0x00);   
	tft_lcd_wr_data(0x1B); 

	tft_lcd_wr_reg(0xB6);    // Display Function Control 
	tft_lcd_wr_data(0x0A); 
	tft_lcd_wr_data(0xA2); 

	tft_lcd_wr_reg(0xF2);    //Enable 3G [02]
	tft_lcd_wr_data(0x00); 

	tft_lcd_wr_reg(0x26);    //Gamma Set [01]
	tft_lcd_wr_data(0x01); 

	tft_lcd_wr_reg(0xE0);    //Set Gamma 
	tft_lcd_wr_data(0x0F); 
	tft_lcd_wr_data(0x26); 
	tft_lcd_wr_data(0x24); 
	tft_lcd_wr_data(0x0B); 
	tft_lcd_wr_data(0x0E); 
	tft_lcd_wr_data(0x09); 
	tft_lcd_wr_data(0x54); 
	tft_lcd_wr_data(0XA8); 
	tft_lcd_wr_data(0x46); 
	tft_lcd_wr_data(0x0C); 
	tft_lcd_wr_data(0x17); 
	tft_lcd_wr_data(0x09); 
	tft_lcd_wr_data(0x0F); 
	tft_lcd_wr_data(0x07); 
	tft_lcd_wr_data(0x00); 	

	tft_lcd_wr_reg(0XE1);    //Set Gamma 
	tft_lcd_wr_data(0x00); 
	tft_lcd_wr_data(0x19); 
	tft_lcd_wr_data(0x1B); 
	tft_lcd_wr_data(0x04); 
	tft_lcd_wr_data(0x10); 
	tft_lcd_wr_data(0x07); 
	tft_lcd_wr_data(0x2A); 
	tft_lcd_wr_data(0x47); 
	tft_lcd_wr_data(0x39); 
	tft_lcd_wr_data(0x03); 
	tft_lcd_wr_data(0x06); 
	tft_lcd_wr_data(0x06); 
	tft_lcd_wr_data(0x30); 
	tft_lcd_wr_data(0x38); 
	tft_lcd_wr_data(0x0F); 

	tft_lcd_wr_reg(0x2B); 
	tft_lcd_wr_data(0x00);
	tft_lcd_wr_data(0x00);
	tft_lcd_wr_data(0x01);
	tft_lcd_wr_data(0x3f);

	tft_lcd_wr_reg(0x2A); 
	tft_lcd_wr_data(0x00);
	tft_lcd_wr_data(0x00);
	tft_lcd_wr_data(0x00);
	tft_lcd_wr_data(0xef);	

	tft_lcd_wr_reg(0xB7); 
	tft_lcd_wr_data(0x07);

	tft_lcd_wr_reg(0x11); //Exit Sleep
	delay(150);
	tft_lcd_wr_reg(0x29); //display on
	lcddev.id = tft_lcd_read_id();	//获取id
	tft_lcd_direction(0);//设置LCD显示方向 
	//	LCD_BL=1;//点亮背光	 	 
}

//========================================================================
// 描述: LCD设置窗口.
// 参数: x0,y0:起始坐标; x1,y1:终点坐标.
// 返回: none.
//========================================================================
void tft_lcd_set_windows(uint16 x0, uint16 y0, uint16 x1, uint16 y1)
{
	tft_lcd_wr_reg(lcddev.setxcmd);
	tft_lcd_wr_data(x0 >> 8);
	tft_lcd_wr_data(0x00FF & x0);
	tft_lcd_wr_data(x1 >> 8);
	tft_lcd_wr_data(0x00FF & x1);

	tft_lcd_wr_reg(lcddev.setycmd);
	tft_lcd_wr_data(y0 >> 8);
	tft_lcd_wr_data(0x00FF & y0);
	tft_lcd_wr_data(y1 >> 8);
	tft_lcd_wr_data(0x00FF & y1);

	tft_lcd_writeram_prepare();	//开始写入GRAM						
}

//========================================================================
// 描述: LCD设置光标.
// 参数: x:横坐标; y:纵坐标;
// 返回: none.
//========================================================================
void tft_lcd_set_cursor(uint16 x, uint16 y)
{
	tft_lcd_set_windows(x, y, x, y);
}

//========================================================================
// 描述: 设置显示方向.
// 参数: direction:0~3;
// 返回: none.
//========================================================================
void tft_lcd_direction(uint8 direction)
{
	lcddev.setxcmd = 0x2A;
	lcddev.setycmd = 0x2B;
	lcddev.wramcmd = 0x2C;
	lcddev.rramcmd = 0x2E;
	switch (direction) {
	case 0:
		lcddev.width = TFT_LCD_LCD_W;
		lcddev.height = TFT_LCD_LCD_H;
		if(lcddev.id == 0x9341|lcddev.id == 0x9340)
			tft_lcd_writereg(0x36,   (1<<3)|(0<<6)); 
		else tft_lcd_writereg(0x36,   (0<<3)|(1<<6)); 
		break;
	case 1:
		lcddev.width = TFT_LCD_LCD_H;
		lcddev.height = TFT_LCD_LCD_W;
		if(lcddev.id == 0x9341 | lcddev.id == 0x9340)
			tft_lcd_writereg(0x36, (1 << 3) | (1 << 7) | (1 << 5) | (1 << 4) | (0 << 6));
		else 	tft_lcd_writereg(0x36, (0 << 3) | (1 << 7) | (1 << 5) | (1 << 4) | (1 << 6));
		break;
	case 2:
		lcddev.width = TFT_LCD_LCD_W;
		lcddev.height = TFT_LCD_LCD_H;
		if(lcddev.id == 0x9341 | lcddev.id == 0x9340)
			tft_lcd_writereg(0x36, (1 << 3) | (1 << 7) | (1 << 4)| (1 << 6));
		else tft_lcd_writereg(0x36, (0 << 3) | (1 << 7) | (1 << 4)| (0 << 6));
		break;
	case 3:
		lcddev.width = TFT_LCD_LCD_H;
		lcddev.height = TFT_LCD_LCD_W;
		if(lcddev.id == 0x9341 | lcddev.id == 0x9340)
			tft_lcd_writereg(0x36, (1 << 3) | (1 << 5)|(1<<6));
		else tft_lcd_writereg(0x36, (0 << 3) | (1 << 5)); 
		break;
	default:break;
	}
}

//========================================================================
// 描述: LCD读取ID.
// 参数: none;
// 返回: 读取到的ID号.
//========================================================================
uint16 tft_lcd_read_id()
{
	uint8 val[4] = { 0 };
	tft_lcd_readreg(0xD3, val, 4);
	return (val[2] << 8) | val[3];
}

//========================================================================
// 描述: LCD填充.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; color:颜色
// 返回: none.
//========================================================================
void tft_lcd_fill(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color)
{
	uint16 i, j;
	uint16 width = x1 - x0 + 1; 		//得到填充的宽度
	uint16 height = y1 - y0 + 1;		//高度
	tft_lcd_set_windows(x0, y0, x1, y1);//设置显示窗口
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
			tft_lcd_wr_data_16bit(color);	//写入数据 	 
	}
	tft_lcd_set_windows(0, 0, lcddev.width - 1, lcddev.height - 1);//恢复窗口设置为全屏
}

//========================================================================
// 描述: LCD绘制线段.
// 参数: x1，y1:起始坐标; x2,y2:终止坐标;color:颜色.
// 返回: none.
//========================================================================
void tft_lcd_draw_line(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color)
{
	int t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	uint16 tem_color = _tft_lcd_point_color;
	_tft_lcd_point_color = color;
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
		tft_lcd_draw_point(uRow, uCol);//画点 
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
	_tft_lcd_point_color = tem_color;
}

//========================================================================
// 描述: LCD绘制一条粗线.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; size:线的粗细(0~2); color:颜色.
// 返回: none.
//========================================================================
void tft_lcd_draw_line1(uint16 x0,uint16 y0,uint16 x1,uint16 y1,uint8 size,uint16 color)
{
	int t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	uint16 tem_color = _tft_lcd_point_color;
	_tft_lcd_point_color = color;

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
		if(size==0)tft_lcd_draw_point(uRow,uCol);//画点
		if(size==1)
		{
			tft_lcd_draw_point(uRow,uCol);   
			tft_lcd_draw_point(uRow+1,uCol);    
			tft_lcd_draw_point(uRow,uCol+1);    
			tft_lcd_draw_point(uRow+1,uCol+1);   
		}
		if(size==2)
		{
			tft_lcd_draw_point(uRow,uCol);
			tft_lcd_draw_point(uRow+1,uCol);
			tft_lcd_draw_point(uRow,uCol+1);
			tft_lcd_draw_point(uRow+1,uCol+1);
			tft_lcd_draw_point(uRow-1,uCol+1);
			tft_lcd_draw_point(uRow+1,uCol-1);
			tft_lcd_draw_point(uRow-1,uCol-1);
			tft_lcd_draw_point(uRow-1,uCol);
			tft_lcd_draw_point(uRow,uCol-1);
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
	_tft_lcd_point_color = tem_color; 				 
}

//========================================================================
// 描述: LCD绘制矩形框.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; color:颜色;
// 返回: none.
//========================================================================
void tft_lcd_draw_rectangle(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color)
{
	tft_lcd_draw_line(x0, y0, x1, y0,color);
	tft_lcd_draw_line(x0, y0, x0, y1,color);
	tft_lcd_draw_line(x0, y1, x1, y1,color);
	tft_lcd_draw_line(x1, y0, x1, y1,color);
}

//========================================================================
// 描述: 在指定位置填充矩形.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; color:颜色;
// 返回: none.
//========================================================================
void tft_lcd_fill_rectangle(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 color)
{
	tft_lcd_fill(x0, y0, x1, y1, color);
}

//========================================================================
// 描述: 画点.
// 参数: x，y:坐标; color:颜色值;
// 返回: none.
//========================================================================
void tft_gui_drawpoint(uint16 x, uint16 y, uint16 color)
{
	tft_lcd_set_cursor(x, y);//设置光标位置 
	tft_lcd_wr_data_16bit(color);
}

//========================================================================
// 描述: 对称圆绘制算法(内部调用).
// 参数: xc:圆心的x坐标; yc:圆心的y坐标; x:相对于圆心的x坐标; y:相对于圆心的y坐标; 
//		 color:圆圈的颜色值
// 返回: none.
//========================================================================
void tft_draw_circle_8(int xc, int yc, int x, int y, uint16 color)
{
	tft_gui_drawpoint(xc + x, yc + y, color);

	tft_gui_drawpoint(xc - x, yc + y, color);

	tft_gui_drawpoint(xc + x, yc - y, color);

	tft_gui_drawpoint(xc - x, yc - y, color);

	tft_gui_drawpoint(xc + y, yc + x, color);

	tft_gui_drawpoint(xc - y, yc + x, color);

	tft_gui_drawpoint(xc + y, yc - x, color);

	tft_gui_drawpoint(xc - y, yc - x, color);
}

//========================================================================
// 描述: 在指定位置绘制指定大小的圆.
// 参数: xc:圆心的x坐标; yc:圆心的y坐标; color:圆的颜色; r:圆半径; fill:1-填充，0-不填充;
// 返回: none.
//========================================================================
void tft_lcd_draw_circle(int xc, int yc, int r, uint16 color, uint8 fill)
{
	int x = 0, y = r, yi, d;

	d = 3 - 2 * r;

	if (fill)
	{
		// 如果填充（画实心圆）
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				tft_draw_circle_8(xc, yc, x, yi, color);

			if (d < 0) {
				d = d + 4 * x + 6;
			}
			else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
	else
	{
		// 如果不填充（画空心圆）
		while (x <= y) {
			tft_draw_circle_8(xc, yc, x, y, color);
			if (d < 0) {
				d = d + 4 * x + 6;
			}
			else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}

//========================================================================
// 描述: 在指定位置绘制一个三角形.
// 参数: x0,y0,x1,y1,x2,y2:三角形三个端点的坐标; color:颜色
// 返回: none.
//========================================================================
void tft_lcd_draw_triangel(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color)
{
	tft_lcd_draw_line(x0, y0, x1, y1,color);
	tft_lcd_draw_line(x1, y1, x2, y2,color);
	tft_lcd_draw_line(x2, y2, x0, y0,color);
}

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
void tft_lcd_fill_triangel(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color)
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
		tft_lcd_fill(a, y0, b, y0, color);
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
		tft_lcd_fill(a, y, b, y, color);
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
		tft_lcd_fill(a, y, b, y, color);
	}
}

//========================================================================
// 描述: 在指定位置显示一个字符.
// 参数: x:起始x坐标; y:起始y坐标; chr:显示的字符; font_color:显示字符的颜色值; 
//       background_color:显示字符的背景色; size:显示字符的大小（12/16）; mode:0-无叠加，1-叠加:	  
// 返回: none.
//========================================================================
void tft_lcd_show_char(int16 x,int16 y, uint8 chr, uint16 font_color, uint16 background_color, uint8 size,uint8 mode)
{  
	uint8 temp;
	uint8 j,i;
	chr=chr-' ';						  //得到偏移后的值
	if(x < 0)
	{
		for(j=0;j<size;j++)
		{
			if(size==12)temp=asc2_1206[chr][j]; //调用1206字体
			else temp=asc2_1608[chr][j];		//调用1608字体 
			for(i=0;i<size/2;i++)
		    {   
				if(((x + i) >= 0))
				{   
					if(!mode) //非叠加方式
					{
						if(temp&0x01){
							tft_lcd_set_cursor(x+i,y+j);
							tft_lcd_wr_data_16bit(font_color);
						}
						else{
							tft_lcd_set_cursor(x+i,y+j);
							tft_lcd_wr_data_16bit(background_color); 
						}
					}
					else
					{            
						if(temp&0x01){
							tft_lcd_set_cursor(x+i,y+j);//画一个点 
							tft_lcd_wr_data_16bit(font_color);	
						}
					}
				}
				temp>>=1; 
		    }
		}	
	}
	else{
		for(j=0;j<size;j++)
		{
			if(size==12)temp=asc2_1206[chr][j]; //调用1206字体
			else temp=asc2_1608[chr][j];		//调用1608字体 
			for(i=0;i<size/2;i++)
		    {      
				if(!mode) //非叠加方式
				{
					if(temp&0x01){
						tft_lcd_set_cursor(x+i,y+j);
						tft_lcd_wr_data_16bit(font_color);
					}
					else{
						tft_lcd_set_cursor(x+i,y+j);
						tft_lcd_wr_data_16bit(background_color); 
					}
				}
				else
				{            
					if(temp&0x01){
						tft_lcd_set_cursor(x+i,y+j);//画一个点 
						tft_lcd_wr_data_16bit(font_color);	
					}
				}
				temp>>=1; 
		    }
		}	
	}
	tft_lcd_set_windows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏	   	 	  
}

//========================================================================
// 描述: 在指定位置显示字符串.
// 参数: x:起始x坐标; y:起始y坐标; p:要显示的字符串; font_color:字符串的颜色值; background_color:背景色
//       size:显示字符的大小（12或16）;  mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
void tft_lcd_show_string(int16 x,int16 y,uint8 *p,uint16 font_color, uint16 background_color,uint8 size,uint8 mode)
{            
    while((*p<='~')&&(*p>=' '))		//判断是不是非法字符	
    {       
        tft_lcd_show_char(x,y,*p,font_color,background_color,size,mode);
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
// void tft_lcd_show_num(uint16 x,uint16 y,uint32 num,uint8 len,uint16 font_color, uint16 background_color,uint8 size, uint8 mode)
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
// 				tft_lcd_show_char(x+(size/2)*t,y,' ',font_color,background_color,size,mode);
// 				continue;
// 			}else enshow=1;  
// 		}
// 	 	tft_lcd_show_char(x+(size/2)*t,y,temp+'0',font_color,background_color,size,mode); 
// 	}
// }

//========================================================================
// 描述: 在指定位置显示数字.
// 参数: x:起始x坐标; y:起始y坐标; num:要显示的数字(0~4294967295);
//       font_color:字符串的颜色值; background_color:背景色; size:显示的大小（12/16） mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
void tft_lcd_show_num(int16 x, int16 y, int num, uint16 font_color, uint16 background_color, uint8 size, uint8 mode)
{
	char men[10];
	_itoa(num,men,10);
	tft_lcd_show_string(x,y,men,font_color,background_color,size,mode);
} 

//========================================================================
// 描述: 在指定位置显示小数.
// 参数: x:起始x坐标; y:起始y坐标; num:要显示的数字;precision:精度;
//       font_color:字符串的颜色值; background_color:背景色; size:显示的大小（12/16） mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
void tft_lcd_show_float(int16 x, int16 y, float num, uint8 precision, uint16 font_color, uint16 background_color, uint8 size, uint8 mode)
{
    uint8 i;
	uint8 float_bit;
    int16 int_part;
	int_part = (int16)num/1;

    if(num>=0)			//整数部分
    {
		tft_lcd_show_num( x, y, int_part, font_color, background_color, size, mode);
        for(i=1;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
		num = num - int_part;
		tft_lcd_show_char(x+(size/2)*i, y, '.', font_color, background_color, size, mode);	//显示小数点
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				tft_lcd_show_num(x+(size/2)*(i+1), y, (int16)float_bit, font_color, background_color, size, mode);	
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				tft_lcd_show_num(x+(size/2)*(i+1), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*100)%10;
				tft_lcd_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				tft_lcd_show_num(x+(size/2)*(i+1), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*100)%10;
				tft_lcd_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*1000)%10;
				tft_lcd_show_num(x+(size/2)*(i+3), y, (int16)float_bit, font_color, background_color, size, mode);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				tft_lcd_show_num(x+(size/2)*(i+1), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				tft_lcd_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				tft_lcd_show_num(x+(size/2)*(i+3), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				tft_lcd_show_num(x+(size/2)*(i+4), y, (int16)float_bit, font_color, background_color, size, mode);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				tft_lcd_show_num(x+(size/2)*(i+1), y, (int16)float_bit, font_color, background_color, size, mode);
			break;//保留1位精度
		}
    }
    else
    {
		i = 0;
		if(int_part == 0)
		{
			tft_lcd_show_char(x, y, '-', font_color, background_color, size, mode);	//显示负号
			i++;
		}
		tft_lcd_show_num( x+(size/2)*i, y, int_part, font_color, background_color, size, mode);
        for(i=1;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
		num = -(num - int_part);
		tft_lcd_show_char( x+(size/2)*(i+1), y, '.', font_color, background_color, size, mode);
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				tft_lcd_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);	
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				tft_lcd_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*100)%10;
				tft_lcd_show_num(x+(size/2)*(i+3), y, (int16)float_bit, font_color, background_color, size, mode);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				tft_lcd_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*100)%10;
				tft_lcd_show_num(x+(size/2)*(i+3), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*1000)%10;
				tft_lcd_show_num(x+(size/2)*(i+4), y, (int16)float_bit, font_color, background_color, size, mode);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				tft_lcd_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				tft_lcd_show_num(x+(size/2)*(i+3), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				tft_lcd_show_num(x+(size/2)*(i+4), y, (int16)float_bit, font_color, background_color, size, mode);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				tft_lcd_show_num(x+(size/2)*(i+5), y, (int16)float_bit, font_color, background_color, size, mode);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				tft_lcd_show_num(x+(size/2)*(i+2), y, (int16)float_bit, font_color, background_color, size, mode);
			break;//保留1位精度
		}
    }
} 

//========================================================================
// 描述: 在指定位置显示12*12字体汉字.
// 参数: hz:汉字的指针; x:起始x坐标; y:起始y坐标; lenth: 字体的总长度 
//       font_color:显示字符的颜色值; background_color:显示字符的背景色;mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
void tft_lcd_show_font12(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode)
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
								tft_lcd_set_cursor(zz * 12+ x + j,y + i);
								tft_lcd_wr_data_16bit(font_color);
							}
							else{
								tft_lcd_set_cursor(zz * 12+ x + j,y + i);
								tft_lcd_wr_data_16bit(background_color);
							} 
						}
						else
						{
							if(hz[zz*24 + i * 2 + j/8] & (0x80>>(j % 8))){
								tft_lcd_set_cursor(zz * 12+ x + j,y + i);
								tft_lcd_wr_data_16bit(font_color);
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
							tft_lcd_set_cursor(zz * 12+ x + j,y + i);
							tft_lcd_wr_data_16bit(font_color);
						}
						else{
							tft_lcd_set_cursor(zz * 12+ x + j,y + i);
							tft_lcd_wr_data_16bit(background_color);
						}
					}
					else
					{
						if(hz[zz*24 + i * 2 + j/8] & (0x80>>(j % 8))){
							tft_lcd_set_cursor(zz * 12+ x + j,y + i);
							tft_lcd_wr_data_16bit(font_color);						
						}
					}
				}
			}
		}
	}
	tft_lcd_set_windows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏
}

//========================================================================
// 描述: 在指定位置显示16*16字体汉字.
// 参数: hz:汉字的指针; x:起始x坐标; y:起始y坐标; lenth: 字体的总长度 
//       font_color:显示字符的颜色值; background_color:显示字符的背景色;mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
void tft_lcd_show_font16(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode)
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
								tft_lcd_set_cursor(zz * 16+ x + j,y + i);
								tft_lcd_wr_data_16bit(font_color);
							}
							else{
								tft_lcd_set_cursor(zz * 16+ x + j,y + i);
								tft_lcd_wr_data_16bit(background_color);
							}
						}
						else
						{
							if(hz[zz*32 + i * 2 + j/8] & (0x80>>(j % 8))){
								tft_lcd_set_cursor(zz * 16+ x + j,y + i);
								tft_lcd_wr_data_16bit(font_color);							
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
							tft_lcd_set_cursor(zz * 16+ x + j,y + i);
							tft_lcd_wr_data_16bit(font_color);
						}
						else{
							tft_lcd_set_cursor(zz * 16+ x + j,y + i);
							tft_lcd_wr_data_16bit(background_color);
						}
					}
					else
					{
						if(hz[zz*32 + i * 2 + j/8] & (0x80>>(j % 8))){
							tft_lcd_set_cursor(zz * 16+ x + j,y + i);
							tft_lcd_wr_data_16bit(font_color);
						}
					}
				}
			}
		}
	}
	tft_lcd_set_windows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏
}
//========================================================================
// 描述: 在指定位置显示24*24字体汉字.
// 参数: hz:汉字的指针; x:起始x坐标; y:起始y坐标; lenth: 字体的总长度 
//       font_color:显示字符的颜色值; background_color:显示字符的背景色;mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
void tft_lcd_show_font24(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode)
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
								tft_lcd_set_cursor(zz * 24+ x + j,y + i);
								tft_lcd_wr_data_16bit(font_color);
							}
							else {
								tft_lcd_set_cursor(zz * 24+ x + j,y + i);
								tft_lcd_wr_data_16bit(background_color);
							}
						}
						else
						{
							if(hz[zz*72 + i * 3 + j/8] & (0x80>>(j % 8))){
								tft_lcd_set_cursor(zz * 24+ x + j,y + i);
								tft_lcd_wr_data_16bit(font_color);	
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
							tft_lcd_set_cursor(zz * 24+ x + j,y + i);
							tft_lcd_wr_data_16bit(font_color);
						}
						else{
							tft_lcd_set_cursor(zz * 24+ x + j,y + i);
							tft_lcd_wr_data_16bit(background_color);
						}
					}
					else
					{
						if(hz[zz*72 + i * 3 + j/8] & (0x80>>(j % 8))){
							tft_lcd_set_cursor(zz * 24+ x + j,y + i);
							tft_lcd_wr_data_16bit(font_color);	
						}
					}
				}
			}
		}
	}
	tft_lcd_set_windows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏
}
//========================================================================
// 描述: 在指定位置显示32*32字体汉字.
// 参数: hz:汉字的指针; x:起始x坐标; y:起始y坐标; lenth: 字体的总长度 
//       font_color:显示字符的颜色值; background_color:显示字符的背景色;mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
void tft_lcd_show_font32(uint8 lenth, uint8 *hz, int16 x, int16 y,uint16 font_color, uint16 background_color, uint8 mode)
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
								tft_lcd_set_cursor(zz * 32+ x + j,y + i);
								tft_lcd_wr_data_16bit(font_color);
							}
							else{ 
								tft_lcd_set_cursor(zz * 32+ x + j,y + i);
								tft_lcd_wr_data_16bit(background_color);
							}
						}
						else
						{
							if (hz[zz * 128 + j * 4 + i/ 8] & (0x80 >> (i % 8))){
								tft_lcd_set_cursor(zz * 32+ x + j,y + i);
								tft_lcd_wr_data_16bit(font_color);
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
			tft_lcd_set_windows(x+32*zz,y,x+32*zz+32-1,y+32-1);
			for(i = 0;i < 32;i++)
			{
				for(j = 0;j < 32;j++)
				{
					if((zz * 32 + j + x -32) > 239)return;
					if((y + i -32) > 339)return;

					if(!mode)		//无叠加
					{
						if (hz[zz * 128 + j * 4 + i/ 8] & (0x80 >> (i % 8))){
							tft_lcd_set_cursor(zz * 32+ x + j,y + i);
							tft_lcd_wr_data_16bit(font_color);
						}
						else{
							tft_lcd_set_cursor(zz * 32+ x + j,y + i);
							tft_lcd_wr_data_16bit(background_color);
						}
					}
					else
					{
						if (hz[zz * 128 + j * 4 + i/ 8] & (0x80 >> (i % 8))){
							tft_lcd_set_cursor(zz * 32+ x + j,y + i);
							tft_lcd_wr_data_16bit(font_color);
						}
					}
				}
			}
		}
	}
	tft_lcd_set_windows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏
}

//========================================================================
// 描述: 显示16位的BMP图像.
// 参数: x0:起始x坐标; y0:起始y坐标;w:图片的宽度; h:图片的高度; p:图像数组的起始地址
// 返回: none.
//========================================================================
void tft_lcd_draw_bmp16(uint16 x,uint16 y,uint16 w,uint16 h,const uint8 *p)
{
  	int i; 
	uint8 picH,picL; 
	tft_lcd_set_windows(x,y,x+w-1,y+h-1);				//窗口设置
    for(i=0;i<40*40;i++)
	{	
	 	picL=*(p+i*2);		//数据低位在前
		picH=*(p+i*2+1);				
		tft_lcd_wr_data_16bit(picH<<8|picL);  						
	}	
	tft_lcd_set_windows(0,0,lcddev.width-1,lcddev.height-1);//恢复显示窗口为全屏
}

#endif  //tftlcd.h





