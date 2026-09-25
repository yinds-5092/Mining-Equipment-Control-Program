/*************  技术支持与购买说明    **************
产品主页：http://twen51.com/
淘宝搜索：天问51，可购买基础版、带彩屏标准版、旗舰版
技术支持QQ群一：1138055784
******************************************/
#ifndef _RC522_H
#define _RC522_H

#include <string.h>
#if defined (_C51)
#include "C51_softspi.h"
#include "C51_delay.h"
#elif defined (_STC12)
#include "STC12_softspi.h"
#include "STC12_delay.h"
#elif defined (_STC15)
#include "STC15_softspi.h"
#include "STC15_delay.h"
#else
#include "softspi.h"
#include "delay.h"
#endif

#ifndef		RC522_RST
#define	    RC522_RST 		    		P1_0  		    //RC522的复位信号,可不接
#endif

#ifndef 	RC522_RST_MODE
#define		RC522_RST_MODE		        {P1M1&=~0x01;P1M0&=~0x01;}
#endif

#ifndef		RC522_NSS
#define	    RC522_NSS 		    		P1_1
#endif

#ifndef 	RC522_NSS_MODE
#define		RC522_NSS_MODE		        {P1M1&=~0x02;P1M0&=~0x02;}
#endif

//MF522命令字
#define PCD_IDLE              0x00               //取消当前命令
#define PCD_AUTHENT           0x0E               //验证密钥
#define PCD_RECEIVE           0x08               //接收数据
#define PCD_TRANSMIT          0x04               //发送数据
#define PCD_TRANSCEIVE        0x0C               //发送并接收数据
#define PCD_RESETPHASE        0x0F               //复位
#define PCD_CALCCRC           0x03               //CRC计算

//Mifare_One卡片命令字
#define PICC_REQIDL           0x26               //寻天线区内未进入休眠状态
#define PICC_REQALL           0x52               //寻天线区内全部卡
#define PICC_ANTICOLL1        0x93               //防冲撞
#define PICC_ANTICOLL2        0x95               //防冲撞
#define PICC_AUTHENT1A        0x60               //验证A密钥
#define PICC_AUTHENT1B        0x61               //验证B密钥
#define PICC_READ             0x30               //读块
#define PICC_WRITE            0xA0               //写块
#define PICC_DECREMENT        0xC0               //扣款
#define PICC_INCREMENT        0xC1               //充值
#define PICC_RESTORE          0xC2               //调块数据到缓冲区
#define PICC_TRANSFER         0xB0               //保存缓冲区中数据
#define PICC_HALT             0x50               //休眠

//MF522 FIFO长度定义
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte

//MF522寄存器定义
// PAGE 0
#define     RFU00                 0x00    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2    
#define     RFU20                 0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     RFU3C                 0x3C   
#define     RFU3D                 0x3D   
#define     RFU3E                 0x3E   
#define     RFU3F		          0x3F

//和MF522通讯时返回的错误代码
#define     MI_OK                 0
#define     MI_NOTAGERR           (-1)
#define     MI_ERR                (-2)

#define     MAXRLEN               18


void rc522_init();  //初始化
uint8 rc522_write(uint8 *key,uint8 addr,uint8 *value);  //写数据
uint8 rc522_read(uint8 *key,uint8 addr,uint8 *value);   //读数据


uint8 rc522_read_raw(uint8 Address);
void rc522_write_raw(uint8 Address, uint8 value);
void rc522_set_bit_mask(uint8 reg,uint8 mask);
void rc522_clear_bit_mask(uint8 reg,uint8 mask);
int8 rc522_pcd_reset();
void rc522_pcd_antenna_on();
void rc522_pcd_antenna_off();
int8 rc522_pcd_com_mf522(uint8   Command, uint8   *pInData, uint8   InLenByte,uint8   *pOutData, uint16  *pOutLenBit);

