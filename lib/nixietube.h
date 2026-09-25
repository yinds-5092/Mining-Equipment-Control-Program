/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef __NIXIETUBE_H
#define __NIXIETUBE_H

#include <STC8HX.h>
#include "hc595.h"

//段码
//0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,-,NONE
code uint8 _nix_seg[18]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E,0xBF,0xFF};
//显示缓存
xdata uint8 _nix_display_buff[8]={17,17,17,17,17,17,17,17};
//小数点位置
uint8 _nix_point_bit=0;
//冒号显示标志位
uint8 _nix_colon_flag=0;

#ifndef NIXIETUBE_PORT
#define NIXIETUBE_PORT         P6  
#endif

#ifndef NIXIETUBE_PORT_MODE
#define NIXIETUBE_PORT_MODE  {P6M1=0x00;P6M0=0xff;}//推挽输出
#endif 

#ifndef NIXIETUBE_LEFT_COLON_PIN          //左侧数码管冒号
#define NIXIETUBE_LEFT_COLON_PIN          P0_7
#endif

#ifndef NIXIETUBE_LEFT_COLON_PIN_MODE
#define NIXIETUBE_LEFT_COLON_PIN_MODE  {P0M1&=~0x80;P0M0|=0x80;}//推挽输出
#endif 

#ifndef NIXIETUBE_RIGHT_COLON_PIN          //右侧数码管冒号
#define NIXIETUBE_RIGHT_COLON_PIN          P2_1
#endif

#ifndef NIXIETUBE_RIGHT_COLON_PIN_MODE
#define NIXIETUBE_RIGHT_COLON_PIN_MODE  {P2M1&=~0x02;P2M0|=0x02;}//推挽输出
#endif 

void nix_init();    //数码管显示初始化
void nix_scan_callback();   // 数码管扫描回调函数(需要在1ms定时器中断函数里调用,总周期16ms<视觉20ms).
void nix_display_num(int32 num);    //数码管显示整数
void nix_display_float(float num, uint8 precision);     //数码管显示浮点数
void nix_display_time(uint8 hour,uint8 minute,uint8 dir); //数码管显示时间
void nix_display_time2(uint8 hour,uint8 minute,uint8 second); //数码管显示时间
void nix_disable_time();    //数码管不显示冒号
void nix_display_clear();   //数码管清屏
void nix_display_clear_bit(uint8 nbit); //数码管清指定位
void nix_display_update_buf(uint8 *buf); //数码管显示缓存更新数据
//========================================================================
// 描述: 数码管显示初始化.
// 参数: none.
// 返回: none.
//========================================================================
void nix_init()
{
    hc595_init();
    NIXIETUBE_PORT_MODE;//推挽输出
    NIXIETUBE_PORT=0xff;

    //左侧数码管冒号
    NIXIETUBE_LEFT_COLON_PIN_MODE;//推挽输出
    NIXIETUBE_LEFT_COLON_PIN = 1;

    //右侧侧数码管冒号
    NIXIETUBE_RIGHT_COLON_PIN_MODE;//推挽输出
    NIXIETUBE_RIGHT_COLON_PIN = 1;
}

//========================================================================
// 描述: 数码管位显示.
// 参数: (0~15),0：小数点不显示；1：小数点显示.
// 返回: none.
//========================================================================
void nix_display(uint8 data,uint8 point)
{
    if(point)
    {
        NIXIETUBE_PORT=_nix_seg[data] & 0x7F;
    }
    else
    {
        NIXIETUBE_PORT=_nix_seg[data];
    }  
}

