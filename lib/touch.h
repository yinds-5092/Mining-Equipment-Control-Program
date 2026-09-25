/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/
#ifndef __TOUCH_H
#define __TOUCH_H	
#include <STC8HX.h>
#include "ADC.h"
#include <math.h>
#include "delay.h"
#include "tftlcd.h"
#include "eeprom.h"

#define TOUCH_EEPROM_ADDR				0x01F1		//触摸校准的数据在内部eeprom中的存储地址，一共需14个字节
#define FAC_MIN							0.9
#define FAC_MAX							1.1

#define TOUCH_READ_TIMES 				5 	//读取次数
#define TOUCH_LOST_VAL 					1	//丢弃值

//引脚定义
#ifndef SM245_TOUCH_YP_PIN
#define SM245_TOUCH_YP_PIN            P1_1        //AD
#endif 

#ifndef SM245_SET_YP_OUT
#define SM245_SET_YP_OUT            { P1M1&=~0x02;P1M0|=0x02; }   //P11推挽输出
#endif 

#ifndef SM245_SET_YP_IN
#define SM245_SET_YP_IN             { P1M1|=0x02;P1M0&=~0x02; }   //INPUT高阻输入
#endif

#ifndef SM245_YP_ADC_PIN
#define SM245_YP_ADC_PIN              ADC_P11  
#endif 

#ifndef SM245_TOUCH_XM_PIN
#define SM245_TOUCH_XM_PIN            P0_3        //AD
#endif 

#ifndef SM245_SET_XM_OUT
#define SM245_SET_XM_OUT            { P0M1&=~0x08;P0M0|=0x08;}   //P03推挽输出
#endif

#ifndef SM245_SET_XM_IN
#define SM245_SET_XM_IN             { P0M1|=0x08;P0M0&=~0x08;}   //INPUT高阻输入
#endif

#ifndef SM245_XM_ADC_PIN
#define SM245_XM_ADC_PIN              ADC_P03   
#endif

#ifndef SM245_TOUCH_YM_PIN
#define SM245_TOUCH_YM_PIN            P6_7
#endif 

#ifndef SM245_SET_YM_OUT
#define SM245_SET_YM_OUT            { P6M1&=~0x80;P6M0|=0x80;}   //P67推挽输出
#endif

#ifndef SM245_SET_YM_IN
#define SM245_SET_YM_IN             { P6M1|=0x80;P6M0&=~0x80;}   //INPUT高阻输入
#endif

#ifndef SM245_TOUCH_XP_PIN
#define SM245_TOUCH_XP_PIN           P6_6
#endif 

#ifndef SM245_SET_XP_OUT 
#define SM245_SET_XP_OUT            { P6M1&=~0x40;P6M0|=0x40;}   //P66推挽输出
#endif

#ifndef SM245_SET_XP_IN
#define SM245_SET_XP_IN             { P6M1|=0x40;P6M0&=~0x40;}   //INPUT高阻输入 
#endif

#ifndef	LCD9341_TOUCH_YP_PIN
#define LCD9341_TOUCH_YP_PIN            P0_3        //A2
#endif

#ifndef	LCD9341_SET_YP_OUT 
#define LCD9341_SET_YP_OUT            { P0M1&=~0x08;P0M0|=0x08;}   //P03推挽输出
#endif

#ifndef	LCD9341_SET_YP_IN
#define LCD9341_SET_YP_IN             { P0M1|=0x08;P0M0&=~0x08;}   //INPUT高阻输入
#endif

#ifndef	LCD9341_YP_ADC_PIN
#define LCD9341_YP_ADC_PIN              ADC_P03 
#endif

#ifndef	LCD9341_TOUCH_XM_PIN 
#define LCD9341_TOUCH_XM_PIN            P1_3        //A3
#endif

#ifndef LCD9341_SET_XM_OUT
#define LCD9341_SET_XM_OUT            { P1M1&=~0x08;P1M0|=0x08; }   //P11推挽输出
#endif

#ifndef LCD9341_SET_XM_IN
#define LCD9341_SET_XM_IN             { P1M1|=0x08;P1M0&=~0x08; }   //INPUT高阻输入
#endif

#ifndef LCD9341_XM_ADC_PIN
#define LCD9341_XM_ADC_PIN             ADC_P13 
#endif

#ifndef LCD9341_TOUCH_YM_PIN
#define LCD9341_TOUCH_YM_PIN           P6_0		//8
#endif