int8 rc522_pcd_request(uint8 req_code,uint8 *pTagType);     //寻卡
int8 rc522_pcd_anticoll(uint8 *pSnr);   //防冲撞
int8 rc522_pcd_select(uint8 *pSnr);     //选卡
int8 rc522_pcd_auth_state(uint8 auth_mode,uint8 addr,uint8 *pKey,uint8 *pSnr);  //验证卡密码
int8 rc522_pcd_read(uint8 addr,uint8 *pData);   //读取一块数据
int8 rc522_read_block(uint8 Block,uint8 *Buf);
int8 rc522_pcd_write(uint8 addr,uint8 *pData);  //写入一块数据
int8 rc522_write_block(uint8 Block);
int8 rc522_pcd_value(uint8 dd_mode,uint8 addr,uint8 *pValue);   //扣款和充值
int8 rc522_pcd_bak_value(uint8 sourceaddr, uint8 goaladdr); //备份钱包
int8 rc522_pcd_halt();  //进入休眠
void rc522_calulate_crc(uint8 *pIndata,uint8 len,uint8 *pOutData);  //crc校验
int8 rc522_m500_pcd_config_ios_type(uint8 type);    //设置工作方式

// extern char KK[8]; // 数据加密密钥
// extern unsigned char RF_Buffer[18]; // 射频卡数据缓冲区
// extern unsigned char UID[5];
// extern unsigned char Password_Buffer[6];
// extern unsigned char des_on; // DES加密标志

//========================================================================
// 描述: 读RC522寄存器.（spi:CPOL=0,CPHA=0）
// 参数: Address[IN]:寄存器地址
// 返回: 读出的值
//========================================================================
// uint8 rc522_read_raw(uint8 Address)     //硬件spi
// {
//      uint8 ucAddr;
//      uint8 ucResult=0;

//      RC522_NSS = 0;
//      ucAddr = ((Address<<1)&0x7E)|0x80;

//     hardspi_wr_data(ucAddr);
//     ucResult = hardspi_wr_data(0xff);
//     RC522_NSS = 1;

//     return ucResult;
// }

uint8 rc522_read_raw(uint8 Address)     //模拟spi
{
     uint8 ucAddr;
     uint8 ucResult=0;

     RC522_NSS = 0;
     ucAddr = ((Address<<1)&0x7E)|0x80;
     softspi_wr_data(ucAddr);
     ucResult = softspi_wr_data(0xff);
     RC522_NSS = 1;

     return ucResult;
}

//========================================================================
// 描述: 写RC522寄存器.（spi:CPOL=0,CPHA=0）
// 参数: Address:寄存器地址
//       value:写入的值
// 返回: none
//========================================================================
// void rc522_write_raw(uint8 Address, uint8 value)    //硬件spi
// {  
//     uint8 ucAddr;

//     RC522_NSS = 0;
//     ucAddr = ((Address<<1)&0x7E);

//     hardspi_wr_data(ucAddr);
//     hardspi_wr_data(value);

//     RC522_NSS = 1;
// }

void rc522_write_raw(uint8 Address, uint8 value)    //模拟spi
{  
    uint8 ucAddr;

    ucAddr = ((Address<<1)&0x7E);
    RC522_NSS = 0;
    softspi_wr_data(ucAddr);
    softspi_wr_data(value);
    RC522_NSS = 1;
}

//========================================================================
// 描述: 置RC522寄存器位.
// 参数: reg[IN]:寄存器地址
//       mask[IN]:置位值
// 返回: none
//========================================================================
void rc522_set_bit_mask(uint8 reg,uint8 mask)  
{
    int8 tmp = 0x0;
    tmp = rc522_read_raw(reg);
    rc522_write_raw(reg,tmp | mask);  // set bit mask
}

//========================================================================
// 描述: 清RC522寄存器位.
// 参数: reg[IN]:寄存器地址
//       mask[IN]:清位值
// 返回: none
//========================================================================
void rc522_clear_bit_mask(uint8 reg,uint8 mask)  
{
    int8 tmp = 0x0;
    tmp = rc522_read_raw(reg);
    rc522_write_raw(reg, tmp & ~mask);  // clear bit mask
} 

