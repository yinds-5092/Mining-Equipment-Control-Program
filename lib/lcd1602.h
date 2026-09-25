/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef __LCD1602_H
#define __LCD1602_H
#include <string.h>
#include <stdio.h>
#include <STC8HX.h>
#include "delay.h"
#include <stdlib.h>

#define LCD_BUSY        0x80    //用于检测LCD状态字中的LCD_BUSY标识

// IO引脚定义
#ifndef LCD1602_RS
#define LCD1602_RS 		    P1_3  
#endif

#ifndef LCD1602_RS_OUT
#define LCD1602_RS_OUT 	{P1M1&=~0x08;P1M0|=0x08;}   //推挽输出
#endif

#ifndef LCD1602_RW
#define LCD1602_RW 		    P1_0
#endif

#ifndef LCD1602_RW_OUT
#define LCD1602_RW_OUT 	{P1M1&=~0x01;P1M0|=0x01;}   //推挽输出
#endif

#ifndef	LCD1602_E
#define LCD1602_E 		    P1_1
#endif

#ifndef LCD1602_E_OUT
#define LCD1602_E_OUT 	{P1M1&=~0x02;P1M0|=0x02;}   //推挽输出
#endif 

#ifndef LCD1602_Data
#define LCD1602_Data        P6
#endif
 
#ifndef LCD1602_Data_OUT
#define LCD1602_Data_OUT  {P6M1=0x00;P6M0=0xff;}      //推挽输出
#endif


void lcd1602_init();			//1602初始化							
void lcd1602_show_char(uint8 x, uint8 y, char c);	//1602显示一个字符
void lcd1602_show_string(uint8 x, uint8 y, uint8 *str);	//1602显示字符串
void lcd1602_show_num(uint8 x,uint8 y,int num);	//1602显示数字
void lcd1602_show_float(uint8 x, uint8 y, float num, uint8 precision);	//显示小数
void lcd1602_clear();//1602清屏
//========================================================================
// 描述: LCD1602读状态
// 参数: none.
// 返回: none.
//========================================================================
uint8 lcd1602_read_status()
{
 	LCD1602_Data = 0xFF; 
 	LCD1602_RS = 0;
 	LCD1602_RW = 1;
 	LCD1602_E = 0;
 	LCD1602_E = 0;
 	LCD1602_E = 1;
 	while (LCD1602_Data & LCD_BUSY); //检测忙信号
 	return(LCD1602_Data);
}

//========================================================================
// 描述: LCD1602写数据
// 参数: dat:写的数据.
// 返回: none.
//========================================================================
void lcd1602_write_data(uint8 dat)
{
	LCD1602_E = 0;	 
	LCD1602_RS = 1;	
	LCD1602_RW = 0;	

	LCD1602_Data = dat;	
	delay(1);

	LCD1602_E = 1;
	delay(5);
	LCD1602_E = 0;

	LCD1602_Data = dat << 4; 
	delay(1);

	LCD1602_E = 1;
	delay(5);
	LCD1602_E = 0;
}

//========================================================================
// 描述: LCD1602写指令
// 参数: com:写的指令.
// 返回: none.
//========================================================================
void lcd1602_write_command(uint8 com)
{
	LCD1602_E = 0;
	LCD1602_RS = 0;	
	LCD1602_RW = 0;

	LCD1602_Data = com;	
	delay(1);

	LCD1602_E = 1;	
	delay(5);
	LCD1602_E = 0;

	LCD1602_Data = com << 4; 
	delay(1);

	LCD1602_E = 1;	 
	delay(5);
	LCD1602_E = 0;
}

//========================================================================
// 描述: LCD1602读数据
// 参数: WCLCD:写的指令; LCD_BUSYC:0,忽略忙检测，1，需要忙检测.
// 返回: none.
//========================================================================
uint8 lcd1602_read_data()
{
 	LCD1602_RS = 1; 
 	LCD1602_RW = 1;
 	LCD1602_E = 0;
 	LCD1602_E = 0;
 	LCD1602_E = 1;
 	return(LCD1602_Data);
}

//========================================================================
// 描述: 初始化LCD1602
// 参数: none.
// 返回: none.
//========================================================================
void lcd1602_init()
{
 	LCD1602_Data = 0;
    //引脚配置
    LCD1602_RS_OUT;
    LCD1602_RW_OUT;
    LCD1602_E_OUT;
    LCD1602_Data_OUT;

 	lcd1602_write_command(0x38); 	//三次模式设置，不检测忙信号
 	delay(5);
 	lcd1602_write_command(0x38);
 	delay(5); 
 	lcd1602_write_command(0x38);
 	delay(5); 

 	lcd1602_write_command(0x32);
 	lcd1602_write_command(0x28);
 	lcd1602_write_command(0x0C);
 	lcd1602_write_command(0x06);
 	lcd1602_write_command(0x01);
	lcd1602_write_command(0x80);
}