#ifndef LCD9341_SET_YM_OUT
#define LCD9341_SET_YM_OUT            { P6M1&=~0x01;P6M0|=0x01;}   //P67推挽输出
#endif

#ifndef LCD9341_SET_YM_IN
#define LCD9341_SET_YM_IN             { P6M1|=0x01;P6M0&=~0x01;}   //INPUT高阻输入
#endif

#ifndef LCD9341_TOUCH_XP_PIN 
#define LCD9341_TOUCH_XP_PIN            P6_1		//9
#endif

#ifndef LCD9341_SET_XP_OUT
#define LCD9341_SET_XP_OUT            { P6M1&=~0x02;P6M0|=0x02;}   //P66推挽输出
#endif

#ifndef LCD9341_SET_XP_IN
#define LCD9341_SET_XP_IN             { P6M1|=0x02;P6M0&=~0x02;}   //INPUT高阻输入 
#endif

//汉字“校准失败”
code uint8 _touch_D0A3D7BCCAA7B0DC[128]={0x10,0x40,0x10,0x20,0x10,0x20,0x11,0xFE,0xFC,0x00,0x10,0x88,0x31,0x04,0x3A,0x02,0x54,0x88,0x50,0x88,0x90,0x50,
			0x10,0x50,0x10,0x20,0x10,0x50,0x10,0x88,0x13,0x06,0x01,0x40,0x41,0x20,0x21,0x20,0x23,0xFE,0x02,0x20,0x16,0x20,0x1B,0xFC,0x12,0x20,0x22,0x20,0x23,
			0xFC,0xE2,0x20,0x22,0x20,0x22,0x20,0x23,0xFE,0x22,0x00,0x02,0x00,0x01,0x00,0x11,0x00,0x11,0x00,0x11,0x00,0x3F,0xF8,0x21,0x00,0x41,0x00,0x01,0x00,
			0xFF,0xFE,0x02,0x80,0x04,0x40,0x04,0x40,0x08,0x20,0x10,0x10,0x20,0x08,0xC0,0x06,0x00,0x40,0x7C,0x40,0x44,0x40,0x54,0x80,0x54,0xFE,0x55,0x08,0x56,
			0x88,0x54,0x88,0x54,0x88,0x54,0x50,0x54,0x50,0x10,0x20,0x28,0x50,0x24,0x88,0x45,0x04,0x82,0x02,};

//汉字“校准成功”
code uint8 _touch_D0A3D7BCB3C9B9A6[128]={0x10,0x40,0x10,0x20,0x10,0x20,0x11,0xFE,0xFC,0x00,0x10,0x88,0x31,0x04,0x3A,0x02,0x54,0x88,0x50,0x88,0x90,0x50,
			0x10,0x50,0x10,0x20,0x10,0x50,0x10,0x88,0x13,0x06,0x01,0x40,0x41,0x20,0x21,0x20,0x23,0xFE,0x02,0x20,0x16,0x20,0x1B,0xFC,0x12,0x20,0x22,0x20,0x23,
			0xFC,0xE2,0x20,0x22,0x20,0x22,0x20,0x23,0xFE,0x22,0x00,0x02,0x00,0x00,0x50,0x00,0x48,0x00,0x40,0x3F,0xFE,0x20,0x40,0x20,0x40,0x20,0x44,0x3E,0x44,
			0x22,0x44,0x22,0x28,0x22,0x28,0x22,0x12,0x2A,0x32,0x44,0x4A,0x40,0x86,0x81,0x02,0x00,0x40,0x00,0x40,0x00,0x40,0xFE,0x40,0x11,0xFC,0x10,0x44,0x10,
			0x44,0x10,0x44,0x10,0x44,0x10,0x84,0x10,0x84,0x1E,0x84,0xF1,0x04,0x41,0x04,0x02,0x28,0x04,0x10,};

//汉字“触摸校准”
code uint8 _touch_B4A5C3FED0A3D7BC[128]={0x20,0x20,0x20,0x20,0x78,0x20,0x49,0xFC,0x91,0x24,0x7D,0x24,0x55,0x24,0x55,0x24,0x7D,0xFC,0x55,0x24,0x54,0x20,
			0x7C,0x28,0x54,0x24,0x57,0xFE,0x45,0x02,0x8C,0x00,0x11,0x10,0x11,0x10,0x17,0xFC,0x11,0x10,0xFC,0x00,0x13,0xF8,0x12,0x08,0x17,0xF8,0x1A,0x08,0x33,
			0xF8,0xD0,0x40,0x17,0xFC,0x10,0xA0,0x11,0x10,0x52,0x08,0x24,0x06,0x10,0x40,0x10,0x20,0x10,0x20,0x11,0xFE,0xFC,0x00,0x10,0x88,0x31,0x04,0x3A,0x02,
			0x54,0x88,0x50,0x88,0x90,0x50,0x10,0x50,0x10,0x20,0x10,0x50,0x10,0x88,0x13,0x06,0x01,0x40,0x41,0x20,0x21,0x20,0x23,0xFE,0x02,0x20,0x16,0x20,0x1B,
			0xFC,0x12,0x20,0x22,0x20,0x23,0xFC,0xE2,0x20,0x22,0x20,0x22,0x20,0x23,0xFE,0x22,0x00,0x02,0x00,};