//========================================================================
// 描述: 复位RC552.
// 参数: none
// 返回: none
//========================================================================
int8 rc522_pcd_reset()
{
    RC522_RST=1;
    delay(1);
    RC522_RST=0;
    delay(1);
    RC522_RST=1;
    delay(1);
    rc522_write_raw(CommandReg,PCD_RESETPHASE);
    delay(1);
    
    rc522_write_raw(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
    rc522_write_raw(TReloadRegL,30);           
    rc522_write_raw(TReloadRegH,0);
    rc522_write_raw(TModeReg,0x8D);
    rc522_write_raw(TPrescalerReg,0x3E);
    rc522_write_raw(TxAutoReg,0x40);     
    return MI_OK;
}

//========================================================================
// 描述: 开启天线（每次启动或关闭天险发射之间应至少有1ms的间隔）.
// 参数: none
// 返回: none
//========================================================================
void rc522_pcd_antenna_on()
{
    uint8 i;
    i = rc522_read_raw(TxControlReg);
    if (!(i & 0x03))
    {
        rc522_set_bit_mask(TxControlReg, 0x03);
    }
}

//========================================================================
// 描述: 关闭天线（每次启动或关闭天险发射之间应至少有1ms的间隔）.
// 参数: none
// 返回: none
//========================================================================
void rc522_pcd_antenna_off()
{
    rc522_clear_bit_mask(TxControlReg, 0x03);
}

//========================================================================
// 描述: 通过RC522和ISO14443卡通讯.
// 参数: Command:RC522命令字
//       pInData:通过RC522发送到卡片的数据
//       InLenByte:发送数据的字节长度
//       pOutData:接收到的卡片返回数据
//       *pOutLenBit:返回数据的位长度
// 返回: 成功返回MI_OK
//========================================================================
int8 rc522_pcd_com_mf522(uint8   Command, uint8   *pInData, uint8   InLenByte,uint8   *pOutData, uint16  *pOutLenBit)
{
    int8 status = MI_ERR;
    uint8 irqEn   = 0x00;
    uint8 waitFor = 0x00;
    uint8 lastBits;
    uint8 n;
    uint16 i;
    switch (Command)
    {
        case PCD_AUTHENT:
            irqEn   = 0x12;
            waitFor = 0x10;
        break;
        case PCD_TRANSCEIVE:
            irqEn   = 0x77;
            waitFor = 0x30;
        break;
        default:
         break;
    }
   
    rc522_write_raw(ComIEnReg,irqEn|0x80);
    rc522_clear_bit_mask(ComIrqReg,0x80);
    rc522_write_raw(CommandReg,PCD_IDLE);
    rc522_set_bit_mask(FIFOLevelReg,0x80);
    
    for (i=0; i<InLenByte; i++)
    {   
        rc522_write_raw(FIFODataReg, pInData[i]);    
    }
    rc522_write_raw(CommandReg, Command);
   
    if (Command == PCD_TRANSCEIVE)
    {    
        rc522_set_bit_mask(BitFramingReg,0x80);  
    }
    
    // i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
    i = 2000;
    do 
    {
        n = rc522_read_raw(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    rc522_clear_bit_mask(BitFramingReg,0x80);    
    if (i!=0)
    {    
        if(!(rc522_read_raw(ErrorReg)&0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   
                status = MI_NOTAGERR;   
            }
            if (Command == PCD_TRANSCEIVE)
            {
               	n = rc522_read_raw(FIFOLevelReg);
              	lastBits = rc522_read_raw(ControlReg) & 0x07;
                if (lastBits)
                {   
                    *pOutLenBit = (n-1)*8 + lastBits;   
                }
                else
                {   
                    *pOutLenBit = n*8;   
                }
                if (n == 0)
                {  
                    n = 1;    
                }
                if (n > MAXRLEN)
                {   
                    n = MAXRLEN;   
                }
                for (i=0; i<n; i++)
                {   
                    pOutData[i] = rc522_read_raw(FIFODataReg);    
                }
            }
        }
        else
        {   
            status = MI_ERR;   
        }
   }
   rc522_set_bit_mask(ControlReg,0x80);           // stop timer now
   rc522_write_raw(CommandReg,PCD_IDLE); 
   return status;
}

//========================================================================
// 描述: rc522初始化.
// 参数: none
// 返回: none
//========================================================================
void rc522_init()
{
  #ifndef _C51
  RC522_RST_MODE;
  RC522_NSS_MODE;
  #endif
  softspi_init();
//   hardspi_init(3);
  rc522_pcd_reset();
  rc522_pcd_antenna_off(); 
  rc522_pcd_antenna_on();  
  rc522_m500_pcd_config_ios_type('A');
}

//========================================================================
// 描述: 寻卡.
// 参数: req_code[IN]:寻卡方式
//                    0x52 = 寻感应区内所有符合14443A标准的卡
//                    0x26 = 寻未进入休眠状态的卡
//       pTagType[OUT]:卡片类型代码
//                      0x4400 = Mifare_UltraLight
//                      0x0400 = Mifare_One(S50)
//                      0x0200 = Mifare_One(S70)
//                      0x0800 = Mifare_Pro(X)
//                      0x4403 = Mifare_DESFire
// 返回: 成功返回MI_OK
//========================================================================
int8 rc522_pcd_request(uint8 req_code,uint8 *pTagType)
{
    int8 status;  
    uint16  unLen;
    uint8 ucComMF522Buf[MAXRLEN]; 
    // uint8 xTest;
    rc522_clear_bit_mask(Status2Reg,0x08);
    rc522_write_raw(BitFramingReg,0x07);

    // xTest = rc522_read_raw(BitFramingReg);
    // if(xTest == 0x07 )LED_GREEN = 0;
    // else{LED_GREEN = 1;while(1);}

    rc522_set_bit_mask(TxControlReg,0x03);
 
    ucComMF522Buf[0] = req_code;

    status = rc522_pcd_com_mf522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);

    // if(status == MI_OK)LED_GREEN  = 0;
    // else LED_GREEN =1;

    if ((status == MI_OK) && (unLen == 0x10))
    {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
    }
    else
    {   
        status = MI_ERR;   
    }
    return status;
}

//========================================================================
// 描述: 防冲撞.
// 参数: pSnr:卡片序列号，4字节
// 返回: 成功返回MI_OK
//========================================================================
int8 rc522_pcd_anticoll(uint8 *pSnr)
{
    int8 status;
    uint8 i,snr_check=0;
    uint16 unLen;
    uint8 ucComMF522Buf[MAXRLEN]; 
    

    rc522_clear_bit_mask(Status2Reg,0x08);
    rc522_write_raw(BitFramingReg,0x00);
    rc522_clear_bit_mask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = rc522_pcd_com_mf522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	for (i=0; i<4; i++)
        {   
            *(pSnr+i)  = ucComMF522Buf[i];
            snr_check ^= ucComMF522Buf[i];
        }
        if (snr_check != ucComMF522Buf[i])
        {   
            status = MI_ERR;    
        }
    }
    
    rc522_set_bit_mask(CollReg,0x80);
    return status;
}

//========================================================================
// 描述: 选定卡片.
// 参数: pSnr:卡片序列号，4字节
// 返回: 成功返回MI_OK
//========================================================================
int8 rc522_pcd_select(uint8 *pSnr)
{
    int8 status;
    uint8 i;
    uint16  unLen;
    uint8 ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    rc522_calulate_crc(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    rc522_clear_bit_mask(Status2Reg,0x08);

    status = rc522_pcd_com_mf522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   
        status = MI_OK;  
    }
    else
    {   
        status = MI_ERR;    
    }
    return status;
}


//========================================================================
// 描述: 验证卡片密码.
// 参数: auth_mode[IN]: 密码验证模式
//                      0x60 = 验证A密钥
//                      0x61 = 验证B密钥 
//       addr[IN]：块地址(0~63)
//       pKey[IN]：密码
//       pSnr[IN]：卡片序列号，4字节
// 返回:成功返回MI_OK
//========================================================================
int8 rc522_pcd_auth_state(uint8 auth_mode,uint8 addr,uint8 *pKey,uint8 *pSnr)
{
    int8 status;
    uint16  unLen;
    uint8 i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {   
        ucComMF522Buf[i+2] = *(pKey+i);   
    }
    for (i=0; i<6; i++)
    {    
        ucComMF522Buf[i+8] = *(pSnr+i);   
    }
    // memcpy(&ucComMF522Buf[2], pKey, 6); 
    // memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = rc522_pcd_com_mf522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(rc522_read_raw(Status2Reg) & 0x08)))
    {   
        status = MI_ERR;   
    }
    return status;
}

//========================================================================
// 描述: 读取M1卡一块数据.
// 参数: addr[IN]:块地址(0~63)
//       pData[OUT]:读出的数据，16字节
// 返回: 成功返回MI_OK
//========================================================================
int8 rc522_pcd_read(uint8 addr,uint8 *pData)
{
    int8 status;
    uint16  unLen;
    uint8 i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    rc522_calulate_crc(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = rc522_pcd_com_mf522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
    // {   
    //     memcpy(pData, ucComMF522Buf, 16);   
    // }
    {
        for (i=0; i<16; i++)
        {    
            *(pData+i) = ucComMF522Buf[i];   
        }
    }
    else
    {   
        status = MI_ERR;   
    }
    return status;
}

//========================================================================
// 描述: 读取M1卡一块数据.
// 参数: Block:块地址(0~63);buf:数据，16字节
// 返回: 成功返回MI_OK
//========================================================================
// int8 rc522_read_block(uint8 Block,uint8 *Buf)
// {
//     int8 result;
//     result = rc522_pcd_auth_state(0x60,Block,Password_Buffer,UID);
//     if(result!=MI_OK)return result;

//     result = rc522_pcd_read(Block,Buf);
//     // return result; // 2011.01.03
  
//     if(result!=MI_OK)return result;
//     if(Block!=0x00&&des_on)
//     {
//         Des_Decrypt((char *)Buf,KK,(char *)Buf);
//         Des_Decrypt((char *)&Buf[8],KK,(char *)&Buf[8]);  
//     }
//     return MI_OK; 
// }

//========================================================================
// 描述: 写数据到M1卡一块.
// 参数: addr[IN]:块地址(0~63)
//       pData[OUT]:写入的数据，16字节
// 返回: 成功返回MI_OK
//========================================================================
int8 rc522_pcd_write(uint8 addr,uint8 *pData)
{
    int8 status;
    uint16  unLen;
    uint8 i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    rc522_calulate_crc(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = rc522_pcd_com_mf522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   
        status = MI_ERR;   
    }
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
        for (i=0; i<16; i++)
        {    
            ucComMF522Buf[i] = *(pData+i);   
        }
        rc522_calulate_crc(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = rc522_pcd_com_mf522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   
            status = MI_ERR;   
        }
    }
    return status;
}

//========================================================================
// 描述: 写数据到M1卡一块.
// 参数: Block：块(0~63);
// 返回: 成功返回MI_OK
//========================================================================
// int8 rc522_write_block(uint8 Block)
// {
//   int8 result;

//   result = rc522_pcd_auth_state(0x60,Block,Password_Buffer,UID);
//   if(result!=MI_OK)return result;  

//   result = rc522_pcd_write(Block,RF_Buffer);
//   return result;  
// }

//========================================================================
//功    能：扣款和充值
//参数说明: dd_mode:命令字
//                      0xC0 = 扣款
//                      0xC1 = 充值
//          addr:钱包地址
//          pValue:4字节增(减)值，低位在前
//返    回: 成功返回MI_OK
//========================================================================             
int8 rc522_pcd_value(uint8 dd_mode,uint8 addr,uint8 *pValue)
{
    int8 status;
    uint16  unLen;
    uint8 ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    rc522_calulate_crc(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = rc522_pcd_com_mf522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   
        status = MI_ERR;   
    }
        
    if (status == MI_OK)
    {
        memcpy(ucComMF522Buf, pValue, 4);
        // for (i=0; i<16; i++)
        // {    
        //     ucComMF522Buf[i] = *(pValue+i);   
        // }
        rc522_calulate_crc(ucComMF522Buf,4,&ucComMF522Buf[4]);
        unLen = 0;
        status = rc522_pcd_com_mf522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    
            status = MI_OK;    
        }
    }
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        rc522_calulate_crc(ucComMF522Buf,2,&ucComMF522Buf[2]); 
   
        status = rc522_pcd_com_mf522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   
            status = MI_ERR;   
        }
    }
    return status;
}

