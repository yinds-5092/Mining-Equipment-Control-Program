/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef __LCD12864_DRAWING_H
#define __LCD12864_DRAWING_H
#include <stdio.h>
#include <STC8HX.h>
#include "delay.h"
#include <stdlib.h>
#include "lcdfont.h"	
#include <string.h>

// IO引脚定义
#ifndef LCD12864_RS
#define LCD12864_RS 		    P1_3
#endif

#ifndef LCD12864_RS_OUT
#define LCD12864_RS_OUT 	{P1M1&=~0x08;P1M0&=~0x08;}      //双向IO口
#endif 

#ifndef LCD12864_RW
#define LCD12864_RW 		    P1_0
#endif

#ifndef LCD12864_RW_OUT
#define LCD12864_RW_OUT 	{P1M1&=~0x01;P1M0&=~0x01;}      //双向IO口
#endif 

#ifndef LCD12864_EN
#define LCD12864_EN 		    P1_1
#endif

#ifndef LCD12864_E_OUT
#define LCD12864_E_OUT 	{P1M1&=~0x02;P1M0&=~0x02;}      //双向IO口
#endif 

#ifndef LCD12864_RST
#define LCD12864_RST         P5_4
#endif

#ifndef LCD12864_RST_OUT
#define LCD12864_RST_OUT 	{P5M1&=~0x10;P5M0&=~0x10;}      //双向IO口
#endif 

#ifndef LCD12864_DATA
#define LCD12864_DATA        P6
#endif

#ifndef LCD12864_DATA_OUT
#define LCD12864_DATA_OUT  {P6M1=0x00;P6M0=0x00;}          //双向IO口
#endif 

uint8 xdata _lcd12864_disbuffer[1024];

void lcd12864_init_drawing();    //初始化12864
void lcd12864_clear_drawing();  //清屏
void lcd12864_set_pixel_drawing(uint8 x, uint8 y, uint8 pixel);	//画点
void lcd12864_fill_drawing(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel);	//填充
void lcd12864_show_line_drawing(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel);	//画线
void lcd12864_show_rectangle_drawing(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel);	//画矩形
void lcd12864_fill_rectangle_drawing(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel);	//填充矩形
void lcd12864_show_circle_drawing(int16 xc, int8 yc, int8 r, uint8 pixel, uint8 fill);	//画圆
void lcd12864_show_triangel_drawing(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 pixel);	//画三角形
void lcd12864_fill_triangel_drawing(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 pixel);	//填充三角形
void lcd12864_show_char_drawing(int16 x,int16 y, uint8 chr, uint8 size, uint8 pixel);	//显示单个字符
void lcd12864_show_string_drawing(int8 x, int8 y, uint8 *chr, uint8 size, uint8 pixel);	//显示字符串
void lcd12864_show_num_drawing(int8 x,int8 y,int16 num,uint8 size, uint8 pixel);	//显示数字
void lcd12864_show_float_drawing(int16 x, int16 y, float num, uint8 precision, uint8 size, uint8 pixel);	//显示小数
void lcd12864_show_font12(uint8 lenth, uint8 *hz, int16 x, int16 y,uint8 pixel);	//显示汉字
void lcd12864_show_font16(uint8 lenth, uint8 *hz, int16 x, int16 y,uint8 pixel);	
void lcd12864_show_font24(uint8 lenth, uint8 *hz, int16 x, int16 y,uint8 pixel);
void lcd12864_show_font32(uint8 lenth, uint8 *hz, int16 x, int16 y,uint8 pixel);
void lcd12864_display_drawing();	//更新显示

//========================================================================
// 描述: lcd12864测忙碌
// 参数: none.
// 返回: 0，空闲，可写指令与数据; 非0, 忙，等待; .
//========================================================================
uint8 lcd12864_chek_busy()
{ 
    uint8 temp1;//状态信息（判断是否忙） 
    LCD12864_RS = 0;
    LCD12864_RW = 1;
    LCD12864_EN = 1;
    delay10us();
    temp1 =(uint8)(LCD12864_DATA&0x80);
    LCD12864_EN = 0;
    return temp1;
}

