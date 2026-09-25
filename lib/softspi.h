/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/
#ifndef __SOFTSPI_H
#define __SOFTSPI_H
#include <STC8HX.h>
#include "delay.h"

#ifndef   SOFTSPI_SCK_PIN
#define   SOFTSPI_SCK_PIN       P2_5
#endif
#ifndef   SOFTSPI_SCK_MODE
#define   SOFTSPI_SCK_MODE      {P2M1&=~0x20;P2M0&=~0x20;}
#endif

#ifndef   SOFTSPI_MISO_PIN
#define   SOFTSPI_MISO_PIN       P2_4
#endif
#ifndef   SOFTSPI_MISO_MODE
#define   SOFTSPI_MISO_MODE      {P2M1&=~0x10;P2M0&=~0x10;}
#endif

#ifndef   SOFTSPI_MOSI_PIN
#define   SOFTSPI_MOSI_PIN       P2_3
#endif
#ifndef   SOFTSPI_MOSI_MODE
#define   SOFTSPI_MOSI_MODE      {P2M1&=~0x08;P2M0&=~0x08;}
#endif

#define SOFTSPI_CPHA 0
#define SOFTSPI_CPOL 0

void softspi_init();//初始化
void softspi_write_byte(uint8 out);
uint8 softspi_read_byte(void);

//========================================================================
// 描述: SPI初始化.
// 参数: none.
// 返回: none.
//========================================================================
void softspi_init()
{
    SOFTSPI_SCK_MODE;
    SOFTSPI_MISO_MODE;
    SOFTSPI_MOSI_MODE;
    SOFTSPI_SCK_PIN = 0;        //set clock to low initial state
}

//========================================================================
// 描述: SPI写入一个字节后读取一个字节数据.
// 参数: none.
// 返回: none.
//========================================================================
uint8 softspi_wr_data(uint8 data)
{
    uint8 datavalue=0,i,wr_data;
    wr_data = data;
    #if SOFTSPI_CPHA==0 && SOFTSPI_CPOL==0
    for(i=0;i<8;i++){
        if(wr_data&0x80){
            SOFTSPI_MOSI_PIN = 1;
        }else{
            SOFTSPI_MOSI_PIN = 0;
        }
        wr_data <<=1;
        SOFTSPI_SCK_PIN = 1;
        datavalue <<=1;
        if(SOFTSPI_MISO_PIN == 1){
            datavalue |= 0x01;
        }
         SOFTSPI_SCK_PIN = 0;
        //delay(1);
    }
    
    #elif SOFTSPI_CPHA==0 && SOFTSPI_CPOL==1
    for(i=0;i<8;i++){
        if(wr_data&0x80){
            SOFTSPI_MOSI_PIN = 1;
        }else{
            SOFTSPI_MOSI_PIN = 0;
        }
        wr_data <<=1;
        SOFTSPI_SCK_PIN = 0;
        datavalue <<=1;
        if(SOFTSPI_MISO_PIN == 1){
            datavalue |= 0x01;
        }
        SOFTSPI_SCK_PIN = 1;
        //delay(1);
    }  
    
    #elif SOFTSPI_CPHA==1 && SOFTSPI_CPOL==0
    for(i=0;i<8;i++){
        SOFTSPI_SCK_PIN = 1;
        if(wr_data&0x80){
            SOFTSPI_MOSI_PIN = 1;
        }else{
            SOFTSPI_MOSI_PIN = 0;
        }
        wr_data <<=1;
        SOFTSPI_SCK_PIN = 0;
        datavalue <<=1;
        if(SOFTSPI_MISO_PIN == 1){
            datavalue |= 0x01;
        }
        //delay(1);
    } 
    
    #elif SOFTSPI_CPHA==1 && SOFTSPI_CPOL==1
    for(i=0;i<8;i++){
        SOFTSPI_SCK_PIN = 0;
        if(wr_data&0x80){
            SOFTSPI_MOSI_PIN = 1;
        }else{
             SOFTSPI_MOSI_PIN = 0;
        }
        wr_data <<=1;
        SOFTSPI_SCK_PIN = 1;
        datavalue <<=1;
        if(SOFTSPI_MISO_PIN == 1){
            datavalue |= 0x01;
        }
        SOFTSPI_SCK_PIN = 1;
        // delay(1);
    } 
    #endif
    return datavalue;
}

//========================================================================
// 描述: SPI写一个字节.
// 参数: none.
// 返回: none.
//========================================================================
void softspi_write_byte(uint8 out)
{
    softspi_wr_data(out);
}

//========================================================================
// 描述: SPI读一个字节.
// 参数: none.
// 返回: none.
//========================================================================
uint8 softspi_read_byte(void)
{
    return softspi_wr_data(0xff);;
}

#endif 