//汉字“依次点击红点中心进行校准”
code uint8 _touch_D2C0B4CEB5E3BBF7BAECB5E3D6D0D0C4BDF8D0D0D0A3D7BC[288]={0x12,0x0C,0x11,0x0C,0x2F,0xEC,0x21,0x0C,0x62,0x0C,0xA5,0x2C,0x2D,0x4C,0x34,0x8C,
			0x24,0x8C,0x24,0x4C,0x26,0x2C,0x24,0x0C,0x04,0x0C,0x84,0x0C,0x47,0xEC,0x08,0x2C,0x0A,0x4C,0x12,0x0C,0x02,0x0C,0x25,0x0C,0x45,0x0C,0x88,0x8C,0x10,
			0x4C,0x20,0x2C,0x04,0x0C,0x07,0xEC,0x04,0x0C,0x04,0x0C,0x7F,0xCC,0x40,0x4C,0x40,0x4C,0x40,0x4C,0x7F,0xCC,0x00,0x0C,0x52,0x4C,0x89,0x2C,0x04,0x0C,
			0x04,0x0C,0x7F,0xCC,0x04,0x0C,0x04,0x0C,0xFF,0xEC,0x04,0x0C,0x44,0x4C,0x44,0x4C,0x44,0x4C,0x7F,0xCC,0x00,0x4C,0x20,0x0C,0x27,0xCC,0x41,0x0C,0x91,
			0x0C,0xE1,0x0C,0x21,0x0C,0x41,0x0C,0xF1,0x0C,0x01,0x0C,0x31,0x0C,0xC1,0x0C,0x0F,0xEC,0x04,0x0C,0x07,0xEC,0x04,0x0C,0x04,0x0C,0x7F,0xCC,0x40,0x4C,
			0x40,0x4C,0x40,0x4C,0x7F,0xCC,0x00,0x0C,0x52,0x4C,0x89,0x2C,0x04,0x0C,0x04,0x0C,0x04,0x0C,0x7F,0xCC,0x44,0x4C,0x44,0x4C,0x44,0x4C,0x7F,0xCC,0x44,
			0x4C,0x04,0x0C,0x04,0x0C,0x04,0x0C,0x00,0x0C,0x08,0x0C,0x04,0x0C,0x02,0x0C,0x12,0x0C,0x10,0x4C,0x50,0x2C,0x50,0x2C,0x50,0x2C,0x90,0x8C,0x10,0x8C,
			0x0F,0x8C,0x44,0x8C,0x24,0x8C,0x0F,0xCC,0x04,0x8C,0xE4,0x8C,0x24,0x8C,0x3F,0xEC,0x24,0x8C,0x24,0x8C,0x28,0x8C,0x50,0x0C,0x8F,0xEC,0x10,0x0C,0x27,
			0xCC,0x40,0x0C,0x90,0x0C,0x2F,0xEC,0x60,0x8C,0xA0,0x8C,0x20,0x8C,0x20,0x8C,0x20,0x8C,0x20,0x8C,0x23,0x8C,0x22,0x0C,0x21,0x0C,0x2F,0xEC,0xF0,0x0C,
			0x22,0x8C,0x74,0x4C,0x6A,0xAC,0xA2,0x8C,0x22,0x8C,0x21,0x0C,0x22,0x8C,0x2C,0x6C,0x05,0x0C,0x04,0x8C,0x8F,0xEC,0x49,0x0C,0x19,0x0C,0x2F,0xCC,0x09,
			0x0C,0x09,0x0C,0x2F,0xCC,0x49,0x0C,0x89,0x0C,0x0F,0xEC,};