//========================================================================
// 描述: lcd12864写命令
// 参数: cmd:写入的命令.
// 返回: none.
//========================================================================
void lcd12864_write_command(uint8 cmd)/*写命令*/
{ 
    while(lcd12864_chek_busy());
    LCD12864_RS = 0;
    LCD12864_RW = 0;
    LCD12864_EN = 0;
    LCD12864_DATA = cmd;
    delay50us();
    LCD12864_EN = 1;
    delay50us();
    LCD12864_EN = 0;
}

//========================================================================
// 描述: lcd12864写数据
// 参数: dat:写入的数据.
// 返回: none.
//========================================================================
void lcd12864_write_data(uint8 dat)
{ 
    while(lcd12864_chek_busy());
    LCD12864_RS = 1;
    LCD12864_RW = 0;
    LCD12864_EN = 0;
    LCD12864_DATA = dat;
    delay50us();
    LCD12864_EN = 1;
    delay50us();
    LCD12864_EN = 0;
}

//========================================================================
// 描述: lcd12864读数据
// 参数: none
// 返回: 读取到的数据.
//========================================================================
uint8 lcd12864_read_data()
{ 
    uint8 read;
    while(lcd12864_chek_busy());
    LCD12864_RS = 1;
    LCD12864_RW = 1;
    LCD12864_EN = 0;
    LCD12864_DATA = 0xff;
    // delay50us();
    LCD12864_EN = 1;
    delay50us();
    read = LCD12864_DATA;
    LCD12864_EN = 0;
    // delay50us();
    return read;
}

//========================================================================
// 描述: lcd12864初始化
// 参数: none.
// 返回: none.
//========================================================================
void lcd12864_init_drawing()
{ 
    //引脚配置
    LCD12864_RS_OUT;
    LCD12864_RW_OUT;
    LCD12864_E_OUT;
    LCD12864_RST_OUT;  
    LCD12864_DATA_OUT;

    LCD12864_RST = 1;  
    LCD12864_RST = 0;             
    LCD12864_RST = 1;                //复位RST=1 
    lcd12864_write_command(0x34);         //34H--扩充指令操作
    delay5us();
    lcd12864_write_command(0x30);         //功能设置，一次送8位数据，基本指令集
    delay5us();
    lcd12864_write_command(0x0C);         //显示状态开关：整体显示开，光标显示关，光标显示反白关
    delay5us();
    lcd12864_write_command(0x01);         //清DDRAM
    delay5us();      
    lcd12864_write_command(0x02);         //DDRAM地址归位
    delay5us(); 
    lcd12864_write_command(0x80);         //设定DDRAM 7位地址000，0000到地址计数器AC
    delay5us();
}

//========================================================================
// 描述: lcd12864清屏(绘图模式)
// 参数: none.
// 返回: none.
//========================================================================
void lcd12864_clear_drawing()
{
    uint8 i,j;
    uint16 count;
    for(count=0;count<1024;count++){
        _lcd12864_disbuffer[count] = 0;
    }
    // lcd12864_write_command(0x34);
    // for(i=0;i<32;i++){
    //     lcd12864_write_command(0x80+i);
    //     lcd12864_write_command(0x80);
    //     for(j=0;j<32;j++){
    //         lcd12864_write_data(0);
    //     }
    // }
    // lcd12864_write_command(0x36);
    // lcd12864_write_command(0x36);

}

//========================================================================
// 描述: 在指定位置显示一个点（使用内部缓存，会擦除内部字库显示的字符）
// 参数: x,y:坐标;pixel:亮/灭.
// 返回: none.
//========================================================================
void lcd12864_set_pixel_drawing(uint8 x, uint8 y, uint8 pixel) 
{
    uint16 location = 0;

    if (x > 127 || y > 63) {
        return;
    }
    location = 16*y + (x/8);
    if(pixel){
        _lcd12864_disbuffer[location] |= (0x80 >> (x % 8));
    }else{
        _lcd12864_disbuffer[location] &= ~(0x80 >> (x % 8));
    }
}

