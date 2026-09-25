/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/


#ifndef __MATRIX_H
#define __MATRIX_H

#include <STC8HX.h>
#include "delay.h"
#include "hc595.h"
#include "oledfont.h"

#ifndef MATRIX_PORT
#define MATRIX_PORT         P6  
#endif

#ifndef MATRIX_PORT_MODE
#define MATRIX_PORT_MODE  {P6M1=0x00;P6M0=0xff;}//推挽输出
#endif 

xdata maxbuf, thebuf,matrix_speed;//最大显示缓存，当前缓存指针
//显示缓存
xdata uint8 _matrix_display_buf[64]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

void matrix_init(); //点阵屏初始化
void matrix_scan_callback();    //点阵屏扫描回调函数(需要在1ms定时器中断函数里调用,总周期16ms<视觉20ms)
void matrix_set_pixel(uint8 x,uint8 y,uint8 state); //点阵屏设置指定点亮灭状态
void matrix_update_buf(uint8 *from);    //点阵屏显示缓存更新数据
void matrix_clear();    //点阵屏清屏
void matrix_display_string(uint8 *chr); //显示字符串
void matrix_display_num(int16 num); //显示数字
void matrix_display_float(float num, uint8 precision); //显示小数

//========================================================================
// 描述: 点阵屏初始化.
// 参数: none.
// 返回: none.
//========================================================================
void matrix_init()
{
    uint8 _i;
    hc595_init();
    MATRIX_PORT_MODE;//推挽输出
    MATRIX_PORT=0xff;
    maxbuf = 0;
    thebuf = 0;
    matrix_speed = 10;
    for (_i = 0; _i < 64; _i++)_matrix_display_buf[_i] = 0xff;
}

//========================================================================
// 描述: 点阵屏显示缓存数据.
// 参数: (0~15),0：小数点不显示；1：小数点显示.
// 返回: none.
//========================================================================
void matrix_display(uint8 data)
{
    MATRIX_PORT=data;
}

//========================================================================
// 描述: 点阵屏扫描回调函数(需要在1ms定时器中断函数里调用,总周期16ms<视觉20ms).
// 参数: none.
// 返回: none.
//========================================================================
void matrix_scan_callback()
{
    static uint8 flag;
    static uint8 index = 8;
    static uint8 temp_speed;
    
    

    flag=~flag;

    if(flag)
    {
        matrix_display(_matrix_display_buf[index-8+thebuf]);//显示段码
        hc595_bit_select(15-index+8);//显示位码
        index++;
        if(index > 15)    
        {
            temp_speed++;
            if (matrix_speed < temp_speed)
            {
                temp_speed = 0;
                if ((thebuf + 8) < maxbuf)
                {
                    thebuf++;
                }
                else {
                    thebuf = 0;

                }
            }

            index = 8;  
        }
    }
    else
    {
        hc595_disable();// 消影
    }  
}

//========================================================================
// 描述: 点阵屏显示点.
// 参数: x坐标（0-7），y坐标（0-7），1：亮；0：灭.
// (0,7)-------(7,7)
//   |           |
//   |           |
//   |           |
// (0,0)-------(7,0)
// 返回: none.
//========================================================================
void matrix_set_pixel(uint8 x,uint8 y,uint8 state)
{
    if((x>7) || (y>7))
    {
        return;
    }
    if(state)
    {
        _matrix_display_buf[7-x] &= ~(0x80 >> y);
    }
    else
    {
        _matrix_display_buf[7-x] |= (0x80 >> y);
    }
    thebuf = 0;
    maxbuf = 0;
}

//========================================================================
// 描述: 点阵屏清屏.
// 参数: none.
// 返回: none.
//========================================================================
void matrix_clear()
{
    uint8 i;
    for(i=0;i<64;i++)
    {
        _matrix_display_buf[i]=0xff;
    }
}

//========================================================================
// 描述: 点阵屏显示缓存更新数据.
// 参数: 子模数组指针.
// 返回: none.
//========================================================================
void matrix_update_buf(uint8 *from)
{
    uint8 i;
    for(i=0;i<8;i++)
    {
        _matrix_display_buf[i]=*from;
        from++;
    }
    thebuf = 0;
    maxbuf = 0;
    
}

