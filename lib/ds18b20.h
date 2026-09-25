/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef _DS18B20_H_
#define _DS18B20_H_

#include <STC8HX.h>
#include "delay.h"

#ifndef     DS18B20_DQ
#define     DS18B20_DQ              P4_7                           //DS18B20的数据口
#endif

#ifndef     DS18B20_DQ_MODE
#define     DS18B20_DQ_MODE       {P4M1&=~0x80;P4M0&=~0x80;}     //双向IO口
#endif

void ds18b20_init();                //DS18B20初始化
float ds18b20_read_temperature();   //读取温度

//========================================================================
// 描述: DS18B20初始化.
// 参数: none.
// 返回: none.
//========================================================================
void ds18b20_init()
{
    DS18B20_DQ_MODE;//双向IO口
}

//========================================================================
// 描述: DS18B20特定延时函数.
// 参数: none.
// 返回: none.
//========================================================================
void ds18b20_delay_us(uint8 us)
{
    do{
        _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
    }while(--us);
}

//========================================================================
// 描述: DS18B20总线复位.
// 参数: none.
// 返回: none.
//========================================================================
uint8 ds18b20_reset()
{
    uint8 ds18b20_retry;
    ds18b20_retry = 0;
    while (1)
    {
        ds18b20_retry++;
        if(ds18b20_retry >= 10)return 1;
        DS18B20_DQ = 1; 
        DS18B20_DQ = 0;                     //送出低电平复位信号
        ds18b20_delay_us(240);      //延时至少480us
        ds18b20_delay_us(240);
        DS18B20_DQ = 1;                     //释放数据线
        ds18b20_delay_us(60);       //等待60us
        if(!DS18B20_DQ)                    //检测存在脉冲
        {
            ds18b20_delay_us(240);      //等待设备释放数据线
            ds18b20_delay_us(180);
            return 0;
        }
        ds18b20_delay_us(240);      //等待设备释放数据线
        ds18b20_delay_us(180);
    }
}

//========================================================================
// 描述: 从DS18B20读1字节数据.
// 参数: none.
// 返回: 1字节数据.
//========================================================================
uint8 ds18b20_read_byte()
{
    uint8 i;
    uint8 dat = 0;

    for (i=0; i<8; i++)             //8位计数器
    {
        dat >>= 1;
        DS18B20_DQ = 0;                     //开始时间片
        ds18b20_delay_us(1);                //延时等待
        DS18B20_DQ = 1;                     //准备接收
        ds18b20_delay_us(1);                //接收延时
        if (DS18B20_DQ) dat |= 0x80;        //读取数据
        ds18b20_delay_us(60);               //等待时间片结束
    }

    return dat;
}

//========================================================================
// 描述: 向DS18B20写1字节数据.
// 参数: 1字节数据.
// 返回: none.
//========================================================================
void ds18b20_write_byte(uint8 dat)
{
    char i;

    for (i=0; i<8; i++)             //8位计数器
    {
        DS18B20_DQ = 0;                     //开始时间片
        ds18b20_delay_us(1);                //延时等待
        dat >>= 1;                  //送出数据
        DS18B20_DQ = CY;
        ds18b20_delay_us(60);               //等待时间片结束
        DS18B20_DQ = 1;                     //恢复数据线
        ds18b20_delay_us(1);                //恢复延时
    }
}

//========================================================================
// 描述: 读取温度.
// 参数: none.
// 返回: 温度值.
//========================================================================
float ds18b20_read_temperature()
{
    uint16 TempH, TempL, Temperature;
	uint16 ds18b20_retry;
    ds18b20_retry = 0;

    if(ds18b20_reset())      //设备复位
    {
        return 255; //返回错误代码
    }   
    ds18b20_write_byte(0xCC);        //跳过ROM命令
    ds18b20_write_byte(0x44);        //开始转换命令
    while (!DS18B20_DQ && ds18b20_retry++ < 400)                    //等待转换完成
    {
		delay(1);
    }
    if(ds18b20_retry >= 400)
    {
        return 255;  //返回错误代码
    }
    if(ds18b20_reset())      //设备复位
    {
        return 255; //返回错误代码
    }  
    ds18b20_write_byte(0xCC);        //跳过ROM命令
    ds18b20_write_byte(0xBE);        //读暂存存储器命令
    TempL = ds18b20_read_byte();     //读温度低字节
    TempH = ds18b20_read_byte();     //读温度高字节
	
    if(TempH & 0xf8)    //如果为负数
    {
       // MinusFlag = 1;  //设置负数标志
        Temperature = (TempH<<8) | TempL;
        Temperature = ~Temperature+1;
        return (float)Temperature*-0.625/10.0;
    }
    else
    {
        //MinusFlag = 0;  //清除负数标志
        Temperature = (TempH<<8) | TempL; 
        return (float)Temperature*0.625/10.0;	//0.0625 * 10，保留1位小数点
    }
}
 #endif