//========================================================================
// 描述: 交换函数（内部调用）.
// 参数: a,b:要交换的值的指针.
// 返回: none.
//========================================================================
static void lcd12864_swap(uint8* a, uint8* b)
{
	uint8 tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

//========================================================================
// 描述: lcd12864填充
// 参数: dot:
// 返回: none.
//========================================================================
void lcd12864_fill_drawing(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel)
{
    uint8 i,j,swap;
    if (x0 > 127 || y0 > 63) return;
    if (x1 > 127 || y1 > 63) return;
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
            lcd12864_set_pixel_drawing(i, j, pixel);
        }
}

//========================================================================
// 描述: lcd12864绘制线段.
// 参数: x0,y0:起始坐标; x1,y1:终止坐标.
// 返回: none.
//========================================================================
void lcd12864_show_line_drawing(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel)
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
        lcd12864_set_pixel_drawing(uRow, uCol, pixel);   //画点
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
// 描述: lcd12864绘制矩形框.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; pixel:亮/灭;
// 返回: none.
//========================================================================
void lcd12864_show_rectangle_drawing(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel)
{
	lcd12864_show_line_drawing(x0, y0, x1, y0, pixel);
	lcd12864_show_line_drawing(x0, y0, x0, y1, pixel);
	lcd12864_show_line_drawing(x0, y1, x1, y1, pixel);
	lcd12864_show_line_drawing(x1, y0, x1, y1, pixel);
}

//========================================================================
// 描述: 在指定位置填充矩形.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; pixel:亮/灭;
// 返回: none.
//========================================================================
void lcd12864_fill_rectangle_drawing(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel)
{
    lcd12864_fill_drawing(x0,y0,x1,y1, pixel);
}

//========================================================================
// 描述: 对称圆绘制算法(内部调用).
// 参数: xc:圆心的x坐标; yc:圆心的y坐标; x:相对于圆心的x坐标; y:相对于圆心的y坐标; 
//		 pixel:亮/灭
// 返回: none.
//========================================================================
void lcd12864_show_circle_8_drawing(int16 xc, int8 yc, int16 x, int8 y, uint8 pixel)
{
	lcd12864_set_pixel_drawing(xc + x, yc + y, pixel);

	lcd12864_set_pixel_drawing(xc - x, yc + y, pixel);

	lcd12864_set_pixel_drawing(xc + x, yc - y, pixel);

	lcd12864_set_pixel_drawing(xc - x, yc - y, pixel);

	lcd12864_set_pixel_drawing(xc + y, yc + x, pixel);

	lcd12864_set_pixel_drawing(xc - y, yc + x, pixel);

	lcd12864_set_pixel_drawing(xc + y, yc - x, pixel);

	lcd12864_set_pixel_drawing(xc - y, yc - x, pixel);
}

