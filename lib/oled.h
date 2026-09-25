/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef __OLED_H
#define __OLED_H

#include <STC8HX.h>
#include "delay.h"
#include "hardiic.h"
#include <string.h>
#include <stdio.h>
#include "oledfont.h"
#include <stdlib.h>

#define OLED_ADDR   0x78 //写地址
#define OLED_CMD    0	//写命令
#define OLED_DATA   1	//写数据
#define OLED_MODE   0

#define OLED_MAX_COLUMN	    128
#define OLED_MAX_ROW		32

static uint8 xdata _oled_disbuffer[128][4];
void oled_init();	//初始化OLED
void oled_display_off();	//关闭OLED显示
void oled_display_on();     //开启OLED显示
void oled_display();		//缓存显示
void oled_clear();	//清屏
void oled_set_pixel(uint8 x, uint8 y,uint8 pixel);	//在指定位置显示一个点
//X 个像素点，y行
void oled_show_char(int8 x,int8 y,uint8 chr);	//在指定位置显示一个字符  字符高8，宽5
void oled_show_string(int8 x,int8 y,uint8 *chr); //在指定位置显示字符串 字符高8，间距8
void oled_show_num(int8 x,int8 y,int16 num);	//在指定位置显示数字 字符高8，间距8
void oled_show_float(int8 x, int8 y, float num, uint8 precision); //在指定位置显示小数
void oled_show_font12(uint8 lenth,const uint8* hz,int x,int y);	//在指定位置显示12x12汉字
void oled_show_font16(uint8 lenth,const uint8* hz,int x,int y);	//在指定位置显示16x16汉字
void oled_show_font24(uint8 lenth,const uint8* hz,int x,int y);	//在指定位置显示24x24汉字
void oled_show_font32(uint8 lenth, const uint8* hz, int x, int y);	//在指定位置显示32x32汉字

void oled_fill(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel);    //填充
void oled_show_line(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel);   //绘制线段
void oled_show_rectangle(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel);  //绘制矩形框
void oled_fill_rectangle(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel);  //填充矩形
void oled_show_circle(int16 xc, int8 yc, int8 r, uint8 pixel, uint8 fill);  //绘制圆
void oled_show_triangel(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 pixel);   //绘制三角形
void oled_fill_triangel(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 pixel);   //填充三角形

void oled_show_bmp(uint8 x0, uint8 y0,uint8 x1, uint8 y1,uint8* BMP);	//显示BMP图片

//========================================================================
// 描述: OLED模块写命令.
// 参数: 写的命令.
// 返回: none.
//========================================================================
static void oled_write_command(uint8 IIC_Command)
{
    hardiic_start();
    hardiic_send_byte(OLED_ADDR);            //Slave address,SA0=0
    hardiic_wait_ack();	
    hardiic_send_byte(0x00);			//write command
	hardiic_wait_ack();	
    hardiic_send_byte(IIC_Command); 
	hardiic_wait_ack();	
    hardiic_stop();
}

//========================================================================
// 描述: OLED模块写数据.
// 参数: 写的数据.
// 返回: none.
//========================================================================
static void oled_write_data(uint8 IIC_Data)
{
    hardiic_start();
    hardiic_send_byte(OLED_ADDR);			//D/C#=0; R/W#=0
	hardiic_wait_ack();	
    hardiic_send_byte(0x40);			//write data
	hardiic_wait_ack();	
    hardiic_send_byte(IIC_Data);
	hardiic_wait_ack();	
    hardiic_stop();
}

//========================================================================
// 描述: OLED模块写一个字节.
// 参数: dat:要写入的字节 ; cmd:写入的字节是数据（1）还是命令（0）
// 返回: none.
//========================================================================
static void oled_wr_byte(uint8 dat,uint8 cmd)
{
    if(cmd)
    {
        oled_write_data(dat);
    }else{
        oled_write_command(dat);
	}
}

//========================================================================
// 描述: 坐标设置
// 参数: x,y:坐标; 
// 返回: none.
//========================================================================
void oled_set_pos(uint8 x, uint8 y) 
{ 	
    oled_wr_byte(0xb0+y,OLED_CMD);		//设置页地址（0-7）
	oled_wr_byte(((x&0xf0)>>4)|0x10,OLED_CMD);	//设置显示位置-列低地址
	oled_wr_byte((x&0x0f),OLED_CMD); 	 //设置显示位置-列高地址   
}

