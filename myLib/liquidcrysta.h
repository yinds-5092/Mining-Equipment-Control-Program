/*************  技术支持与购买说明    **************
产品主页：http://twen51.com/
淘宝搜索：天问51，可购买基础版、带彩屏标准版、旗舰版
技术支持QQ群一：1138055784
******************************************/

#ifndef __LIQUIDCRYSTA_H
#define __LIQUIDCRYSTA_H


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if defined (_C51)

#include "C51_delay.h"
#include "C51_softiic.h"

#elif defined (_STC12)
#include "STC12_delay.h"
#include "STC12_softiic.h"

#elif defined (_STC15)
#include "STC15_delay.h"
#include "STC15_softiic.h"

#else
#include "delay.h"
#include "softiic.h"

#endif

#ifndef  SOFTIIC_SCL
#define  SOFTIIC_SCL 		 P1_5
#endif

#ifndef  SOFTIIC_SCL_OUT
#define  SOFTIIC_SCL_OUT       {P1M1|=0x20;P1M0|=0x20;}   //开漏输出
#endif

#ifndef  SOFTIIC_SDA
#define  SOFTIIC_SDA 		  P1_4
#endif

#ifndef	 SOFTIIC_SDA_IN 
#define  SOFTIIC_SDA_IN        {P1M1|=0x10;P1M0&=~0x10;}  //INPUT高阻输入
#endif

#ifndef	 SOFTIIC_SDA_OUT
#define  SOFTIIC_SDA_OUT       {P1M1|=0x10;P1M0|=0x10;}  //开漏输出
#endif

#ifndef LIQUIDCRYSTA_ADDR
#define	LIQUIDCRYSTA_ADDR		0x3B	//设备地址
#endif

// commands
#define LIQUIDCRYSTA_CLEARDISPLAY 0x01
#define LIQUIDCRYSTA_RETURNHOME 0x02
#define LIQUIDCRYSTA_ENTRYMODESET 0x04
#define LIQUIDCRYSTA_DISPLAYCONTROL 0x08
#define LIQUIDCRYSTA_CURSORSHIFT 0x10
#define LIQUIDCRYSTA_FUNCTIONSET 0x20
#define LIQUIDCRYSTA_SETCGRAMADDR 0x40
#define LIQUIDCRYSTA_SETDDRAMADDR 0x80

// flags for display entry mode
#define LIQUIDCRYSTA_ENTRYRIGHT 0x00
#define LIQUIDCRYSTA_ENTRYLEFT 0x02
#define LIQUIDCRYSTA_ENTRYSHIFTINCREMENT 0x01
#define LIQUIDCRYSTA_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LIQUIDCRYSTA_DISPLAYON 0x04
#define LIQUIDCRYSTA_DISPLAYOFF 0x00
#define LIQUIDCRYSTA_CURSORON 0x02
#define LIQUIDCRYSTA_CURSOROFF 0x00
#define LIQUIDCRYSTA_BLINKON 0x01
#define LIQUIDCRYSTA_BLINKOFF 0x00

// flags for display/cursor shift
#define LIQUIDCRYSTA_DISPLAYMOVE 0x08
#define LIQUIDCRYSTA_CURSORMOVE 0x00
#define LIQUIDCRYSTA_MOVERIGHT 0x04
#define LIQUIDCRYSTA_MOVELEFT 0x00

// flags for function set
#define LIQUIDCRYSTA_8BITMODE 0x10
#define LIQUIDCRYSTA_4BITMODE 0x00
#define LIQUIDCRYSTA_2LINE 0x08
#define LIQUIDCRYSTA_1LINE 0x00
#define LIQUIDCRYSTA_5x10DOTS 0x04
#define LIQUIDCRYSTA_5x8DOTS 0x00

// flags for backlight control
#define LIQUIDCRYSTA_BACKLIGHT 0x08
#define LIQUIDCRYSTA_NOBACKLIGHT 0x00

#define LIQUIDCRYSTA_EN 0x04  // Enable bit
#define LIQUIDCRYSTA_RW 0x02  // Read/Write bit
#define LIQUIDCRYSTA_RS 0x01  // Register select bit

#define LIQUIDCRYSTA_LINES	16
#define LIQUIDCRYSTA_ROWS	2

uint8 _liquidcrysta_displayfunction;
uint8 _liquidcrysta_displaycontrol;
uint8 _liquidcrysta_displaymode;
uint8 _liquidcrysta_numlines;
uint8 _liquidcrysta_backlightval;

void liquidcrysta_init();	// 初始化
void liquidcrysta_show_char(uint8 x, uint8 y, uint8 chr);	//显示一个字符
void liquidcrysta_show_string(uint8 x,uint8 y,uint8 *chr);	//显示一个字符串
void liquidcrysta_show_num(int8 x,int8 y,int16 num);	//在指定位置显示数字
void liquidcrysta_show_float(uint8 x, uint8 y, float num, uint8 precision);	//显示小数