void touch_init(uint8 t); //触摸初始化程序
uint16 touch_read_raw_x();	//读取x的物理坐标
uint16 touch_read_raw_y();	//读取y的物理坐标
void touch_get_point(uint16 *tspoint);	//读取坐标以及压力值
uint16 touch_read_pressure(); //读取触摸的压力值.
//========================================================================
// 描述: 读取X物理坐标.
// 参数: none.
// 返回: 读到的坐标.y
//========================================================================
uint16 touch_read_raw_x()
{
	uint16 i, j;
	uint16 buf[TOUCH_READ_TIMES];
	uint16 sum=0;
	uint16 temp;
	if(lcddev.id == 0x9341)
	{
		LCD9341_SET_XM_OUT;
		LCD9341_SET_XP_OUT;
		LCD9341_SET_YP_IN;   
		LCD9341_SET_YM_IN;
		LCD9341_TOUCH_XP_PIN = 1;
		LCD9341_TOUCH_YP_PIN = 0;
		LCD9341_TOUCH_YM_PIN = 0;
		LCD9341_TOUCH_XM_PIN = 0;
		adc_init(LCD9341_YP_ADC_PIN, ADC_SYSclk_DIV_2, ADC_12BIT);
		for(i=0;i<TOUCH_READ_TIMES;i++)buf[i]=adc_read(LCD9341_YP_ADC_PIN); 		    
		for(i=0;i<TOUCH_READ_TIMES-1; i++)//排序
		{
			for(j=i+1;j<TOUCH_READ_TIMES;j++)
			{
				if(buf[i]>buf[j])//升序排列
				{
					temp=buf[i];
					buf[i]=buf[j];
					buf[j]=temp;
				}
			}
		}	 
		sum=0;
		for(i=TOUCH_LOST_VAL;i<TOUCH_READ_TIMES-TOUCH_LOST_VAL;i++)sum+=buf[i];
		temp=sum/(TOUCH_READ_TIMES-2*TOUCH_LOST_VAL);
		LCD9341_SET_YP_OUT;
		LCD9341_SET_YM_OUT;
		return temp;
	}
	else
	{
		SM245_SET_XM_OUT;
		SM245_SET_XP_OUT;
		SM245_SET_YP_IN;   
		SM245_SET_YM_IN;
		SM245_TOUCH_XP_PIN = 1;
		SM245_TOUCH_YP_PIN = 0;
		SM245_TOUCH_YM_PIN = 0;
		SM245_TOUCH_XM_PIN = 0;
		adc_init(SM245_YP_ADC_PIN, ADC_SYSclk_DIV_2, ADC_12BIT);
		for(i=0;i<TOUCH_READ_TIMES;i++)buf[i]=adc_read(SM245_YP_ADC_PIN); 		    
		for(i=0;i<TOUCH_READ_TIMES-1; i++)//排序
		{
			for(j=i+1;j<TOUCH_READ_TIMES;j++)
			{
				if(buf[i]>buf[j])//升序排列
				{
					temp=buf[i];
					buf[i]=buf[j];
					buf[j]=temp;
				}
			}
		}	 
		sum=0;
		for(i=TOUCH_LOST_VAL;i<TOUCH_READ_TIMES-TOUCH_LOST_VAL;i++)sum+=buf[i];
		temp=sum/(TOUCH_READ_TIMES-2*TOUCH_LOST_VAL);
		SM245_SET_YP_OUT;
		SM245_SET_YM_OUT;
		return temp;
	}
}