//========================================================================
// 描述: 开启OLED显示
// 参数: none.
// 返回: none.
//========================================================================
void oled_display_on()
{
	oled_wr_byte(0X8D,OLED_CMD);  //SET DCDC命令
	oled_wr_byte(0X14,OLED_CMD);  //DCDC ON
	oled_wr_byte(0XAF,OLED_CMD);  //DISPLAY ON
}

//========================================================================
// 描述: 关闭OLED显示
// 参数: none.
// 返回: none.
//========================================================================
void oled_display_off()
{
	oled_wr_byte(0X8D,OLED_CMD);  //SET DCDC命令
	oled_wr_byte(0X10,OLED_CMD);  //DCDC OFF
	oled_wr_byte(0XAE,OLED_CMD);  //DISPLAY OFF
}

//========================================================================
// 描述: 清屏
// 参数: none.
// 返回: none.
//========================================================================
void oled_clear()  
{  
	uint8 y,x;
	for(y=0;y<4;y++)
	{
		for(x=0;x<128;x++)
		{
			_oled_disbuffer[x][y]=0x00;
		}
	}
}

//========================================================================
// 描述: 更新显示
// 参数: none.
// 返回: none.
//========================================================================
void oled_display()  
{  
    uint8 x,y;
	for(y=0;y<4;y++)  
	{  
		oled_wr_byte (0xb0+y,OLED_CMD);    //设置页地址（0-7）
		oled_wr_byte (0x00,OLED_CMD);      //设置显示位置-列低地址
		oled_wr_byte (0x10,OLED_CMD);      //设置显示位置-列高地址  
		for(x=0;x<128;x++)oled_wr_byte(_oled_disbuffer[x][y],OLED_DATA); 
	} 
    oled_wr_byte(0xaf,OLED_CMD);
}//更新显示

//========================================================================
// 描述: OLED填充
// 参数: dot:
// 返回: none.
//========================================================================
void oled_fill(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel)
{
    uint8 i,j,swap;
    if (x0 > OLED_MAX_COLUMN || y0 > OLED_MAX_ROW) return;
    if (x1 > OLED_MAX_COLUMN || y1 > OLED_MAX_ROW) return;
    if(x0 > x1){
        swap = x1;
        x1 = x0;
        x0 = swap;
    }
    if(y0 > y1){
        swap = y1;
        y1 = y0;
        y0 = swap;
    }
    for(i=x0;i<=x1;i++)
        for(j=y0;j<=y1;j++)
        {
            oled_set_pixel(i, j, pixel);
        }
}

//========================================================================
// 描述: 计算m^n函数
// 参数: m,n:要计算的数值
// 返回: none.
//========================================================================
// static unsigned int oled_pow(uint8 m,uint8 n)
// {
// 	unsigned int result=1;	 
// 	while(n--)result*=m;    
// 	return result;
// }

//========================================================================
// 描述: 在指定位置显示一个点
// 参数: x,y:坐标; pixel:亮或灭
// 返回: none.
//========================================================================
void oled_set_pixel(uint8 x, uint8 y,uint8 pixel) 
{ 
    if (x > OLED_MAX_COLUMN || y > OLED_MAX_ROW) {
        return;
    }
    else
    {
        if (pixel) {
            _oled_disbuffer[x][y / 8] |= (1 << (y % 8));
        }
        else {
            _oled_disbuffer[x][y / 8] &= ~(1 << (y % 8));
        }
    }
}

//========================================================================
// 描述: 在指定位置显示一个字符
// 参数: x,y:起点坐标; chr:要显示的字符;
// 返回: none.
//========================================================================
void oled_show_char(int8 x,int8 y,uint8 chr)
{
	uint8 temp;
	uint8 j,i;
//	chr=chr-' ';						  //得到偏移后的值
	if(x < 0)
	{
		for(j=0;j<5;j++)
		{
			temp=fontx[chr][j];
			for(i=0;i<8;i++)
		    {   
				if(((x + i) >= 0)){
                    if(temp&0x01){
                        oled_set_pixel(x+j,y+i,1);
                    }
                }
				temp>>=1; 
		    }
		}	
	}
	else{
		for(j=0;j<5;j++)
		{
			temp=fontx[chr][j];
			for(i=0;i<8;i++)
		    {      
				if(temp&0x01)
                {
                   oled_set_pixel(x+j,y+i,1);  
                }
                temp>>=1; 
			}
		}			
    }
}