//========================================================================
// 描述: 点阵屏显示字符.
// 参数: 字符.
// 返回: none.
//========================================================================
void matrix_display_char(char c)
{
    uint8 i;
    for(i=0;i<5;i++)
    {
         _matrix_display_buf[i+thebuf]=fontx[c][i]^0xff;
       
    }
    thebuf = thebuf + 6;
    maxbuf = maxbuf + 6;
}

//========================================================================
// 描述: 点阵屏显示字符串.
// 参数: 字符串.
// 返回: none.
//========================================================================
void matrix_display_string(uint8 *chr)
{
	uint8 j=0;

    thebuf = 2;
    maxbuf = 0;
	while (chr[j]!='\0')
	{		

	    matrix_display_char(chr[j]);

		j++;
	}
}

//========================================================================
// 描述: 点阵屏显示数字(内部调用，不初始化显示位置).
// 参数: 数字.
// 返回: none.
//========================================================================
void _matrix_display_num(int16 num)
{
    int32 temp;
    char i;
    uint8 shu[7];
    if (num >= 0)
    {
        for (i = 0; i < 7; i++)
        {
            temp = num % 10;
            num = num / 10;
            shu[i] = temp;
            //matrix_display_char(0x30 + (uint8)temp);
            //_nix_display_buff[i] = temp;

            if (num == 0)
            {
                break;
            }
        }

     
        do{
            matrix_display_char(0x30 + shu[i]);
            i--;
        } while (i > -1);
        maxbuf = maxbuf + 2;

    }
    else//负数
    {
        num = -num;
        for (i = 0; i < 7; i++)
        {
            temp = num % 10;
            num = num / 10;
            shu[i] = temp;


            if (num == 0)
            {
                for (num = 0; num < 4; num++) 
                {
                _matrix_display_buf[thebuf] = 0xf7;
                thebuf++;
                maxbuf++;
               }

                break;
            }
        }

        do {
            matrix_display_char(0x30 + shu[i]);
            i--;
        } while (i > -1);
        maxbuf = maxbuf + 2;
    }
}

//========================================================================
// 描述: 点阵屏显示数字.
// 参数: 数字.
// 返回: none.
//========================================================================
void matrix_display_num(int16 num)
{
    thebuf = 2;
    maxbuf = 0;
    _matrix_display_num(num);
}

//========================================================================
// 描述: 显示小数(整数，小数部分范围-32768~+32767)
// 参数: num:要显示的数值;precision:精度.
// 返回: none.
//========================================================================
void matrix_display_float(float num, uint8 precision)
{
    int32 int_part,float_part;
	int_part = (int32)num/1;

    thebuf = 2;
    maxbuf = 0;

    if(int_part>=0)			//整数部分
    {
		switch(precision)
		{
			case 1:float_part = (int32)(num*10)%10;break;//保留1位精度
			case 2:float_part = (int32)(num*100)%100;break;//保留2位精度
			case 3:float_part = (int32)(num*1000)%1000;break;//保留3位精度
			case 4:float_part = (int32)(num*10000)%10000;break;//保留4位精度
			default:float_part = (int32)(num*10)%10;break;//保留1位精度
		}
		if(float_part<0)
		{
			float_part = -float_part;
		}
        _matrix_display_num(int_part);
		matrix_display_char('.');	//显示小数点
		_matrix_display_num(float_part);	//显示小数部分
    }
    else
    {
		switch(precision)
		{
			case 1:float_part = (int32)(num*10)%10;break;//保留1位精度
			case 2:float_part = (int32)(num*100)%100;break;//保留2位精度
			case 3:float_part = (int32)(num*1000)%1000;break;//保留3位精度
			case 4:float_part = (int32)(num*10000)%10000;break;//保留4位精度
			default:float_part = (int32)(num*10)%10;break;//保留1位精度
		}
		if(float_part<0)
		{
			float_part = -float_part;
		}
        _matrix_display_num(int_part);
		matrix_display_char('.');	//显示小数点
		_matrix_display_num(float_part);	//显示小数部分
    }
} 


#endif