//========================================================================
// 描述: 读取Y物理坐标.
// 参数: none.
// 返回: 读到的坐标.
//========================================================================
uint16 touch_read_raw_y()
{
	uint16 i, j;
	uint16 buf[TOUCH_READ_TIMES];
	uint16 sum=0;
	uint16 temp;
	if(lcddev.id == 0x9341)
	{
		LCD9341_SET_YP_OUT;
		LCD9341_SET_YM_OUT;
		LCD9341_SET_XP_IN;   
		LCD9341_SET_XM_IN;
		LCD9341_TOUCH_YP_PIN = 1;
		LCD9341_TOUCH_YM_PIN = 0;
		LCD9341_TOUCH_XP_PIN = 0;
		LCD9341_TOUCH_XM_PIN = 0;
	
		adc_init(LCD9341_XM_ADC_PIN, ADC_SYSclk_DIV_2, ADC_12BIT);

		for(i=0;i<TOUCH_READ_TIMES;i++)buf[i]=adc_read(LCD9341_XM_ADC_PIN); 		    
		for(i=0;i<TOUCH_READ_TIMES-1; i++)//排序
		{
			for(j=i+1;j<TOUCH_READ_TIMES;j++)
			{
				if(buf[i]>buf[j])//升序排列
				{
					temp=buf[i];
					buf[i]=buf[j];
					buf[j]=temp;
				}
			}
		}	 
		sum=0;
		for(i=TOUCH_LOST_VAL;i<TOUCH_READ_TIMES-TOUCH_LOST_VAL;i++)sum+=buf[i];
		temp=sum/(TOUCH_READ_TIMES-2*TOUCH_LOST_VAL);
		LCD9341_SET_XM_OUT;
		LCD9341_SET_XP_OUT;
		return temp;
	}
	else
	{
		SM245_SET_YP_OUT;
		SM245_SET_YM_OUT;
		SM245_SET_XP_IN;   
		SM245_SET_XM_IN;
		SM245_TOUCH_YP_PIN = 1;
		SM245_TOUCH_YM_PIN = 0;
		SM245_TOUCH_XP_PIN = 0;
		SM245_TOUCH_XM_PIN = 0;
	
		adc_init(SM245_XM_ADC_PIN, ADC_SYSclk_DIV_2, ADC_12BIT);

		for(i=0;i<TOUCH_READ_TIMES;i++)buf[i]=adc_read(SM245_XM_ADC_PIN); 		    
		for(i=0;i<TOUCH_READ_TIMES-1; i++)//排序
		{
			for(j=i+1;j<TOUCH_READ_TIMES;j++)
			{
				if(buf[i]>buf[j])//升序排列
				{
					temp=buf[i];
					buf[i]=buf[j];
					buf[j]=temp;
				}
			}
		}	 
		sum=0;
		for(i=TOUCH_LOST_VAL;i<TOUCH_READ_TIMES-TOUCH_LOST_VAL;i++)sum+=buf[i];
		temp=sum/(TOUCH_READ_TIMES-2*TOUCH_LOST_VAL);
		SM245_SET_XM_OUT;
		SM245_SET_XP_OUT;
		return temp;
	}
}

//========================================================================
// 描述: 读取触摸的压力值.
// 参数: none.
// 返回: 读到的压力值.
//========================================================================
uint16 touch_read_pressure()
{
    int z1,z2;
	if(lcddev.id == 0x9341)
	{
		LCD9341_SET_XP_OUT;
		LCD9341_SET_YM_OUT;
		LCD9341_SET_XM_IN;
		LCD9341_SET_YP_IN;
		LCD9341_TOUCH_XP_PIN = 0;
		LCD9341_TOUCH_YM_PIN = 1; 
		LCD9341_TOUCH_XM_PIN = 0;
		LCD9341_TOUCH_YP_PIN = 0;

		adc_init(LCD9341_XM_ADC_PIN, ADC_SYSclk_DIV_2, ADC_12BIT);
		adc_init(LCD9341_YP_ADC_PIN, ADC_SYSclk_DIV_2, ADC_12BIT);

		z1 = adc_read(LCD9341_XM_ADC_PIN); 
		z2 = adc_read(LCD9341_YP_ADC_PIN);

		LCD9341_SET_XM_OUT;
		LCD9341_SET_YP_OUT;
		LCD9341_TOUCH_YP_PIN = 1;
		LCD9341_TOUCH_XM_PIN = 1;
		return (4095 - (z2-z1));
	}
	else
	{
		SM245_SET_XP_OUT;
		SM245_SET_YM_OUT;
		SM245_SET_XM_IN;
		SM245_SET_YP_IN;
		SM245_TOUCH_XP_PIN = 0;
		SM245_TOUCH_YM_PIN = 1; 
		SM245_TOUCH_XM_PIN = 0;
		SM245_TOUCH_YP_PIN = 0;

		adc_init(SM245_XM_ADC_PIN, ADC_SYSclk_DIV_2, ADC_12BIT);
		adc_init(SM245_YP_ADC_PIN, ADC_SYSclk_DIV_2, ADC_12BIT);

		z1 = adc_read(SM245_XM_ADC_PIN); 
		z2 = adc_read(SM245_YP_ADC_PIN);

		SM245_SET_XM_OUT;
		SM245_SET_YP_OUT;
		SM245_TOUCH_YP_PIN = 1;
		SM245_TOUCH_XM_PIN = 1;
		return (4095 - (z2-z1));
	}
}