//========================================================================  
//功    能：备份钱包
//参数说明: sourceaddr:源地址
//          goaladdr:目标地址
//返    回: 成功返回MI_OK
//========================================================================  
int8 rc522_pcd_bak_value(uint8 sourceaddr, uint8 goaladdr)
{
    int8 status;
    uint16  unLen;
    uint8 ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_RESTORE;
    ucComMF522Buf[1] = sourceaddr;
    rc522_calulate_crc(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = rc522_pcd_com_mf522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   
        status = MI_ERR;   
    }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = 0;
        ucComMF522Buf[1] = 0;
        ucComMF522Buf[2] = 0;
        ucComMF522Buf[3] = 0;
        rc522_calulate_crc(ucComMF522Buf,4,&ucComMF522Buf[4]);
 
        status = rc522_pcd_com_mf522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {   
            status = MI_OK;    
        }
    }
    
    if (status != MI_OK)
    {    
        return MI_ERR;   
    }
    
    ucComMF522Buf[0] = PICC_TRANSFER;
    ucComMF522Buf[1] = goaladdr;

    rc522_calulate_crc(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = rc522_pcd_com_mf522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   
        status = MI_ERR;   
    }

    return status;
}


//========================================================================
// 描述: 命令卡片进入休眠状态.
// 参数: none
// 返回: 成功返回MI_OK
//========================================================================
int8 rc522_pcd_halt()
{
    int8 status;
    uint16  unLen;
    uint8 ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    rc522_calulate_crc(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = rc522_pcd_com_mf522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    return status;
    // return MI_OK;
}

//========================================================================
// 描述: 用CR522计算CRC16函数.
// 参数: none
// 返回: none
//========================================================================
void rc522_calulate_crc(uint8 *pIndata,uint8 len,uint8 *pOutData)
{
    uint8 i,n;
    rc522_clear_bit_mask(DivIrqReg,0x04);
    rc522_write_raw(CommandReg,PCD_IDLE);
    rc522_set_bit_mask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   
        rc522_write_raw(FIFODataReg, *(pIndata+i));   
    }
    rc522_write_raw(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = rc522_read_raw(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = rc522_read_raw(CRCResultRegL);
    pOutData[1] = rc522_read_raw(CRCResultRegM);
}

//========================================================================
// 描述: 设置RC552的工作方式.
// 参数: type:工作方式
// 返回: 成功返回MI_OK
//========================================================================
int8 rc522_m500_pcd_config_ios_type(uint8 type)
{
    if (type == 'A')                     //ISO14443_A
    { 
        rc522_clear_bit_mask(Status2Reg,0x08);

        // rc522_write_raw(CommandReg,0x20);    //as default   
        // rc522_write_raw(ComIEnReg,0x80);     //as default
        // rc522_write_raw(DivlEnReg,0x0);      //as default
	    // rc522_write_raw(ComIrqReg,0x04);     //as default
	    // rc522_write_raw(DivIrqReg,0x0);      //as default
	    // rc522_write_raw(Status2Reg,0x0);//80    //trun off temperature sensor
	    // rc522_write_raw(WaterLevelReg,0x08); //as default
        // rc522_write_raw(ControlReg,0x20);    //as default
	    // rc522_write_raw(CollReg,0x80);    //as default

        rc522_write_raw(ModeReg,0x3D);//3F

        // rc522_write_raw(TxModeReg,0x0);      //as default???
        // rc522_write_raw(RxModeReg,0x0);      //as default???
        // rc522_write_raw(TxControlReg,0x80);  //as default???
        // rc522_write_raw(TxSelReg,0x10);      //as default???

        rc522_write_raw(RxSelReg,0x86);//84

        // rc522_write_raw(RxThresholdReg,0x84);//as default
        // rc522_write_raw(DemodReg,0x4D);      //as default

        // rc522_write_raw(ModWidthReg,0x13);//26
        rc522_write_raw(RFCfgReg,0x7F);   //4F
        // rc522_write_raw(GsNReg,0x88);        //as default???
        // rc522_write_raw(CWGsCfgReg,0x20);    //as default???
        // rc522_write_raw(ModGsCfgReg,0x20);   //as default???

        rc522_write_raw(TReloadRegL,30);  //tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
        rc522_write_raw(TReloadRegH,0);
        rc522_write_raw(TModeReg,0x8D);
        rc522_write_raw(TPrescalerReg,0x3E);
	   

        // PcdSetTmo(106);
	    delay(10);
        rc522_pcd_antenna_on();
    }
    else
    {
         return -1; 
    }
   
   return MI_OK;
}

#if 0
//========================================================================
// 描述: RC552往一块地址写入数据.
// 参数: key:密码(6字节);addr:块地址(0~63);value:写入的值(16字节)
// 返回: 1,操作成功;其他值,操作失败
//========================================================================
uint8 rc522_write(uint8 *key,uint8 addr,uint8 *value)
{
    uint8 ct[2];
    uint8 sn[4];
    uint8 status;
    status = MI_ERR;
    status = rc522_pcd_request(0x52,ct);  //寻卡。0x52 = 寻感应区内所有符合14443A标准的卡，ct返回卡片类型
    if(status != MI_OK)
 	{
         return 2;  //寻卡失败
 	}
    status = MI_ERR;
 	status = rc522_pcd_anticoll(sn);  //防冲撞。sn卡片序列号
    if(status != MI_OK)
 	{
         return 3;  //防冲撞失败
 	}  
    status = MI_ERR;
 	status = rc522_pcd_select(sn);  //选卡。sn卡片序列号
    if(status != MI_OK)
 	{
         return 4;  //选卡失败
 	}  
    status = MI_ERR;
 	status = rc522_pcd_auth_state(0x60,addr,key,sn);  //验证卡密码；0x60 = 验证A密钥，块地址（0-63），addr卡片密码，sn序列号
    if(status != MI_OK)
 	{
         return 5;  //验证卡密码失败
 	}  
    status = MI_ERR;
 	status = rc522_pcd_write(addr,value); //写入一块数据。addr块地址，
    if(status != MI_OK)
 	{
         return 6;  //写入失败失败
 	} 
    return 1;   //写入成功
}

//========================================================================
// 描述: RC552往一块地址读出数据.
// 参数: key:密码(6字节);addr:块地址(0~63);value:读出的值(16字节)
// 返回: 1,操作成功;其他值,操作失败
//========================================================================
uint8 rc522_read(uint8 *key,uint8 addr,uint8 *value)
{
    uint8 ct[2];
    uint8 sn[4];
    uint8 status;
    status = MI_ERR;
    status = rc522_pcd_request(0x52,ct);  //寻卡。0x52 = 寻感应区内所有符合14443A标准的卡，ct返回卡片类型
    if(status != MI_OK)
 	{
         return 2;  //寻卡失败
 	}
    status = MI_ERR;
 	status = rc522_pcd_anticoll(sn);  //防冲撞。sn卡片序列号
    if(status != MI_OK)
 	{
         return 3;  //防冲撞失败
 	}  
    status = MI_ERR;
 	status = rc522_pcd_select(sn);  //选卡。sn卡片序列号
    if(status != MI_OK)
 	{
         return 4;  //选卡失败
 	}  
    status = MI_ERR;
 	status = rc522_pcd_auth_state(0x60,addr,key,sn);  //验证卡密码；0x60 = 验证A密钥，块地址（0-63），addr卡片密码，sn序列号
    if(status != MI_OK)
 	{
         return 5;  //验证卡密码失败
 	}  
    status = MI_ERR;
 	status = rc522_pcd_read(addr,value); //读出一块数据。addr块地址，
    if(status != MI_OK)
 	{
         return 6;  //读出数据失败
 	} 
    return 1;   //读出成功
}
#endif

#endif      //rc522.h