//========================================================================
// 描述: 在指定位置绘制指定大小的圆.
// 参数: xc:圆心的x坐标; yc:圆心的y坐标; r:圆半径;fill:1-填充，0-不填充;
// 返回: none.
//========================================================================
void lcd12864_show_circle_drawing(int16 xc, int8 yc, int8 r, uint8 pixel, uint8 fill)
{
	int16 x = 0, y = r, yi, d;

	d = 3 - 2 * r;

	if (fill)
	{
		// 如果填充（画实心圆）
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				lcd12864_show_circle_8_drawing(xc, yc, x, yi, pixel);

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
			lcd12864_show_circle_8_drawing(xc, yc, x, y, pixel);
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
// 参数: x0,y0,x1,y1,x2,y2:三角形三个端点的坐标.
// 返回: none.
//========================================================================
void lcd12864_show_triangel_drawing(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 pixel)
{
	lcd12864_show_line_drawing(x0, y0, x1, y1, pixel);
	lcd12864_show_line_drawing(x1, y1, x2, y2, pixel);
	lcd12864_show_line_drawing(x2, y2, x0, y0, pixel);
}

//========================================================================
// 描述: 在指定位置填充三角形.
// 参数: x0,y0,x1,y1,x2,y2:三角形三个端点的坐标.
// 返回: none.
//========================================================================
void lcd12864_fill_triangel_drawing(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 pixel)
{
	uint8 a, b, y, last;
	int16 dx01, dy01, dx02, dy02, dx12, dy12;
	long sa = 0;
	long sb = 0;
	if (y0 > y1)
	{
		lcd12864_swap(&y0, &y1);
		lcd12864_swap(&x0, &x1);
	}
	if (y1 > y2)
	{
		lcd12864_swap(&y2, &y1);
		lcd12864_swap(&x2, &x1);
	}
	if (y0 > y1)
	{
		lcd12864_swap(&y0, &y1);
		lcd12864_swap(&x0, &x1);
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
		lcd12864_fill_drawing(a, y0, b, y0, pixel);
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
			lcd12864_swap(&a, &b);
		}
		lcd12864_fill_drawing(a, y, b, y, pixel);
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
			lcd12864_swap(&a, &b);
		}
		lcd12864_fill_drawing(a, y, b, y, pixel);
	}
}

//========================================================================
// 描述: lcd12864更新显示
// 参数: p:要显示的数组.
// 返回: none.
//========================================================================
void lcd12864_display_drawing()
{
    int16 ygroup,x,y,i;
    int16 temp;
    int16 tmp;
             
    for(ygroup=0;ygroup<64;ygroup++)           
    {                   
        if(ygroup<32)                   //写入液晶上半图象部分
        {
            x=0x80;
            y=ygroup+0x80;
        }else 
        {
            x=0x88;
            y=ygroup-32+0x80;    
        }         
        // lcd12864_write_command(0x34);        //写入扩充指令命令
		lcd12864_write_command(0x36);        //显示图象
        lcd12864_write_command(y);           //写入y轴坐标
        lcd12864_write_command(x);           //写入x轴坐标
        tmp=ygroup*16;
        for(i=0;i<16;i++)
		{
		    temp=_lcd12864_disbuffer[tmp++];
		    lcd12864_write_data(temp);
        }
    }
	lcd12864_write_command(0x30);        //写入基本指令命令
}

//========================================================================
// 描述: 在指定位置显示一个字符（绘图模式）.
// 参数: x:起始x坐标; y:起始y坐标; chr:显示的字符;
//       size:显示字符的大小（12/16）; pixel:亮/灭	  
// 返回: none.
//========================================================================
void lcd12864_show_char_drawing(int16 x,int16 y, uint8 chr, uint8 size, uint8 pixel)
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
                    if(temp&0x01){
                        lcd12864_set_pixel_drawing(x+i,y+j, pixel);
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
				if(temp&0x01){
					lcd12864_set_pixel_drawing(x+i,y+j, pixel);
				}
				temp>>=1; 
		    }
		}	
	}	   	 	  
}

//========================================================================
// 描述: 显示一个字符串
// 参数: x,y:起点坐标;chr:要显示的字符串
// 返回: none.
//========================================================================
void lcd12864_show_string_drawing(int8 x, int8 y, uint8 *chr, uint8 size, uint8 pixel)
{
    while((*chr<='~')&&(*chr>=' '))		//判断是不是非法字符	
    {       
        lcd12864_show_char_drawing(x,y,*chr,size,pixel);
        x+=size/2;
        chr++;
    }  
}

