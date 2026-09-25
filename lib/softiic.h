/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/
#ifndef __SOFTIIC_H
#define __SOFTIIC_H

#include <STC8HX.h>
#include "delay.h"

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

void softiic_init();	//IIC初始化
void softiic_start();	//IIC发送start信号
void softiic_stop();	//IIC发送stop信号
uint8 softiic_wait_ack();//IIC等待ack信号
void softiic_ack();	//IIC发送ack信号
void softiic_nack();	//IIC发送NAck信号
void softiic_send_byte(uint8 IIC_Byte);	//IIC发送一个字节数据
uint8 softiic_read_byte();	//IIC读取一个字节数据
void softiic_read_nbyte(uint8 device_addr, uint8 reg_addr, uint8 *p, uint8 number);	//IIC读取n个字节数据
void softiic_write_nbyte(uint8 device_addr, uint8 reg_addr, uint8 *p, uint8 number);	//IIC写n个字节数据

//========================================================================
// 描述: 延时函数（仅本模块内可调用）.
// 参数: 延时的us数（不精确的）.
// 返回: none.
//========================================================================
static void delay_us(uint8 time)
{
    do { delay1us();} while (--time);
}

//========================================================================
// 描述: IIC初始化.
// 参数: none.
// 返回: none.
//========================================================================
void softiic_init()
{
    SOFTIIC_SCL_OUT;//开漏输出
    SOFTIIC_SDA_OUT;//开漏输出
    SOFTIIC_SDA = 1;
    SOFTIIC_SCL = 1;
}

//========================================================================
// 描述: IIC发送start信号.
// 参数: none.
// 返回: none.
//========================================================================
void softiic_start()
{
	SOFTIIC_SDA_OUT;      //sda线输出
	SOFTIIC_SDA=1;	  	  
	SOFTIIC_SCL=1;
	delay_us(4);
 	SOFTIIC_SDA=0;      //当CLK为高电平时，SDA发送一个下降沿代表一个起始信号
	delay_us(4);
	SOFTIIC_SCL=0;      //嵌住IIC总线，准备发送或者接收数据
}

//========================================================================
// 描述: IIC发送stop信号.
// 参数: none.
// 返回: none.
//========================================================================
void softiic_stop()
{
	SOFTIIC_SDA_OUT;      //sda线输出
	SOFTIIC_SCL=0;
	SOFTIIC_SDA=0;      //当CLK为高电平时，SDA发送一个上升沿代表一个起始信号
 	delay_us(4);
	SOFTIIC_SCL=1; 
	delay_us(1);
	SOFTIIC_SDA=1;      //发送结束信号
	delay_us(4);	
}

//========================================================================
// 描述: IIC等待ack信号.
// 参数: none.
// 返回: 0，接收到应答信号; 1，没有接收到应答信号.
//========================================================================
uint8 softiic_wait_ack()
{
	uint8 ucErrTime=0;
	SOFTIIC_SDA_IN;      //SDA设置为输入
	SOFTIIC_SDA=1;delay_us(1);	   
	SOFTIIC_SCL=1;delay_us(1);	 
	while(SOFTIIC_SDA == 1)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			softiic_stop();
			return 1;
		}
	}
	SOFTIIC_SCL=0;//时钟输出0 	   
	return 0; 
}

//========================================================================
// 描述: IIC发送ack信号.
// 参数: none.
// 返回: none.
//========================================================================
void softiic_ack()
{
	SOFTIIC_SCL=0;
	SOFTIIC_SDA_OUT;
	SOFTIIC_SDA=0;
	delay_us(2);
	SOFTIIC_SCL=1;
	delay_us(2);
	SOFTIIC_SCL=0;
}

//========================================================================
// 描述: IIC发送NAck信号.
// 参数: none.
// 返回: none.
//========================================================================
void softiic_nack()
{
	SOFTIIC_SCL=0;
	SOFTIIC_SDA_OUT;
	SOFTIIC_SDA=1;
	delay_us(2);
	SOFTIIC_SCL=1;
	delay_us(2);
	SOFTIIC_SCL=0;
}

//========================================================================
// 描述: IIC发送一个字节数据.
// 参数: 发送的字节数据.
// 返回: none.
//========================================================================
void softiic_send_byte(uint8 IIC_Byte)
{
    uint8 t;   
	SOFTIIC_SDA_OUT; 	    
    SOFTIIC_SCL=0;      //拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        SOFTIIC_SDA=(IIC_Byte&0x80)>>7;
        IIC_Byte<<=1; 	  
		delay_us(2);
		SOFTIIC_SCL=1;
		delay_us(2); 
		SOFTIIC_SCL=0;	
		delay_us(2);
    }	
}

//========================================================================
// 描述: IIC读取一个字节数据.
// 参数: none
// 返回: 读取到的字节数据.
//========================================================================
uint8 softiic_read_byte()
{
	uint8 i,receive=0;
	SOFTIIC_SDA_IN;   //SDA设置为输入
    for(i=0;i<8;i++ )
	{
        SOFTIIC_SCL=0; 
        delay_us(2);
		SOFTIIC_SCL=1;
        receive<<=1;
        if(SOFTIIC_SDA == 1)receive++;   
		delay_us(1); 
    }					 
    return receive;
}

//========================================================================
// 描述: IIC读取n个字节数据.
// 参数: 设备地址（8位模式，7位地址需要左移一位），寄存器地址，缓存数据地址，数量
// 返回: none.
//========================================================================
void softiic_read_nbyte(uint8 device_addr, uint8 reg_addr, uint8 *p, uint8 number)
{
    softiic_start();                        //开始信号
    softiic_send_byte(device_addr);      //发送器件写地址
    softiic_wait_ack();
    softiic_send_byte(reg_addr);                //发送寄存器地址
    softiic_wait_ack();

    softiic_start();                        //重复开始信号
    softiic_send_byte(device_addr | 0x01);      //发送器件读地址
    softiic_wait_ack();	
    do
    {
        *p = softiic_read_byte();
        p++;
        if(number != 1) softiic_ack();     

    }while(--number);
    softiic_nack(); 
    softiic_stop();
}

//========================================================================
// 描述: IIC写n个字节数据.
// 参数: 设备地址（8位模式，7位地址需要左移一位），寄存器地址，缓存数据地址，数量
// 返回: none.
//========================================================================
void softiic_write_nbyte(uint8 device_addr, uint8 reg_addr, uint8 *p, uint8 number)
{
    softiic_start();                        //开始信号
    softiic_send_byte(device_addr);      //发送器件写地址
    softiic_wait_ack();
    softiic_send_byte(reg_addr);                //发送寄存器地址
    softiic_wait_ack();
    do
    {
        softiic_send_byte(*p++);             //发送数据
        softiic_wait_ack();
    }while(--number);
    softiic_stop();                          //发送停止命令
}

#endif          //softiic