// void oled_show_char(uint8 x,uint8 y,uint8 chr)
// {      	
// 	uint8 c=0,i=0;
// 	uint8 ci=0,x1=0,y1=y;
// 	if(y>4)return;
// 	for(ci=0;ci<5;ci++){
// 		x1 = x+ci+i*5;
// 		if(x1>122){x1=x1-122;y1=y+1;}
// 		_oled_disbuffer[x1][y1]=fontx[chr][ci];
// 	}
// }

	// unsigned char c=0,i=0;	
	// c=chr-' ';//得到偏移后的值		
	// if(x>Max_Column-1){x=0;y=y+2;}
	// if(Char_Size ==16)
	// {
	// 	OLED_Set_Pos(x,y);	
	// 	for(i=0;i<8;i++)
	// 		OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
	// 	OLED_Set_Pos(x,y+1);
	// 	for(i=0;i<8;i++)
	// 		OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	// 	}else 
	// 	{	
	// 		OLED_Set_Pos(x,y);
	// 		for(i=0;i<6;i++)
	// 			OLED_WR_Byte(F6x8[c][i],OLED_DATA);
    //     }

//========================================================================
// 描述: 显示一个字符串
// 参数: x,y:起点坐标;chr:要显示的字符串
// 返回: none.
//========================================================================
void oled_show_string(int8 x,int8 y,uint8 *chr)
{
	uint8 j=0;
	while (chr[j]!='\0')
	{		
	    oled_show_char(x,y,chr[j]);
		x+=8;
		if(x>120){
		    x=0;
			y+=2;
		}
		j++;
	}
}

// //========================================================================
// // 描述: 显示数字
// // 参数: x,y:起点坐标;num:要显示的数值 len: 数字的位数;
// // 返回: none.
// //========================================================================
// void oled_show_num(uint8 x,uint8 y,uint16 num,uint8 len)
// {         	
// 	uint8 t,temp;
// 	uint8 enshow=0;						   
// 	for(t=0;t<len;t++)
// 	{
// 		temp=(num/oled_pow(10,len-t-1))%10;
// 		if(enshow==0&&t<(len-1))
// 		{
// 			if(temp==0)
// 			{
// 				oled_show_char(x+5*t,y,' ');
// 				continue;
// 			}else enshow=1; 
// 		}
// 	 	oled_show_char(x+5*t,y,temp+'0'); 
// 	}
// } 

//========================================================================
// 描述: 在指定位置显示数字
// 参数: x,y:坐标; num:显示的数字; 
// 返回: none.
//========================================================================
void oled_show_num(int8 x,int8 y,int16 num)
{
	char men[15];
	//char *str = men;
	//sprintf(str,"%d",num);
    _itoa(num,men,10);
	oled_show_string(x,y,men);
} 