//========================================================================
// 描述: 在指定位置显示数字
// 参数: x,y:坐标; num:显示的数字; 
// 返回: none.
//========================================================================
void lcd12864_show_num_drawing(int8 x,int8 y,int16 num,uint8 size, uint8 pixel)
{
	char men[15];
	//char *str = men;
	//sprintf(str,"%d",num);
    _itoa(num,men,10);
	lcd12864_show_string_drawing(x,y,men,size, pixel);
} 

//========================================================================
// 描述: 在指定位置显示小数.
// 参数: x:起始x坐标; y:起始y坐标; num:要显示的数字;precision:精度;
//       font_color:字符串的颜色值; background_color:背景色; size:显示的大小（12/16） mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
void lcd12864_show_float_drawing(int16 x, int16 y, float num, uint8 precision, uint8 size, uint8 pixel)
{
    uint8 i;
	uint8 float_bit;
    int16 int_part;
	int_part = (int16)num/1;

    if(num>=0)			//整数部分
    {
		lcd12864_show_num_drawing( (int8)x, (int8)y, int_part, size, pixel);
        for(i=1;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
		num = num - int_part;
		lcd12864_show_char_drawing(x+(size/2)*i, (int8)y, '.', size, pixel);	//显示小数点
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+1)), (int8)y,(int16)float_bit, size, pixel);	
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+1)), (int8)y,(int16)float_bit, size, pixel);
				float_bit = (int32)(num*100)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+2)), (int8)y,(int16)float_bit, size, pixel);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+1)), (int8)y,(int16)float_bit, size, pixel);
				float_bit = (int32)(num*100)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+2)), (int8)y,(int16)float_bit, size, pixel);
				float_bit = (int32)(num*1000)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+3)), (int8)y,(int16)float_bit, size, pixel);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+1)), (int8)y,(int16)float_bit, size, pixel);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+2)), (int8)y,(int16)float_bit, size, pixel);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+3)), (int8)y,(int16)float_bit, size, pixel);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+4)), (int8)y,(int16)float_bit, size, pixel);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+1)), (int8)y,(int16)float_bit, size, pixel);	
			break;//保留1位精度
		}
    }
    else
    {
		i = 0;
		if(int_part == 0)
		{
			lcd12864_show_char_drawing((int8)x, (int8)y, '-', size, pixel);
			i++;
		}
		lcd12864_show_num_drawing( (int8)(x+(size/2)*i), (int8)y, int_part, size, pixel);

        for(i=1;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
		num = -(num - int_part);
		lcd12864_show_char_drawing(x+(size/2)*(i+1), y, '.', size, pixel);	//显示小数点
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+2)), (int8)y,(int16)float_bit, size, pixel);	
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+2)), (int8)y,(int16)float_bit, size, pixel);
				float_bit = (int32)(num*100)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+3)), (int8)y,(int16)float_bit, size, pixel);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+2)), (int8)y,(int16)float_bit, size, pixel);
				float_bit = (int32)(num*100)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+3)), (int8)y,(int16)float_bit, size, pixel);
				float_bit = (int32)(num*1000)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+4)), (int8)y,(int16)float_bit, size, pixel);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+2)), (int8)y,(int16)float_bit, size, pixel);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+3)), (int8)y,(int16)float_bit, size, pixel);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+4)), (int8)y,(int16)float_bit, size, pixel);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+5)), (int8)y,(int16)float_bit, size, pixel);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				lcd12864_show_num_drawing((int8)(x+(size/2)*(i+2)), (int8)y,(int16)float_bit, size, pixel);	
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
void lcd12864_show_font12(uint8 lenth, uint8 *hz, int16 x, int16 y,uint8 pixel)
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
                        if(hz[zz*24 + i * 2 + j/8] & (0x80>>(j % 8))){
                            lcd12864_set_pixel_drawing(zz * 12+ x + j,y + i, pixel);
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
					if(hz[zz*24 + i * 2 + j/8] & (0x80>>(j % 8))){
						lcd12864_set_pixel_drawing(zz * 12+ x + j,y + i, pixel);					
					}
				}
			}
		}
	}
}