//========================================================================
// 描述: 通过2次采样误差得到更准确的坐标值.
// 参数: x,y:采样得到的物理坐标.
// 返回: 误差在50以内返回1，否则返回0.
//========================================================================
#define ERR_RANGE 	50      //误差范围 
uint8 tp_read_xy2(uint16 *x,uint16 *y) 
{
	uint16 x1,y1;
 	uint16 x2,y2;

	x1 = touch_read_raw_x();
	y1 = touch_read_raw_y();
	x2 = touch_read_raw_x();
	y2 = touch_read_raw_y();

    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后2次采样误差在50以内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
} 

//========================================================================
// 描述: 画一个触摸点.
// 参数: x,y:坐标; color:颜色值.
// 返回: none.
//========================================================================
void tp_drow_touch_point(uint16 x,uint16 y,uint16 color)
{
	_tft_lcd_point_color = color;
	tft_lcd_draw_line(x-12,y,x+13,y,color);    //横线
	tft_lcd_draw_line(x,y-12,x,y+13,color);    //竖线
	tft_lcd_draw_point(x+1,y+1);
	tft_lcd_draw_point(x-1,y+1);
	tft_lcd_draw_point(x+1,y-1);
	tft_lcd_draw_point(x-1,y-1);
	tft_lcd_draw_circle(x,y,6,color,0);         //画中心圈
}

//触摸屏控制器
typedef struct
{		
	uint16 x;      	
	uint16 y;		             		
    /////////////////////触摸屏校准参数//////////////////////								
	float xfac;					
	float yfac;
	int xoff;
	int yoff;	   

	uint8 touchtype;	//触屏类型,	
						//最高位为1代表该屏id为9341，引脚和SM245相同
}_m_tp_dev;

_m_tp_dev tp_dev=
{
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,
	0	  	 		
};

