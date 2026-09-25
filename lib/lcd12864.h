/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef __LCD12864_H
#define __LCD12864_H
#include <stdio.h>
#include <STC8HX.h>
#include "delay.h"
#include <stdlib.h>
	
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

void lcd12864_init();    //初始化12864
void lcd12864_clear();   //12864清屏
void lcd12864_show_char(uint8 x, uint8 y, uint8 sig);   //12864在指定位置显示一个字符
void lcd12864_show_string(uint8 x, uint8 y, uint8 *str);    //12864在指定位置显示字符串
void lcd12864_show_num(uint8 x, uint8 y, int num);   //12864在指定位置显示数字
void lcd12864_show_float(uint8 x, uint8 y, float num, uint8 precision); //显示小数
void lcd12864_show_bmp(uint8 *p);    //12864显示图片


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
void lcd12864_init()
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
// 描述: lcd12864设定显示位置
// 参数: X,Y:坐标.
// 返回: none.
//========================================================================
void lcd12864_display_pos(uint8 X,uint8 Y)
{                          
   uint8  pos;
   if (X==0)
     {X=0x80;}
   else if (X==1)
     {X=0x90;}
   else if (X==2)
     {X=0x88;}
   else if (X==3)
     {X=0x98;}
   pos = X+Y ;  
   lcd12864_write_command(pos);     //显示地址
}

//========================================================================
// 描述: lcd12864清屏
// 参数: none.
// 返回: none.
//========================================================================
void lcd12864_clear()
{
    lcd12864_write_command(0x30);//
    lcd12864_write_command(0x01);//清除显示
    delay(1);
}

//========================================================================
// 描述: lcd12864写单个字符(使用内部字库)
// 参数: X,Y;坐标; sig:要显示的字符.
// 返回: none.
//========================================================================
void lcd12864_show_char(uint8 x, uint8 y, uint8 sig)
{    
    lcd12864_display_pos(x,y); 
    lcd12864_write_data(sig);             //输出单个字符
}

//========================================================================
// 描述: lcd12864写字符串(使用内部字库)
// 参数: X(0~3)行,Y(0~7)个字符; str:要显示的字符串;
// 返回: none.
//========================================================================
void lcd12864_show_string(uint8 x, uint8 y, uint8 *str)
{ 
    uint8 i = 0;
    lcd12864_display_pos(x,y);
    while(str[i] != '\0')
    {
        lcd12864_write_data(str[i]);
        i++;
    }
}

//========================================================================
// 描述: lcd12864写数字(使用内部字库)
// 参数: X(0~3)行,Y(0~7)个字符; num:要显示的数字;
// 返回: none.
//========================================================================
void lcd12864_show_num(uint8 x, uint8 y, int num)
{
	char men[15];
    _itoa(num,men,10);
	lcd12864_show_string(x,y,men);
} 

//========================================================================
// 描述: 显示小数(使用内部字库)
// 参数: x,y:起点坐标;num:要显示的数值;precision:精度.
// 返回: none.
//========================================================================
void lcd12864_show_float(uint8 x, uint8 y, float num, uint8 precision)
{
    char men[15];
    uint8 i;
    uint8 float_bit;
    int16 int_part;
	int_part = (int16)num/1;
    lcd12864_display_pos(x,y);
    
    if(num >= 0)			//整数部分
    {
        _itoa(int_part,men,10);
		
        for(i=1;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
        men[i] = '.';
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+1],10);
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+1],10);
				float_bit = (int32)(num*100)%10;
				_itoa(float_bit,&men[i+2],10);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+1],10);
				float_bit = (int32)(num*100)%10;
				_itoa(float_bit,&men[i+2],10);
				float_bit = (int32)(num*1000)%10;
				_itoa(float_bit,&men[i+3],10);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				_itoa(float_bit,&men[i+1],10);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				_itoa(float_bit,&men[i+2],10);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				_itoa(float_bit,&men[i+3],10);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				_itoa(float_bit,&men[i+4],10);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+1],10);
			break;//保留1位精度
		}

        i = 0;
        while(men[i] != '\0')
        {
            lcd12864_write_data(men[i]);
            i++;
        }
    }
    else
    {
        if(int_part == 0){
            men[0] = '-';
            men[1] = '0';
            i = 1;
        }else{
            _itoa(int_part,men,10);
            for(i=1;i<10;i++)
            {
                int_part=int_part/10;
                if(int_part == 0)
                {
                    break;
                }
            }
        }
        num = -num;
        men[i+1] = '.';
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+2],10);
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+2],10);
				float_bit = (int32)(num*100)%10;
				_itoa(float_bit,&men[i+3],10);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+2],10);
				float_bit = (int32)(num*100)%10;
				_itoa(float_bit,&men[i+3],10);
				float_bit = (int32)(num*1000)%10;
				_itoa(float_bit,&men[i+4],10);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				_itoa(float_bit,&men[i+2],10);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				_itoa(float_bit,&men[i+3],10);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				_itoa(float_bit,&men[i+4],10);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				_itoa(float_bit,&men[i+5],10);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+2],10);
			break;//保留1位精度
		}
        i = 0;
        while(men[i] != '\0')
        {
            lcd12864_write_data(men[i]);
            i++;
        }
    }
} 

//========================================================================
// 描述: lcd12864显示图像
// 参数: p:要显示的图像.
// 返回: none.
//========================================================================
void lcd12864_show_bmp(uint8 *p)
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