//========================================================================
// 描述: 在指定位置显示16*16字体汉字.
// 参数: hz:汉字的指针; x:起始x坐标; y:起始y坐标; lenth: 字体的总长度 
//       font_color:显示字符的颜色值; background_color:显示字符的背景色;mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
void lcd12864_show_font16(uint8 lenth, uint8 *hz, int16 x, int16 y,uint8 pixel)
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
						if(hz[zz*32 + i * 2 + j/8] & (0x80>>(j % 8))){
                            lcd12864_set_pixel_drawing(zz * 16+ x + j,y + i, pixel);						
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
					if(hz[zz*32 + i * 2 + j/8] & (0x80>>(j % 8))){
						lcd12864_set_pixel_drawing(zz * 16+ x + j,y + i, pixel);
					}					
				}
			}
		}
	}
}
//========================================================================
// 描述: 在指定位置显示24*24字体汉字.
// 参数: hz:汉字的指针; x:起始x坐标; y:起始y坐标; lenth: 字体的总长度 
//       font_color:显示字符的颜色值; background_color:显示字符的背景色;mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
void lcd12864_show_font24(uint8 lenth, uint8 *hz, int16 x, int16 y,uint8 pixel)
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
                        if(hz[zz*72 + i * 3 + j/8] & (0x80>>(j % 8))){
                            lcd12864_set_pixel_drawing(zz * 24+ x + j,y + i, pixel);
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
					if(hz[zz*72 + i * 3 + j/8] & (0x80>>(j % 8))){
                        lcd12864_set_pixel_drawing(zz * 24+ x + j,y + i, pixel);
					}
				}
			}
		}
	}
}
//========================================================================
// 描述: 在指定位置显示32*32字体汉字.
// 参数: hz:汉字的指针; x:起始x坐标; y:起始y坐标; lenth: 字体的总长度 
//       font_color:显示字符的颜色值; background_color:显示字符的背景色;mode:0-无叠加，1-叠加.
// 返回: none.
//========================================================================
void lcd12864_show_font32(uint8 lenth, uint8 *hz, int16 x, int16 y,uint8 pixel)
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
						if (hz[zz * 128 + j * 4 + i/ 8] & (0x80 >> (i % 8))){
                            lcd12864_set_pixel_drawing(zz * 32+ x + j,y + i, pixel);
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
			for(i = 0;i < 32;i++)
			{
				for(j = 0;j < 32;j++)
				{
					if((zz * 32 + j + x -32) > 239)return;
					if((y + i -32) > 339)return;
                    if (hz[zz * 128 + j * 4 + i/ 8] & (0x80 >> (i % 8))){
                        lcd12864_set_pixel_drawing(zz * 32+ x + j,y + i, pixel);
					}
				}
			}
		}
	}
}

//========================================================================
// 描述: lcd12864显示图像
// 参数: p:要显示的图像.
// 返回: none.
//========================================================================
void lcd12864_show_bmp_drawing(uint8 *p)
{
    int16 ygroup,x,y,i;
    int16 temp;
    int16 tmp;
             
    for(ygroup=0;ygroup<64;ygroup++)           
    {                   
        if(ygroup<32)                   //写入液晶上半图象部分
        {
            x=0x80;
            y=ygroup+0x80;
        }else 
        {
            x=0x88;
            y=ygroup-32+0x80;    
        }         
        lcd12864_write_command(0x34);        //写入扩充指令命令
        lcd12864_write_command(y);           //写入y轴坐标
        lcd12864_write_command(x);           //写入x轴坐标
        lcd12864_write_command(0x30);        //写入基本指令命令
        tmp=ygroup*16;
        for(i=0;i<16;i++)
		{
		    temp=p[tmp++];
		    lcd12864_write_data(temp);
        }
    }
    lcd12864_write_command(0x34);        //写入扩充指令命令
    lcd12864_write_command(0x36);        //显示图象
}

#endif      //lcd12864.h