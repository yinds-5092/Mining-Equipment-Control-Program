/*************  技术支持与购买说明    **************
产品主页：http://twen51.com/
淘宝搜索：天问51，可购买基础版、带彩屏标准版、旗舰版
技术支持QQ群一：1138055784
******************************************/

#ifndef __AT24CXX_H
#define __AT24CXX_H

#if defined (_C51)
#include "C51_softiic.h"
#include "C51_delay.h"
#elif defined (_STC12)
#include "STC12_softiic.h"
#include "STC12_delay.h"
#elif defined (_STC15)
#include "STC15_softiic.h"
#include "STC15_delay.h"
#else
#include "softiic.h"
#include "delay.h"
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

#ifndef	EE_TYPE
#define EE_TYPE AT24C02
#endif

#ifndef AT24CXX_ADDR
#define AT24CXX_ADDR  0x50
#endif

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767

#define AT24CXX_ADDR_W  (AT24CXX_ADDR<<1)
#define AT24CXX_ADDR_R  (AT24CXX_ADDR_W | 0x01)

void at24cxx_init();    //初始化
void at24cxx_write_one_byte(uint16 addr, uint8 dat);    //在指定地址写入一个数据
uint8 at24cxx_read_one_byte(uint16 addr);   //在指定地址读出一个数据
void at24cxx_write(uint16 addr, uint8 *pbuf, uint16 len); //在指定地址写入指定长度的数据
void at24cxx_read(uint16 addr, uint8 *pbuf, uint16 len); //在指定地址读出指定长度的数据

//========================================================================
// 描述: 24cxx初始化.
// 参数: none.
// 返回: none.
//========================================================================
void at24cxx_init()
{
	softiic_init();
}

//========================================================================
// 描述: 24cxx在指定地址写入一个数据.
// 参数: addr:寄存器地址;dat:要写入的数据.
// 返回: none.
//========================================================================
void at24cxx_write_one_byte(uint16 addr, uint8 dat)
{
    softiic_start();                        //开始信号
#if(EE_TYPE > AT24C16)
    softiic_send_byte(AT24CXX_ADDR_W);      //发送写命令
    softiic_wait_ack();
    softiic_send_byte(addr>>8);      //发送高地址
#else
    softiic_send_byte((uint8)(AT24CXX_ADDR_W+((addr/256)<<1)));     //发送器件地址,写数据
#endif
    softiic_wait_ack();
    softiic_send_byte((uint8)addr);      //发送低地址
    softiic_wait_ack();
    softiic_send_byte(dat);      //发送数据
    softiic_wait_ack();
    softiic_stop();     //发送停止命令
}

//========================================================================
// 描述: 24cxx在指定地址读出一个数据.
// 参数: addr:寄存器地址;dat:要写入的数据.
// 返回: none.
//========================================================================
uint8 at24cxx_read_one_byte(uint16 addr)
{
    uint8 temp;
    softiic_start();                        //开始信号
#if(EE_TYPE > AT24C16)
    softiic_send_byte(AT24CXX_ADDR_W);      //发送写命令
    softiic_wait_ack();
    softiic_send_byte(addr>>8);      //发送高地址
#else
    softiic_send_byte((uint8)(AT24CXX_ADDR_W+((addr/256)<<1)));     //发送器件地址,写数据
#endif
    softiic_wait_ack();
    softiic_send_byte((uint8)addr);      //发送低地址
    softiic_wait_ack();
    softiic_start();         //重复开始信号
    softiic_send_byte(AT24CXX_ADDR_R);      //发送读信号
    softiic_wait_ack();
    temp = softiic_read_byte();
    softiic_nack(); 
    softiic_stop();                          //发送停止命令
    return temp;
}

//========================================================================
// 描述: 24cxx在指定地址写入最大4个字节的数据.
// 参数: addr:寄存器地址;dat:要写入的数据（16或者32位）;len:要写入的数据长度.
// 返回: none.
//========================================================================
void at24cxx_write_nbyte(uint16 addr,uint32 dat,uint8 len)
{  	
	uint8 t;
	for(t=0;t<len;t++)
	{
		at24cxx_write_one_byte(addr+t,(dat>>(8*t))&0xff);
	}												    
}

//========================================================================
// 描述: 24cxx在指定地址读出最多4个字节的数据.
// 参数: addr:寄存器地址;len:要写入的数据长度.
// 返回: 要读出的数据（16或者32位）.
//========================================================================
uint32 at24cxx_read_nbyte(uint16 addr, uint8 len)
{  	
	uint8 t;
    uint32 temp;
	for(t=0;t<len;t++)
	{
        temp <<= 8;
		temp += at24cxx_read_one_byte(addr+len-t-1);
	}	
    return temp;											    
}

//========================================================================
// 描述: 24cxx在指定地址写入指定个数的数据.
// 参数: addr:寄存器地址;*pbuf:数据数组指针;len:要写入的数据长度.
// 返回: none.
//========================================================================
void at24cxx_write(uint16 addr, uint8 *pbuf, uint16 len)
{
    uint8 i;
    for(i=0;i<len;i++)
    {
        at24cxx_write_one_byte(addr, *pbuf);
        delay(5);
        addr++;
        pbuf++;
    }
}

//========================================================================
// 描述: 24cxx在指定地址读出指定个数的数据.
// 参数: addr:寄存器地址;*pbuf:数据数组指针;len:要读出的数据长度.
// 返回: none.
//========================================================================
void at24cxx_read(uint16 addr, uint8 *pbuf, uint16 len)
{
    uint8 i;
    for(i=0;i<len;i++)
    {
        *pbuf = at24cxx_read_one_byte(addr);
        addr++;
        pbuf++;
    }
}


#endif  //at24cxx.h