/*****************************************************************************************
 *  该部分为使用绘图模式，直接控制lcd12864内部RAM显示图案（整屏刷新慢）               
 * 
//========================================================================
// 描述: lcd12864清屏
// 参数: none.
// 返回: none.
//========================================================================
void lcd12864_clear_drawing()
{
    uint8 i,j;
    lcd12864_write_command(0x34);
    for(i=0;i<32;i++){
        lcd12864_write_command(0x80+i);
        lcd12864_write_command(0x80);
        for(j=0;j<32;j++){
            lcd12864_write_data(0);
        }
    }
    lcd12864_write_command(0x36);
    lcd12864_write_command(0x36);
}

//========================================================================
// 描述: 在指定位置显示一个点（不会擦除使用内部字库函数显示的字符，不过整屏刷新慢）
// 参数: x,y:坐标;pixel:亮/灭.
// 返回: none.
//========================================================================
void lcd12864_set_pixel(uint8 x, uint8 y, uint8 pixel) 
{
    uint8 i=0;
    uint8 j=0;
    uint16 num = 0;
    uint16 last_num = 0;

    j = 0x80+y%32;
    if(y > 31)i = 0x88+x/16;
    else i = 0x80+x/16;
    num = (pixel>0?0x8000:0)>>(x%16);

    lcd12864_write_command(0x34);
    lcd12864_write_command(0x34);        //写入扩充指令命令
    lcd12864_write_command(j);     
    lcd12864_write_command(i);   //写入坐标

    lcd12864_read_data();

    last_num = lcd12864_read_data();
    last_num <<= 8;

    last_num |= lcd12864_read_data();
    if(pixel >= 1)num = num | last_num;
    else num = num & last_num;

    lcd12864_write_command(j);     
    lcd12864_write_command(i);   //写入坐标
    lcd12864_write_data(num>>8);
    lcd12864_write_data(num);

   lcd12864_write_command(0x36); 
   lcd12864_write_command(0x30);        //写入基本指令命令  
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
void lcd12864_fill(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel)
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
            lcd12864_set_pixel(i, j, pixel);
        }
}

//========================================================================
// 描述: lcd12864绘制线段.
// 参数: x0,y0:起始坐标; x1,y1:终止坐标.
// 返回: none.
//========================================================================
void lcd12864_show_line(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel)
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
        lcd12864_set_pixel(uRow, uCol, pixel);   //画点
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
void lcd12864_show_rectangle(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel)
{
	lcd12864_show_line(x0, y0, x1, y0, pixel);
	lcd12864_show_line(x0, y0, x0, y1, pixel);
	lcd12864_show_line(x0, y1, x1, y1, pixel);
	lcd12864_show_line(x1, y0, x1, y1, pixel);
}

//========================================================================
// 描述: 在指定位置填充矩形.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; pixel:亮/灭;
// 返回: none.
//========================================================================
void lcd12864_fill_rectangle(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 pixel)
{
    lcd12864_fill(x0,y0,x1,y1, pixel);
}

//========================================================================
// 描述: 对称圆绘制算法(内部调用).
// 参数: xc:圆心的x坐标; yc:圆心的y坐标; x:相对于圆心的x坐标; y:相对于圆心的y坐标; 
//		 pixel:亮/灭
// 返回: none.
//========================================================================
void lcd12864_show_circle_8(int16 xc, int8 yc, int16 x, int8 y, uint8 pixel)
{
	lcd12864_set_pixel(xc + x, yc + y, pixel);

	lcd12864_set_pixel(xc - x, yc + y, pixel);

	lcd12864_set_pixel(xc + x, yc - y, pixel);

	lcd12864_set_pixel(xc - x, yc - y, pixel);

	lcd12864_set_pixel(xc + y, yc + x, pixel);

	lcd12864_set_pixel(xc - y, yc + x, pixel);

	lcd12864_set_pixel(xc + y, yc - x, pixel);

	lcd12864_set_pixel(xc - y, yc - x, pixel);
}

//========================================================================
// 描述: 在指定位置绘制指定大小的圆.
// 参数: xc:圆心的x坐标; yc:圆心的y坐标; r:圆半径;fill:1-填充，0-不填充;
// 返回: none.
//========================================================================
void lcd12864_show_circle(int16 xc, int8 yc, int8 r, uint8 pixel, uint8 fill)
{
	int16 x = 0, y = r, yi, d;

	d = 3 - 2 * r;

	if (fill)
	{
		// 如果填充（画实心圆）
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				lcd12864_show_circle_8(xc, yc, x, yi, pixel);

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
			lcd12864_show_circle_8(xc, yc, x, y, pixel);
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
void lcd12864_show_triangel(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 pixel)
{
	lcd12864_show_line(x0, y0, x1, y1, pixel);
	lcd12864_show_line(x1, y1, x2, y2, pixel);
	lcd12864_show_line(x2, y2, x0, y0, pixel);
}

//========================================================================
// 描述: 在指定位置填充三角形.
// 参数: x0,y0,x1,y1,x2,y2:三角形三个端点的坐标.
// 返回: none.
//========================================================================
void lcd12864_fill_triangel(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 pixel)
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
		lcd12864_fill(a, y0, b, y0, pixel);
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
		lcd12864_fill(a, y, b, y, pixel);
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
		lcd12864_fill(a, y, b, y, pixel);
	}
}
********************************************************************************************/

#endif      //lcd12864.h