//========================================================================
// 描述: 数码管扫描回调函数(需要在1ms定时器中断函数里调用,总周期16ms<视觉20ms).
// 参数: none.
// 返回: none.
//========================================================================
void nix_scan_callback()
{
    static uint8 flag;
    static uint8 index = 0;
    static uint16 count = 0;

    flag++;

    if(flag%2==0)
    {
        if(_nix_colon_flag)
        {
            count++;

            if(count>500)//1秒冒号切换状态
            {
                if(_nix_colon_flag == 1)
                {
                    NIXIETUBE_RIGHT_COLON_PIN^=1;
                }
                else
                {
                    NIXIETUBE_LEFT_COLON_PIN^=1;
                }
                count = 0;
            }
        }

        nix_display(_nix_display_buff[index],0x01&(_nix_point_bit>>index));//显示段码
        hc595_bit_select(index);//显示位码
        index++;
        if(index > 7)    
        {
            index = 0;  
        }
    }
    else
    {
        hc595_disable();// 消影
    }  
}

//========================================================================
// 描述: 数码管显示整数.
// 参数: (-9999999~99999999).
// 返回: none.
//========================================================================
void nix_display_num(int32 num)
{
    int32 temp;
    uint8 i;

    nix_disable_time();//冒号不显示

    if(num>=0)
    {
        for(i=0;i<8;i++)
        {
            temp=num%10;
            num=num/10;
            _nix_display_buff[i]=temp;
            if(num == 0)
            {
                break;
            }
        }
    }
    else//负数
    {
        num = -num;
        for(i=0;i<8;i++)
        {
            temp=num%10;
            num=num/10;
            _nix_display_buff[i]=temp;
            if(num == 0)
            {
                _nix_display_buff[i+1]=16;//添加负号
                break;
            }
        }
    }
}

//========================================================================
// 描述: 数码管显示浮点数.
// 参数: 浮点数,精度(1,2,3,4).
// 返回: none.
//========================================================================
void nix_display_float(float num, uint8 precision)
{
    uint8 i;
    uint8 float_bit;
    int32 int_part;
    int32 temp;

    nix_disable_time();//冒号不显示
    i = 0;
    int_part = (int32)num/1;
    if(num>=0)			
    {
        switch(precision)
        {
            case 1: //保留1位精度
                float_bit = (int32)(num*10)%10;
                _nix_display_buff[0]=float_bit;
                i+=1;
            break;
            case 2: //保留2位精度
                float_bit = (int32)(num*10)%10;
                 _nix_display_buff[1]=float_bit;
                float_bit = (int32)(num*100)%10;
                 _nix_display_buff[0]=float_bit;
                i+=2;
            break;
            case 3: //保留3位精度
                float_bit = (int32)(num*10)%10;
                _nix_display_buff[2]=float_bit;
                float_bit = (int32)(num*100)%10;
                _nix_display_buff[1]=float_bit;
                float_bit = (int32)(num*1000)%10;
                _nix_display_buff[0]=float_bit;
                i+=3;
            break;
            case 4: //保留4位精度
                float_bit = (int32)(num*10)%10;
                 _nix_display_buff[3]=float_bit;
                float_bit = (int32)(num*100)%10;
                 _nix_display_buff[2]=float_bit;
                float_bit = (int32)(num*1000)%10;
                 _nix_display_buff[1]=float_bit;
                float_bit = (int32)(num*10000)%10;
                 _nix_display_buff[0]=float_bit;
                i+=4;
            break;
            default:    //保留1位精度
                float_bit = (int32)(num*10)%10;
                _nix_display_buff[0]=float_bit;
                i+=1;
            break;
        }
        _nix_point_bit = (0x01 <<(i));//显示小数点
        for(;i<8;i++)
        {
            temp=int_part%10;
            int_part=int_part/10;
            _nix_display_buff[i]=temp;
            if(int_part == 0)
            {
                break;
            }
        }
    }else
    {
        num  = -num;
        int_part = -int_part;
        switch(precision)
        {
            case 1: //保留1位精度
                float_bit = (int32)(num*10)%10;
                _nix_display_buff[0]=float_bit;
                i+=1;
            break;
            case 2: //保留2位精度
                float_bit = (int32)(num*10)%10;
                 _nix_display_buff[1]=float_bit;
                float_bit = (int32)(num*100)%10;
                 _nix_display_buff[0]=float_bit;
                i+=2;
            break;
            case 3: //保留3位精度
                float_bit = (int32)(num*10)%10;
                _nix_display_buff[2]=float_bit;
                float_bit = (int32)(num*100)%10;
                _nix_display_buff[1]=float_bit;
                float_bit = (int32)(num*1000)%10;
                _nix_display_buff[0]=float_bit;
                i+=3;
            break;
            case 4: //保留4位精度
                float_bit = (int32)(num*10)%10;
                 _nix_display_buff[3]=float_bit;
                float_bit = (int32)(num*100)%10;
                 _nix_display_buff[2]=float_bit;
                float_bit = (int32)(num*1000)%10;
                 _nix_display_buff[1]=float_bit;
                float_bit = (int32)(num*10000)%10;
                 _nix_display_buff[0]=float_bit;
                i+=4;
            break;
            default:    //保留1位精度
                float_bit = (int32)(num*10)%10;
                _nix_display_buff[0]=float_bit;
                i+=1;
            break;
        }
        _nix_point_bit = (0x01 <<(i));//显示小数点
        for(;i<8;i++)
        {
            temp=int_part%10;
            int_part=int_part/10;
            _nix_display_buff[i]=temp;
            if(int_part == 0)
            {
                _nix_display_buff[i+1]=16;//添加负号
                break;
            }
        }
    }     
}