//========================================================================
// 描述: liquidcrysta模块写数据.
// 参数: 要写入的数据.
// 返回: none.
//========================================================================
static void liquidcrysta_expander_write(uint8 dat)
{
    softiic_start();
    softiic_send_byte(LIQUIDCRYSTA_ADDR<<1); 
    softiic_wait_ack();	
    softiic_send_byte(dat | _liquidcrysta_backlightval);
	softiic_wait_ack();	
    softiic_stop();
}

//========================================================================
// 描述: liquidcrysta使能.
// 参数: 要写入的数据.
// 返回: none.
//========================================================================
static void liquidcrysta_pulse_enable(uint8 dat)
{
	liquidcrysta_expander_write(dat | LIQUIDCRYSTA_EN);
	delay10us();
	liquidcrysta_expander_write(dat & ~LIQUIDCRYSTA_EN);
	delay50us();
}

//========================================================================
// 描述: liquidcrysta写4位数据.
// 参数: 要写入的数据.
// 返回: none.
//========================================================================
void liquidcrysta_write4bits(uint8 value)
{
	liquidcrysta_expander_write(value);
	liquidcrysta_pulse_enable(value);
}

//========================================================================
// 描述: liquidcrysta写入数据或者命令.
// 参数: value:要写入的字节;mode:命令或者数据.
// 返回: none.
//========================================================================
void liquidcrysta_send(uint8 value, uint8 mode)
{
	liquidcrysta_write4bits( (value&0xf0) | mode );
	liquidcrysta_write4bits( ((value<<4)&0xf0) | mode );
}

//========================================================================
// 描述: liquidcrysta清除显示，设置光标位置为零
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_clear()
{
	liquidcrysta_send(LIQUIDCRYSTA_CLEARDISPLAY, 0);
	delay(2);
}

//========================================================================
// 描述: liquidcrysta设置光标位置为零
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_home()
{
	liquidcrysta_send(LIQUIDCRYSTA_RETURNHOME, 0);
	delay(2);
}

//========================================================================
// 描述: liquidcrysta快速关闭显示器
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_no_display()
{
	_liquidcrysta_displaycontrol &= ~LIQUIDCRYSTA_DISPLAYON;
	liquidcrysta_send(LIQUIDCRYSTA_DISPLAYCONTROL | _liquidcrysta_displaycontrol, 0);	
}

//========================================================================
// 描述: liquidcrysta快速打开显示器
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_display()
{
	_liquidcrysta_displaycontrol |= LIQUIDCRYSTA_DISPLAYON;
	liquidcrysta_send(LIQUIDCRYSTA_DISPLAYCONTROL | _liquidcrysta_displaycontrol, 0);	
}

//========================================================================
// 描述: liquidcrysta关闭下划线光标
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_cursor_off()
{
	_liquidcrysta_displaycontrol &= ~LIQUIDCRYSTA_CURSORON;
	liquidcrysta_send(LIQUIDCRYSTA_DISPLAYCONTROL | _liquidcrysta_displaycontrol, 0);	
}

//========================================================================
// 描述: liquidcrysta打开下划线光标
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_cursor_on()
{
	_liquidcrysta_displaycontrol |= LIQUIDCRYSTA_CURSORON;
	liquidcrysta_send(LIQUIDCRYSTA_DISPLAYCONTROL | _liquidcrysta_displaycontrol, 0);	
}

//========================================================================
// 描述: liquidcrysta关闭光标闪烁
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_blink_off()
{
	_liquidcrysta_displaycontrol &= ~LIQUIDCRYSTA_BLINKON;
	liquidcrysta_send(LIQUIDCRYSTA_DISPLAYCONTROL | _liquidcrysta_displaycontrol, 0);	
}

//========================================================================
// 描述: liquidcrysta打开光标闪烁
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_blink_on()
{
	_liquidcrysta_displaycontrol |= LIQUIDCRYSTA_BLINKON;
	liquidcrysta_send(LIQUIDCRYSTA_DISPLAYCONTROL | _liquidcrysta_displaycontrol, 0);	
}

//========================================================================
// 描述: liquidcrysta在不改变RAM的情况下滚动显示器
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_scroll_display_left()
{
	liquidcrysta_send(LIQUIDCRYSTA_CURSORSHIFT | LIQUIDCRYSTA_DISPLAYMOVE | LIQUIDCRYSTA_MOVELEFT, 0);	
}

void liquidcrysta_scroll_display_right()
{
	liquidcrysta_send(LIQUIDCRYSTA_CURSORSHIFT | LIQUIDCRYSTA_DISPLAYMOVE | LIQUIDCRYSTA_MOVERIGHT, 0);	
}