//========================================================================
// 描述: 在指定位置显示数字
// 参数: x,y:坐标; Num:显示的数字; len:长度.
// 返回: none.
//========================================================================
void lcd1602_clear()
{
	lcd1602_write_command(0x01);
	delay(2); 
} 

//========================================================================
// 描述: LCD1602按指定位置显示一个字符
// 参数: x,y:坐标; c:显示的字符.
// 返回: none.
//========================================================================
void lcd1602_show_char(uint8 x, uint8 y, char c)
{
 	y &= 0x1;
 	x &= 0xF; 				//限制X不能大于15，Y不能大于1
 	if (y) x |= 0x40; 		//当要显示第二行时地址码+0x40;
 	x |= 0x80; 			    //算出指令码
 	lcd1602_write_command(x);  //这里不检测忙信号，发送地址码
 	lcd1602_write_data(c);
}

//========================================================================
// 描述: LCD1602按指定位置显示一个字符串
// 参数: x,y:坐标; str:显示的字符串.
// 返回: none.
//========================================================================
void lcd1602_show_string(uint8 x, uint8 y, uint8 *str)
{
 	uint8 len,i;

 	len = strlen(str);
 	y &= 0x1;
 	x &= 0xF; 				//限制X不能大于15，Y不能大于1
	
	for(i=0;i<len;i++)
	{
		lcd1602_show_char(x,y,str[i]);
		x++;
	}
}

//========================================================================
// 描述: 在指定位置显示数字
// 参数: x,y:坐标; num:显示的数字; 
// 返回: none.
//========================================================================
void lcd1602_show_num(uint8 x,uint8 y,int num)
{
	char men[15];
    _itoa(num,men,10);
	lcd1602_show_string(x,y,men);
} 

//========================================================================
// 描述: 显示小数
// 参数: x,y:起点坐标;num:要显示的数值;precision:精度.
// 返回: none.
//========================================================================
void lcd1602_show_float(uint8 x, uint8 y, float num, uint8 precision)
{
  uint8 i;
  uint8 float_bit;
  int16 int_part;
  int_part = (int16)num/1;

    if(num>=0)			//整数部分
    {
		lcd1602_show_num( x, y, int_part);
        for(i=1;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
		num = num - int_part;
		lcd1602_show_char((uint8)(x+i), y, '.');	//显示小数点
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				lcd1602_show_num((uint8)(x+i+1), y,(int16)float_bit);	
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				lcd1602_show_num((uint8)(x+i+1), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;
				lcd1602_show_num((uint8)(x+i+2), y,(int16)float_bit);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				lcd1602_show_num((uint8)(x+i+1), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;
				lcd1602_show_num((uint8)(x+i+2), y,(int16)float_bit);
				float_bit = (int32)(num*1000)%10;
				lcd1602_show_num((uint8)(x+i+3), y,(int16)float_bit);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				lcd1602_show_num((uint8)(x+i+1), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				lcd1602_show_num((uint8)(x+i+2), y,(int16)float_bit);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				lcd1602_show_num((uint8)(x+i+3), y,(int16)float_bit);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				lcd1602_show_num((uint8)(x+i+4), y,(int16)float_bit);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				lcd1602_show_num((uint8)(x+i+1), y,(int16)float_bit);	
			break;//保留1位精度
		}
    }
    else
    {
		i = 0;
		if(int_part == 0)
		{
			lcd1602_show_char((uint8)(x), y, '-');
			i++;
		}
		lcd1602_show_num( x+i, y, int_part);
        for(;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
		num = -(num - int_part);
		lcd1602_show_char((uint8)(x+i+1), y, '.');	//显示小数点
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				lcd1602_show_num((uint8)(x+i+2), y,(int16)float_bit);	
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				lcd1602_show_num((uint8)(x+i+2), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;
				lcd1602_show_num((uint8)(x+i+3), y,(int16)float_bit);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				lcd1602_show_num((uint8)(x+i+2), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;
				lcd1602_show_num((uint8)(x+i+3), y,(int16)float_bit);
				float_bit = (int32)(num*1000)%10;
				lcd1602_show_num((uint8)(x+i+4), y,(int16)float_bit);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				lcd1602_show_num((uint8)(x+i+2), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				lcd1602_show_num((uint8)(x+i+3), y,(int16)float_bit);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				lcd1602_show_num((uint8)(x+i+4), y,(int16)float_bit);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				lcd1602_show_num((uint8)(x+i+5), y,(int16)float_bit);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				lcd1602_show_num((uint8)(x+i+2), y,(int16)float_bit);	
			break;//保留1位精度
		}
    }
}  
#endif      //LCD1602.h