//========================================================================
// 描述: 数码管显示时间.
// 参数: 小时，分，0：左侧；1：右侧.
// 返回: none.
//========================================================================
void nix_display_time(uint8 hour,uint8 minute,uint8 dir) 
{
    uint32 temp;

    temp = hour*100+minute;
        
    if(dir)
    {
        nix_display_num(temp);
        _nix_colon_flag = 1;
    }
    else
    {
        temp = temp*10000;
        nix_display_num(temp);
        nix_display_clear_bit(0);
        nix_display_clear_bit(1);
        nix_display_clear_bit(2);
        nix_display_clear_bit(3);
        _nix_colon_flag = 2;
    }  
}

//========================================================================
// 描述: 数码管显示时间.
// 参数: 时，分，秒.
// 返回: none.
//========================================================================
void nix_display_time2(uint8 hour,uint8 minute,uint8 second)
{
	if(hour >= 10)  
    {
        _nix_display_buff[7] = hour / 10;
    }
    else
    {
        _nix_display_buff[7] = 0x11;
    }            
    _nix_display_buff[6] = hour % 10;
    _nix_display_buff[5] = 0x10;
    _nix_display_buff[4] = minute / 10;
    _nix_display_buff[3] = minute % 10;
    _nix_display_buff[2] = 0x10;
    _nix_display_buff[1] = second / 10;
    _nix_display_buff[0] = second % 10;
}

//========================================================================
// 描述: 数码管不显示冒号.
// 参数: none.
// 返回: none.
//========================================================================
void nix_disable_time()
{
    _nix_colon_flag = 0;
    P2_1=1;
    P0_7=1;
}

//========================================================================
// 描述: 数码管清屏.
// 参数: none.
// 返回: none.
//========================================================================
void nix_display_clear()
{
    uint8 i;
    for(i=0;i<8;i++)
    {
        _nix_display_buff[i]=17;
    }

    _nix_point_bit=0;
}

//========================================================================
// 描述: 数码管清指定位.
// 参数: (0~7).
// 返回: none.
//========================================================================
void nix_display_clear_bit(uint8 nbit)
{
    _nix_display_buff[nbit]=17;
}

//========================================================================
// 描述: 数码管显示缓存更新数据.
// 参数: 缓存地址.
// 返回: none.
//========================================================================
void nix_display_update_buf(uint8 *buf)
{
    int8 i;
    for(i=7;i>=0;i--)
    {
       _nix_display_buff[i]=*buf++;
    }
}

#endif