//========================================================================
// 描述: liquidcrysta用于从左到右流动的文本
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_left_to_right()
{
	_liquidcrysta_displaymode |= LIQUIDCRYSTA_ENTRYLEFT;
	liquidcrysta_send(LIQUIDCRYSTA_ENTRYMODESET | _liquidcrysta_displaymode, 0);	
}

//========================================================================
// 描述: liquidcrysta用于从右到左流动的文本
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_right_to_left()
{
	_liquidcrysta_displaymode &= ~LIQUIDCRYSTA_ENTRYLEFT;
	liquidcrysta_send(LIQUIDCRYSTA_ENTRYMODESET | _liquidcrysta_displaymode, 0);	
}

//========================================================================
// 描述: liquidcrysta使光标向右对齐文本
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_auto_scroll()
{
	_liquidcrysta_displaymode |= LIQUIDCRYSTA_ENTRYSHIFTINCREMENT;
	liquidcrysta_send(LIQUIDCRYSTA_ENTRYMODESET | _liquidcrysta_displaymode, 0);	
}

//========================================================================
// 描述: liquidcrysta光标'左对齐'文本
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_no_auto_scroll()
{
	_liquidcrysta_displaymode &= ~LIQUIDCRYSTA_ENTRYSHIFTINCREMENT;
	liquidcrysta_send(LIQUIDCRYSTA_ENTRYMODESET | _liquidcrysta_displaymode, 0);	
}

//========================================================================
// 描述: liquidcrysta设置背光灯
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_set_black_light(uint8 new_val)
{
	if(new_val){
		_liquidcrysta_backlightval=LIQUIDCRYSTA_BACKLIGHT;
		liquidcrysta_expander_write(0);

	}else{
		_liquidcrysta_backlightval=LIQUIDCRYSTA_NOBACKLIGHT;	
		liquidcrysta_expander_write(0);
	}

}

//========================================================================
// 描述: 初始化liquidcrysta
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_begin_start(uint8 lines, uint8 dotsize)
{
	if (lines > 1) {
		_liquidcrysta_displayfunction |= LIQUIDCRYSTA_2LINE;
	}
	_liquidcrysta_numlines = lines;

	//对于一些1行显示，可以选择10像素高的字体
	if ((dotsize != 0) && (lines == 1)) {
		_liquidcrysta_displayfunction |= LIQUIDCRYSTA_5x10DOTS;
	}
	delay(50);
	delay(1000);
	liquidcrysta_write4bits( 0x03 << 4 );
	delay(4);
	liquidcrysta_write4bits( 0x03 << 4 );
	delay(4);
	liquidcrysta_write4bits( 0x02 << 4 );

	liquidcrysta_send(LIQUIDCRYSTA_FUNCTIONSET | _liquidcrysta_displayfunction, 0);
	_liquidcrysta_displaycontrol = LIQUIDCRYSTA_DISPLAYON | LIQUIDCRYSTA_CURSOROFF | LIQUIDCRYSTA_BLINKOFF;
	liquidcrysta_display();
	liquidcrysta_clear();
	_liquidcrysta_displaymode = LIQUIDCRYSTA_ENTRYLEFT | LIQUIDCRYSTA_ENTRYSHIFTDECREMENT;
	liquidcrysta_home();
}

//========================================================================
// 描述: 初始化liquidcrysta
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_init()
{ 	
	softiic_init();
	_liquidcrysta_backlightval = LIQUIDCRYSTA_BACKLIGHT;
	_liquidcrysta_displayfunction = LIQUIDCRYSTA_4BITMODE | LIQUIDCRYSTA_1LINE | LIQUIDCRYSTA_5x8DOTS;

	liquidcrysta_begin_start(LIQUIDCRYSTA_ROWS, 0);
}  

//========================================================================
// 描述: liquidcrysta设置光标位置
// 参数: none.
// 返回: none.
//========================================================================
void liquidcrysta_set_cursor(uint8 col, uint8 row)
{
	uint8 row_offsets[4];
	row_offsets[0] = 0x00;
	row_offsets[1] = 0x40;
	row_offsets[2] = 0x14;
	row_offsets[3] = 0x54;
	if ( row > _liquidcrysta_numlines ) {
		row = _liquidcrysta_numlines-1;    // we count rows starting w/0
	}	
	liquidcrysta_send(LIQUIDCRYSTA_SETDDRAMADDR | (col + row_offsets[row]), 0);
}

//========================================================================
// 描述: 显示一个字符
// 参数: x,y:起点坐标;chr:要显示的字符串
// 返回: none.
//========================================================================
void liquidcrysta_show_char(uint8 x, uint8 y, uint8 chr)
{
	liquidcrysta_set_cursor(x, y);
	liquidcrysta_send(chr, LIQUIDCRYSTA_RS);
}

