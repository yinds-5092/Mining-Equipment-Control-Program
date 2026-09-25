/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef __HARDSPI_H
#define __HARDSPI_H
#include <STC8HX.h>
#include "delay.h"

#define   HARDSPI_SPIF    0x80        //SPI传输完成标志。写入1清0。
#define   HARDSPI_WCOL    0x40        //SPI写冲突标志。写入1清0。

#ifndef   HARDSPI_SPI_SCK_PIN
#define   HARDSPI_SPI_SCK_PIN       P2_5
#endif

#ifndef   HARDSPI_SCK_PIN_MODE
#define   HARDSPI_SCK_PIN_MODE    {P2M1&=~0x20;P2M0&=~0x20;}    //P25双向IO口
#endif

#ifndef   HARDSPI_SPI_MISO_PIN
#define   HARDSPI_SPI_MISO_PIN      P2_4
#endif

#ifndef   HARDSPI_MISO_PIN_MODE
#define   HARDSPI_MISO_PIN_MODE   {P2M1&=~0x10;P2M0&=~0x10;}    //P24双向IO口
#endif

#ifndef   HARDSPI_SPI_MOSI_PIN
#define   HARDSPI_SPI_MOSI_PIN      P2_3
#endif 

#ifndef   HARDSPI_MOSI_PIN_MODE
#define   HARDSPI_MOSI_PIN_MODE   {P2M1&=~0x08;P2M0&=~0x08;}    //P23双向IO口
#endif

#define   HARDSPI_CPHA              0      //数据 SS 管脚为低电平驱动第一位数据并在 SCLK 的后时钟沿改变数据
#define   HARDSPI_CPOL              0      //SCLK 空闲时为低电平，SCLK 的前时钟沿为上升沿，后时钟沿为下降沿

void hardspi_init(uint8 speed);         //spi初始化
void hardspi_write_byte(uint8 out);     //spi写一个字节
uint8 hardspi_read_byte();              //spi读取一个字节
uint8 hardspi_wr_data(uint8 data);      //spi写入一个字节并且返回一个读取的字节

//========================================================================
// 描述: SPI初始化.
// 参数: speed:通信速率（0~3）.
// 返回: none.
//========================================================================
void hardspi_init(uint8 speed)
{
   
    HARDSPI_SCK_PIN_MODE;            //引脚初始化
    HARDSPI_MISO_PIN_MODE;
    HARDSPI_MOSI_PIN_MODE;

    SPCTL |=  (1 << 7);         //忽略 SS 引脚功能，使用 MSTR 确定器件是主机还是从机
    SPCTL |=  (1 << 6);         //使能 SPI 功能
    SPCTL &= ~(1 << 5);         //先发送/接收数据的高位（ MSB）
    SPCTL |=  (1 << 4);         //设置主机模式
    SPCTL &= ~(1 << 3);         //设置CPOL  
    SPCTL |=  (HARDSPI_CPOL << 3);

    SPCTL &= ~(1 << 2);         //设置CPHA
    SPCTL |=  (HARDSPI_CPHA << 3);

    SPCTL = (SPCTL & ~3) | speed;           //SPI 时钟频率选择, 0: 4T, 1: 8T,  2: 16T,  3: 32T
    P_SW1 = (P_SW1 & ~(3<<2)) | (1<<2); //IO口切换. 0: P1.2/P5.4 P1.3 P1.4 P1.5, 1: P2.2 P2.3 P2.4 P2.5, 2: P5.4 P4.0 P4.1 P4.3, 3: P3.5 P3.4 P3.3 P3.2

    HARDSPI_SPI_SCK_PIN = 0;        //set clock to low initial state
    HARDSPI_SPI_MOSI_PIN = 1;
    SPSTAT = HARDSPI_SPIF + HARDSPI_WCOL;   //清0 SPIF和WCOL标志
}

//========================================================================
// 描述: SPI写一个字节.
// 参数: none.
// 返回: none.
//========================================================================
void hardspi_write_byte(uint8 out)
{
    SPDAT = out;
    while((SPSTAT & HARDSPI_SPIF) == 0) ;
    SPSTAT = HARDSPI_SPIF + HARDSPI_WCOL;   //清0 SPIF和WCOL标志
}

//========================================================================
// 描述: SPI读一个字节.
// 参数: none.
// 返回: none.
//========================================================================
uint8 hardspi_read_byte()
{
    SPDAT = 0xff;
    while((SPSTAT & HARDSPI_SPIF) == 0) ;
    SPSTAT = HARDSPI_SPIF + HARDSPI_WCOL;   //清0 SPIF和WCOL标志
    return (SPDAT);
}

//========================================================================
// 描述: SPI写入一个字节后读取一个字节数据.
// 参数: none.
// 返回: none.
//========================================================================
uint8 hardspi_wr_data(uint8 data)
{
    SPDAT = data;
    while((SPSTAT & HARDSPI_SPIF) == 0) ;
    SPSTAT = HARDSPI_SPIF + HARDSPI_WCOL;   //清0 SPIF和WCOL标志
    return (SPDAT);
}

#endif      //spi.h