//========================================================================
// 描述: 触摸校准程序.
// 参数: none.
// 返回: none.
//========================================================================
void touch_adjust()
{
	uint16 pos_temp[4][2];//坐标缓冲值
	uint16 swap;
	uint8  cnt=0;
	uint8  i=0;	
	uint16 d1,d2;
	uint32 tem1,tem2;
	float fac; 	
	uint16 outtime=0;
 	cnt=0;				
	_tft_lcd_point_color=TFT_LCD_BLUE;
	_tft_lcd_back_color =TFT_LCD_WHITE;
	tft_lcd_clear(TFT_LCD_WHITE);   //清屏 
	_tft_lcd_point_color=TFT_LCD_RED;    //红色
	tft_lcd_clear(TFT_LCD_WHITE);   //清屏   
	_tft_lcd_point_color=TFT_LCD_BLACK;

	tft_lcd_show_font16(4,_touch_B4A5C3FED0A3D7BC,88,40,(TFT_LCD_RED),(TFT_LCD_WHITE),1);//显示提示信息
	tft_lcd_show_font12(12,_touch_D2C0B4CEB5E3BBF7BAECB5E3D6D0D0C4BDF8D0D0D0A3D7BC,48,60,(TFT_LCD_RED),(TFT_LCD_WHITE),1);

	tp_drow_touch_point(20,20,TFT_LCD_RED);     //画点1

	tp_dev.xfac=0;      //xfac清除校准标志	
    while(1)            //如果连续10s没有按下，则自动退出
    {
        if( tp_read_xy2(&tp_dev.x,&tp_dev.y) && touch_read_pressure() > 100)     //代表有按键按下
        {
            outtime = 0;

			pos_temp[cnt][0] = tp_dev.x;
			pos_temp[cnt][1] = tp_dev.y; 

            cnt++;
            switch(cnt)
			{			   
				case 1:					 
					tp_drow_touch_point(20,20,TFT_LCD_WHITE);				//清除点1
					tp_drow_touch_point(lcddev.width-20,20,TFT_LCD_RED);	//画点2
					break;
				case 2:
 					tp_drow_touch_point(lcddev.width-20,20,TFT_LCD_WHITE);	//清除点2
					tp_drow_touch_point(20,lcddev.height-20,TFT_LCD_RED);	//画点3
					break;
				case 3:
 					tp_drow_touch_point(20,lcddev.height-20,TFT_LCD_WHITE);			//清除点3
 					tp_drow_touch_point(lcddev.width-20,lcddev.height-20,TFT_LCD_RED);	//画点4
					break;
				case 4:	 //全部4个点已经得到
					if((((int16)pos_temp[1][1] - (int16)pos_temp[0][1]) >= 300) || (((int16)pos_temp[1][1] - (int16)pos_temp[0][1]) <= -300) )  //如果x，y轴反了
					{
						tp_dev.touchtype |= 0x04;
						for(i=0;i<4;i++){
							swap = pos_temp[i][0];
							pos_temp[i][0] = pos_temp[i][1];
							pos_temp[i][1] = swap;
						}
					}

					if(((int16)pos_temp[0][0] -  (int16)pos_temp[1][0]) >= 300)	//如果x从左到右递减
					{
						tp_dev.touchtype |= 0x01;
						for(i=0;i<4;i++){
							pos_temp[i][0] = 4095 - pos_temp[i][0];
						}
					}

					if(((int16)pos_temp[0][1] -  (int16)pos_temp[2][1]) >= 300 ) //如果y从上到下递减
					{
						tp_dev.touchtype |= 0x02;
						for(i=0;i<4;i++){
							pos_temp[i][1] = 4095 - pos_temp[i][1];
						}
					}

	    		    //对边相等
					tem1=fabsf(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=fabsf(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrtf(tem1+tem2);     //得到1，2的距离
					
					tem1=fabsf(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=fabsf(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrtf(tem1+tem2);//得到3，4的距离
					fac=(float)d1/d2;
					if(fac<FAC_MIN||fac>FAC_MAX||d1==0||d2==0)//不合格	
					{
						cnt=0;
 				    	tp_drow_touch_point(lcddev.width-20,lcddev.height-20,TFT_LCD_WHITE);	//清除点4
   	 					tp_drow_touch_point(20,20,TFT_LCD_RED);								//画点一 
						continue;
					}
					tem1=fabsf(pos_temp[0][0]-pos_temp[2][0]);    //x1-x3
					tem2=fabsf(pos_temp[0][1]-pos_temp[2][1]);    //y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrtf(tem1+tem2);//得到1，3的距离
					
					tem1=fabsf(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=fabsf(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrtf(tem1+tem2);//得到2，4的距离
					fac=(float)d1/d2;
					if(fac<FAC_MIN||fac>FAC_MAX)//不合格
					{
						cnt=0;
 				    	tp_drow_touch_point(lcddev.width-20,lcddev.height-20,TFT_LCD_WHITE);	//清除点4
   	 					tp_drow_touch_point(20,20,TFT_LCD_RED);								//画点1
						continue;
					}//正确了
								   
					//对角线相等
					tem1=fabsf(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=fabsf(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrtf(tem1+tem2);    //得到1，4的距离
	
					tem1=fabsf(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=fabsf(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrtf(tem1+tem2);//得到2，3的距离
					fac=(float)d1/d2;
					if(fac<FAC_MIN||fac>FAC_MAX)//不合格
					{
						cnt=0;
 				    	tp_drow_touch_point(lcddev.width-20,lcddev.height-20,TFT_LCD_WHITE);	//清除点4
   	 					tp_drow_touch_point(20,20,TFT_LCD_RED);								//画点1
						continue;
					}//正确了
					//计算结果
					tp_dev.xfac=(float)(lcddev.width-40)/(pos_temp[1][0]-pos_temp[0][0]);//得到xfac		 
					tp_dev.xoff=(lcddev.width-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//得到xoff
						  
					tp_dev.yfac=(float)(lcddev.height-40)/(pos_temp[2][1]-pos_temp[0][1]);//得到yfac
					tp_dev.yoff=(lcddev.height-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//得到yoff  

					if(fabsf(tp_dev.xfac)>2||fabsf(tp_dev.yfac)>2)//触屏和预设的相反了
					{
						cnt=0;
						tft_lcd_clear(TFT_LCD_WHITE);
 				    	tp_drow_touch_point(lcddev.width-20,lcddev.height-20,TFT_LCD_WHITE);	//清除点4
   	 					tp_drow_touch_point(20,20,TFT_LCD_RED);								//画点1
						//tft_lcd_show_string(40,26,"TP Need readjust!",TFT_LCD_RED,TFT_LCD_WHITE,16,1);
						tft_lcd_show_font16(4,_touch_D0A3D7BCCAA7B0DC,88,40,(TFT_LCD_RED),(TFT_LCD_WHITE),1);	    
						continue;
					}		
					_tft_lcd_point_color=TFT_LCD_BLUE;
					tft_lcd_clear(TFT_LCD_WHITE);//清屏
					tft_lcd_show_font16(4,_touch_D0A3D7BCB3C9B9A6,88,40,(TFT_LCD_RED),(TFT_LCD_WHITE),1);
					//tft_lcd_show_string(35,110,"Touch Screen Adjust OK!",TFT_LCD_RED,TFT_LCD_WHITE,16,1);//校正完成
					delay(200);
					//把校准数据写入触摸屏
                    eeprom_sector_erase(TOUCH_EEPROM_ADDR);
    				eeprom_write_one_float(TOUCH_EEPROM_ADDR,tp_dev.xfac);
					eeprom_write_one_float(TOUCH_EEPROM_ADDR+sizeof(float),tp_dev.yfac);
					eeprom_write_one_ushort(TOUCH_EEPROM_ADDR+2*sizeof(float),tp_dev.xoff);
					eeprom_write_one_ushort(TOUCH_EEPROM_ADDR+2*sizeof(float)+sizeof(int),tp_dev.yoff);
					eeprom_write_one_ushort(TOUCH_EEPROM_ADDR+2*sizeof(float)+2*sizeof(int),(int)tp_dev.touchtype);
 					tft_lcd_clear(TFT_LCD_WHITE);//清屏  
					return;//校正完成				 
			}
            while(touch_read_pressure() > 100);
		}
		delay(10);
		outtime++;
		if(outtime>1000)
		{
            tft_lcd_clear(TFT_LCD_WHITE);
			//从存储器中得到校准数据
            tp_dev.xfac = eeprom_read_one_float(TOUCH_EEPROM_ADDR);
			tp_dev.yfac = eeprom_read_one_float(TOUCH_EEPROM_ADDR+sizeof(float));
			tp_dev.xoff = eeprom_read_one_ushort(TOUCH_EEPROM_ADDR+2*sizeof(float));
			tp_dev.yoff = eeprom_read_one_ushort(TOUCH_EEPROM_ADDR+2*sizeof(float)+sizeof(int));
			tp_dev.touchtype =(uint8)eeprom_read_one_ushort(TOUCH_EEPROM_ADDR+2*sizeof(float)+2*sizeof(int));
			return;
	 	} 
 	}            
}

//========================================================================
// 描述: 触摸的初始化函数.
// 参数: t:0,不进入校准程序;其他值:进入校准程序.
// 返回: none.
//========================================================================
void touch_init(uint8 t)
{
	tp_dev.touchtype = 0;
	if(t == 0)	//直接从eeprom中获取校准数据
	{
		tp_dev.xfac = eeprom_read_one_float(TOUCH_EEPROM_ADDR);
		tp_dev.yfac = eeprom_read_one_float(TOUCH_EEPROM_ADDR+sizeof(float));
		tp_dev.xoff = eeprom_read_one_ushort(TOUCH_EEPROM_ADDR+2*sizeof(float));
		tp_dev.yoff = eeprom_read_one_ushort(TOUCH_EEPROM_ADDR+2*sizeof(float)+sizeof(int));	
		tp_dev.touchtype = (uint8)eeprom_read_one_ushort(TOUCH_EEPROM_ADDR+2*sizeof(float)+2*sizeof(int));	
	}else{

		if( (lcddev.id == 0x9341) && (touch_read_pressure() >= 100) )	//彩屏id为9341引脚却和SM245一致，临时将id转为0.
		{
			lcddev.id = 0;
			tp_dev.touchtype |= 0x80;
			touch_adjust();	//进入校准程序
			lcddev.id = 0x9341;	//将id还原
		}else{
			touch_adjust();	//进入校准程序
		}
	}
}

//========================================================================
// 描述: 读取触摸的坐标以及压力值.
// 参数: 读取到的坐标.
// 返回: none.
//========================================================================
void touch_get_point(uint16 *tspoint)
{
 	uint16 x,y,swap;
	if(tp_dev.touchtype & 0x80)	//彩屏id为9341引脚却和SM245一致，临时将id转为0.
	{
		lcddev.id = 0;
	}
	tp_read_xy2(&x,&y);

	if(tp_dev.touchtype&0x01)
	{
		x = 4095 - x;
	}
	if(tp_dev.touchtype&0x02)
	{
		y = 4095 - y;
	}
	if(tp_dev.touchtype&0x04)  //如果x，y方向相反了
	{
		swap = x;
		x = y;
		y = swap;
	}

	//读取X坐标
	tspoint[0] = tp_dev.xfac*(x)+tp_dev.xoff;	
	//读取Y坐标
	tspoint[1] = tp_dev.yfac*(y)+tp_dev.yoff;	

    //读取压力
    tspoint[2] = touch_read_pressure();

	if(tp_dev.touchtype & 0x80)	//id还原.
	{
		lcddev.id = 0x9341;
	}
}

#endif      //touch.h