//========================================================================
// 描述: 显示一个字符串
// 参数: x,y:起点坐标;chr:要显示的字符串
// 返回: none.
//========================================================================
void liquidcrysta_show_string(uint8 x,uint8 y,uint8 *chr)
{
	uint8 j=0;
	y &= 0x1;
 	x &= 0xF; 				//限制X不能大于15，Y不能大于1
	while (chr[j]!='\0')
	{		
	    liquidcrysta_show_char(x,y,chr[j]);
		x++;
		if(x>15){
		    x=0;
			y++;
		}
		j++;
	}
}

//========================================================================
// 描述: 在指定位置显示数字
// 参数: x,y:坐标; num:显示的数字; 
// 返回: none.
//========================================================================
void liquidcrysta_show_num(int8 x,int8 y,int16 num)
{
	char men[15];
#if defined (_STC16)
	sprintf(men,"%d",num);
#elif defined (_STC32G)
	sprintf(men,"%d",num);
#else
    _itoa(num,men,10);
#endif
	liquidcrysta_show_string(x,y,men);
} 

//========================================================================
// 描述: 显示小数
// 参数: x,y:起点坐标;num:要显示的数值;precision:精度.
// 返回: none.
//========================================================================
void liquidcrysta_show_float(uint8 x, uint8 y, float num, uint8 precision)
{
  uint8 i;
  uint8 float_bit;
  int16 int_part;
  int_part = (int16)num/1;

    if(num>=0)			//整数部分
    {
		liquidcrysta_show_num( x, y, int_part);
        for(i=1;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
		num = num - int_part;
		liquidcrysta_show_char((uint8)(x+i), y, '.');	//显示小数点
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				liquidcrysta_show_num((uint8)(x+i+1), y,(int16)float_bit);	
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				liquidcrysta_show_num((uint8)(x+i+1), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;
				liquidcrysta_show_num((uint8)(x+i+2), y,(int16)float_bit);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				liquidcrysta_show_num((uint8)(x+i+1), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;
				liquidcrysta_show_num((uint8)(x+i+2), y,(int16)float_bit);
				float_bit = (int32)(num*1000)%10;
				liquidcrysta_show_num((uint8)(x+i+3), y,(int16)float_bit);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				liquidcrysta_show_num((uint8)(x+i+1), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				liquidcrysta_show_num((uint8)(x+i+2), y,(int16)float_bit);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				liquidcrysta_show_num((uint8)(x+i+3), y,(int16)float_bit);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				liquidcrysta_show_num((uint8)(x+i+4), y,(int16)float_bit);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				liquidcrysta_show_num((uint8)(x+i+1), y,(int16)float_bit);	
			break;//保留1位精度
		}
    }
    else
    {
		i = 0;
		if(int_part == 0)
		{
			liquidcrysta_show_char((uint8)(x), y, '-');
			i++;
		}
		liquidcrysta_show_num( x+i, y, int_part);
        for(;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
		num = -(num - int_part);
		liquidcrysta_show_char((uint8)(x+i+1), y, '.');	//显示小数点
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				liquidcrysta_show_num((uint8)(x+i+2), y,(int16)float_bit);	
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				liquidcrysta_show_num((uint8)(x+i+2), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;
				liquidcrysta_show_num((uint8)(x+i+3), y,(int16)float_bit);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				liquidcrysta_show_num((uint8)(x+i+2), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;
				liquidcrysta_show_num((uint8)(x+i+3), y,(int16)float_bit);
				float_bit = (int32)(num*1000)%10;
				liquidcrysta_show_num((uint8)(x+i+4), y,(int16)float_bit);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				liquidcrysta_show_num((uint8)(x+i+2), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				liquidcrysta_show_num((uint8)(x+i+3), y,(int16)float_bit);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				liquidcrysta_show_num((uint8)(x+i+4), y,(int16)float_bit);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				liquidcrysta_show_num((uint8)(x+i+5), y,(int16)float_bit);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				liquidcrysta_show_num((uint8)(x+i+2), y,(int16)float_bit);	
			break;//保留1位精度
		}
    }
}  

/**************************************************************************
//========================================================================
// 描述: 显示小数
// 参数: x,y:起点坐标;num:要显示的数值 len: 数字的位数;  size:字体大小
// 返回: none.
//========================================================================
void oled_showfloat(uint8 x,uint8 y,uint16 num,uint8 len,uint8 size)
{         	
    uint8 temp[10];
    sprintf(temp,"%.3f",num);
    oled_show_string(x,y,(u8 *)temp,size)
} 
*****************************************************************************/


#endif  //liquidcrysta.h