//========================================================================
// 描述: 显示小数
// 参数: x,y:起点坐标;num:要显示的数值;precision:精度.
// 返回: none.
//========================================================================
void oled_show_float(int8 x, int8 y, float num, uint8 precision)
{
    uint8 i;
	uint8 float_bit;
    int16 int_part;
	int_part = (int16)num/1;

    if(num>=0)			//整数部分
    {
		oled_show_num( x, y, int_part);
        for(i=1;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
		num = num - int_part;
		oled_show_char( x+8*i, y, '.');		//显示小数点
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				oled_show_num( x+8*(i+1), y,(int16)float_bit);	
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				oled_show_num( x+8*(i+1), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;
				oled_show_num( x+8*(i+2), y,(int16)float_bit);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				oled_show_num( x+8*(i+1), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;
				oled_show_num( x+8*(i+2), y,(int16)float_bit);
				float_bit = (int32)(num*1000)%10;
				oled_show_num( x+8*(i+3), y,(int16)float_bit);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				oled_show_num( x+8*(i+1), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				oled_show_num( x+8*(i+2), y,(int16)float_bit);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				oled_show_num( x+8*(i+3), y,(int16)float_bit);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				oled_show_num( x+8*(i+4), y,(int16)float_bit);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				oled_show_num( x+8*(i+1), y,(int16)float_bit);	
			break;//保留1位精度
		}
    }
    else
    {
		i = 0;
		if(int_part == 0)
		{
			oled_show_char( x, y, '-');		//显示小数点
			i += 1;
		}
		oled_show_num( x+8*i, y, int_part);
        for(i=1;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
		num = -(num - int_part);
		oled_show_char( x+8*(i+1), y, '.');		//显示小数点
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				oled_show_num( x+8*(i+2), y,(int16)float_bit);	
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				oled_show_num( x+8*(i+2), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;
				oled_show_num( x+8*(i+3), y,(int16)float_bit);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				oled_show_num( x+8*(i+2), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;
				oled_show_num( x+8*(i+3), y,(int16)float_bit);
				float_bit = (int32)(num*1000)%10;
				oled_show_num( x+8*(i+4), y,(int16)float_bit);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				oled_show_num( x+8*(i+2), y,(int16)float_bit);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				oled_show_num( x+8*(i+3), y,(int16)float_bit);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				oled_show_num( x+8*(i+4), y,(int16)float_bit);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				oled_show_num( x+8*(i+5), y,(int16)float_bit);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				oled_show_num( x+8*(i+2), y,(int16)float_bit);	
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

//========================================================================
// 描述: 显示12*12汉字
// 参数: x,y:起点坐标;lenth:要显示的汉字长度;hz:字库数组
// 返回: none.
//========================================================================
void oled_show_font12(uint8 lenth,const uint8* hz,int x,int y)
{      
	int zs;
	uint8 zz,i,j;	    
	if (x < 0)
    {
        zs = (0 - x) / 12;

        for (zz = zs; zz < lenth; zz++)
        {
            for (i = 0; i < 12; i++)
            {
                for (j = 0; j < 12; j++)
                {
                    if ((zz * 12 + j + x - 12) > 127)return;
                    if ((y + i - 12) > 63)return;
                    if (!((zz * 12 + x + j) < 0))
                    if ((hz[zz * 24 + i * 2 + j / 8]) & (0x80 >> (j % 8)))oled_set_pixel(zz * 12 + x + j, y + i, 1);
                }
            }

        }
    }
    else
    {
        for (zz = 0; zz < lenth; zz++) {
            for (i = 0; i < 12; i++)
            {
                for (j = 0; j < 12; j++)
                {
                    if ((zz *12 + j + x - 12) > 127)return;
                    if ((y + i - 12) > 63)return;
                    if ((hz[zz * 24 + i * 2 + j / 8]) & (0x80 >> (j % 8)))oled_set_pixel(zz * 12 + x + j, y + i, 1);
                }
            }
        }
    }			
}

//========================================================================
// 描述: 显示16*16汉字
// 参数: 参数: x,y:起点坐标;lenth:要显示的汉字长度;hz:字库数组
// 返回: none.
//========================================================================
void oled_show_font16(uint8 lenth,const uint8* hz,int x,int y)
{    
	int zs;
	uint8 zz,i,j;
	if (x < 0)
    {
        zs = (0 - x) / 16;
        for (zz = zs; zz < lenth; zz++)
        {
            for (i = 0; i < 16; i++)
            {
                for (j = 0; j < 16; j++)
                {
                    if ((zz * 16 + j + x - 16) > 127)return;
                    if ((y + i - 16) > 63)return;
                    if (!((zz * 16 + x + j) < 0))
                    if ((hz[zz * 32 + i * 2 + j / 8]) & (0x80 >> (j % 8)))oled_set_pixel(zz * 16 + x + j, y + i, 1);
                }
            }
        }
    }
    else {
        for (zz = 0; zz < lenth; zz++) {
            for (i = 0; i < 16; i++)
            {
                for (j = 0; j < 16; j++)
                {
                    if ((zz * 16 + j + x - 16) > 127)return;
                    if ((y + i - 16) > 63)return;
                    if ((hz[zz * 32 + i * 2 + j / 8]) & (0x80 >> (j % 8)))oled_set_pixel(zz * 16 + x + j, y + i, 1);
                }
            }
        }
    }					
}

//========================================================================
// 描述: 显示24*24汉字
// 参数: 参数: x,y:起点坐标;lenth:要显示的汉字长度;hz:字库数组
// 返回: none.
//========================================================================
void oled_show_font24(uint8 lenth,const uint8* hz,int x,int y)
{   
	uint16 zs;
	uint8 zz,i,j;	   			    
	if (x < 0) 
    {
        zs = (0 - x)/24;
        
        for (zz = zs; zz < lenth; zz++)
        {
            for (i = 0; i < 24; i++)
            {
                for (j = 0; j < 24; j++)
                {
                    if ((zz * 24 + j + x - 24) > 127)return;
                    if ((y + i - 24) > 63)return;
                    if(!((zz*24+x+j)<0))
                    if ((hz[zz * 72 + i * 3 + j / 8]) & (0x80 >> (j % 8)))oled_set_pixel(zz * 24 +x+ j, y + i, 1);
                }
            }
        }
    }
    else {
        for (zz = 0; zz < lenth; zz++) {
            for (i = 0; i < 24; i++)
            {
                for (j = 0; j < 24; j++)
                {
                    if ((zz * 24 + j + x-24) > 127)return;
                    if ((y + i-24) > 63)return;
                    if ((hz[zz * 72 + i * 3 + j / 8]) & (0x80 >> (j % 8)))oled_set_pixel(zz * 24 + x + j, y + i, 1);
                }
            }
        }
    }
}
//========================================================================
// 描述: 显示32*32汉字
// 参数: 参数: x,y:起点坐标;lenth:要显示的汉字长度;hz:字库数组
// 返回: none.
//========================================================================
void oled_show_font32(uint8 lenth, const uint8* hz, int x, int y)
{
    uint16 zs;
    uint8 zz, i, j;
    if (x < 0)
    {
        zs = (0 - x) / 32;

        for (zz = zs; zz < lenth; zz++)
        {
            for (i = 0; i < 32; i++)
            {
                for (j = 0; j < 32; j++)
                {
                    if ((zz * 32 + j + x - 32) > 127)return;
                    if ((y + i - 32) > 63)return;
                    if (!((zz * 32 + x + j) < 0))
                        if ((hz[zz * 128 + j * 4 + i / 8]) & (0x80 >> (i % 8)))oled_set_pixel(zz * 32 + x + j, y + i, 1);
                }
            }
        }
    }
    else {
        for (zz = 0; zz < lenth; zz++) {
            for (i = 0; i < 32; i++)
            {
                for (j = 0; j < 32; j++)
                {
                    if ((zz * 32 + j + x - 32) > 127)return;
                    if ((y + i - 32) > 63)return;
                    if ((hz[zz * 128 + j * 4 + i / 8]) & (0x80 >> (i % 8)))oled_set_pixel(zz * 32 + x + j, y + i, 1);
                }
            }
        }
    }
}

//========================================================================
// 描述: 交换函数（内部调用）.
// 参数: a,b:要交换的值的指针.
// 返回: none.
//========================================================================
static void oled_swap(uint8* a, uint8* b)
{
	uint8 tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

//========================================================================
// 描述: OLED绘制线段.
// 参数: x0,y0:起始坐标; x1,y1:终止坐标; pixel:亮/灭
// 返回: none.
//========================================================================
void oled_show_line(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel)
{
	uint8 t;
	int16 xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int16 incx, incy, uRow, uCol;

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
        oled_set_pixel(uRow, uCol,pixel);   //画点
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
// 描述: OLED绘制矩形框.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; pixel:亮/灭;
// 返回: none.
//========================================================================
void oled_show_rectangle(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel)
{
	oled_show_line(x0, y0, x1, y0,pixel);
	oled_show_line(x0, y0, x0, y1,pixel);
	oled_show_line(x0, y1, x1, y1,pixel);
	oled_show_line(x1, y0, x1, y1,pixel);
}

//========================================================================
// 描述: 在指定位置填充矩形.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; pixel:亮/灭;
// 返回: none.
//========================================================================
void oled_fill_rectangle(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel)
{
    oled_fill(x0,y0,x1,y1,pixel);
}

//========================================================================
// 描述: 对称圆绘制算法(内部调用).
// 参数: xc:圆心的x坐标; yc:圆心的y坐标; x:相对于圆心的x坐标; y:相对于圆心的y坐标; 
//		 pixel:亮/灭
// 返回: none.
//========================================================================
void oled_show_circle_8(int16 xc, int8 yc, int16 x, int8 y, uint8 pixel)
{
	oled_set_pixel(xc + x, yc + y, pixel);

	oled_set_pixel(xc - x, yc + y, pixel);

	oled_set_pixel(xc + x, yc - y, pixel);

	oled_set_pixel(xc - x, yc - y, pixel);

	oled_set_pixel(xc + y, yc + x, pixel);

	oled_set_pixel(xc - y, yc + x, pixel);

	oled_set_pixel(xc + y, yc - x, pixel);

	oled_set_pixel(xc - y, yc - x, pixel);
}

//========================================================================
// 描述: 在指定位置绘制指定大小的圆.
// 参数: xc:圆心的x坐标; yc:圆心的y坐标; r:圆半径; pixel:亮/灭; fill:1-填充，0-不填充;
// 返回: none.
//========================================================================
void oled_show_circle(int16 xc, int8 yc, int8 r, uint8 pixel, uint8 fill)
{
	int16 x = 0, y = r, yi, d;

	d = 3 - 2 * r;

	if (fill)
	{
		// 如果填充（画实心圆）
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				oled_show_circle_8(xc, yc, x, yi, pixel);

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
			oled_show_circle_8(xc, yc, x, y, pixel);
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
// 参数: x0,y0,x1,y1,x2,y2:三角形三个端点的坐标; pixel:亮/灭.
// 返回: none.
//========================================================================
void oled_show_triangel(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 pixel)
{
	oled_show_line(x0, y0, x1, y1, pixel);
	oled_show_line(x1, y1, x2, y2, pixel);
	oled_show_line(x2, y2, x0, y0, pixel);
}

//========================================================================
// 描述: 在指定位置填充三角形.
// 参数: x0,y0,x1,y1,x2,y2:三角形三个端点的坐标; pixel:亮/灭.
// 返回: none.
//========================================================================
void oled_fill_triangel(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 pixel)
{
	uint8 a, b, y, last;
	int dx01, dy01, dx02, dy02, dx12, dy12;
	long sa = 0;
	long sb = 0;
	if (y0 > y1)
	{
		oled_swap(&y0, &y1);
		oled_swap(&x0, &x1);
	}
	if (y1 > y2)
	{
		oled_swap(&y2, &y1);
		oled_swap(&x2, &x1);
	}
	if (y0 > y1)
	{
		oled_swap(&y0, &y1);
		oled_swap(&x0, &x1);
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
		oled_fill(a, y0, b, y0, pixel);
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
			oled_swap(&a, &b);
		}
		oled_fill(a, y, b, y, pixel);
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
			oled_swap(&a, &b);
		}
		oled_fill(a, y, b, y, pixel);
	}
}

//========================================================================
// 描述: 显示BMP图片
// 参数: x0,y0:起点坐标; x1,y1:终点坐标; BMP:要显示的图片
// 返回: none.
//========================================================================
void oled_show_bmp(uint8 x0, uint8 y0,uint8 x1, uint8 y1,uint8* BMP)
{ 	
	uint16 j=0;
	uint8 x,y;
    y1 = y1 / 8;
	if(y1%8==0) y=y1/8;      
	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		for(x=x0;x<x1;x++)
		{
			_oled_disbuffer[x][y]=BMP[j++];
		}
	}
}

//========================================================================
// 描述: 初始化OLED
// 参数: none.
// 返回: none.
//========================================================================
void oled_init()
{ 	
	hardiic_init();
    delay(100);
    oled_wr_byte(0xAE,OLED_CMD);//关闭显示
	
	oled_wr_byte(0x40,OLED_CMD);//---set low column address
	oled_wr_byte(0xB0,OLED_CMD);//---set high column address

	oled_wr_byte(0xC8,OLED_CMD);//-not offset

	oled_wr_byte(0x81,OLED_CMD);//设置对比度
	oled_wr_byte(0xff,OLED_CMD);

	oled_wr_byte(0xa1,OLED_CMD);//段重定向设置

	oled_wr_byte(0xa6,OLED_CMD);//
	
	oled_wr_byte(0xa8,OLED_CMD);//设置驱动路数
	oled_wr_byte(0x1f,OLED_CMD);
	
	oled_wr_byte(0xd3,OLED_CMD);
	oled_wr_byte(0x00,OLED_CMD);
	
	oled_wr_byte(0xd5,OLED_CMD);
	oled_wr_byte(0xf0,OLED_CMD);
	
	oled_wr_byte(0xd9,OLED_CMD);
	oled_wr_byte(0x22,OLED_CMD);
	
	oled_wr_byte(0xda,OLED_CMD);
	oled_wr_byte(0x02,OLED_CMD);
	
	oled_wr_byte(0xdb,OLED_CMD);
	oled_wr_byte(0x49,OLED_CMD);
	
	oled_wr_byte(0x8d,OLED_CMD);
	oled_wr_byte(0x14,OLED_CMD);
	
	//oled_wr_byte(0xaf,OLED_CMD);
	oled_clear();
}  

#endif  //oled.h
