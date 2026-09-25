/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/
#ifndef __W25QXX_H
#define __W25QXX_H

#include <STC8HX.h>
#include "delay.h"
#include "hardspi.h"

#define     W25Q80 	            		0XEF13 	
#define     W25Q16 	            		0XEF14
#define     W25Q32 	            		0XEF15
#define     W25Q64 	            		0XEF16
#define     W25Q128	            		0XEF17

//指令表
#define 	W25QXX_WRITE_ENABLE		    0x06 
#define 	W25QXX_WRITE_DISABLE		0x04 
#define 	W25QXX_READ_STATUS_REG		0x05 
#define 	W25QXX_WRITE_STATUS_REG		0x01 
#define 	W25QXX_READ_DATA			0x03 
#define 	W25QXX_FAST_READ_DATA		0x0B 
#define 	W25QXX_FAST_READ_DUAL		0x3B 
#define 	W25QXX_PAGE_PROGRAM		    0x02 
#define 	W25QXX_BLOCK_ERASE			0xD8 
#define 	W25QXX_SECTOR_ERASE		    0x20 
#define 	W25QXX_CHIP_ERASE			0xC7 
#define 	W25QXX_POWER_DOWN			0xB9 
#define 	W25QXX_RELEASE_POWER_DOWN	0xAB 
#define 	W25QXX_DEVICE_ID			0xAB 
#define 	W25QXX_MANUFACT_DEVICE_ID	0x90 
#define 	W25QXX_JEDEC_DEVICE_ID		0x9F

#ifndef		W25QXX_CS
#define	    W25QXX_CS 		    		P2_2  		    //W25QXX的片选信号
#endif

#ifndef 	W25QXX_CS_MODE
#define		W25QXX_CS_MODE		{P2M1&=~0x04;P2M0|=0x04;}	//P22推挽输出
#endif

void w25qxx_init();	//w25qxx初始化
uint16 w25qxx_read_id();	//读取芯片ID
void w25qxx_read(uint8* pBuffer,uint32 ReadAddr,uint16 NumByteToRead);	//在指定地址开始读取指定长度的数据
void w25qxx_write(uint8* pBuffer,uint32 WriteAddr,uint16 NumByteToWrite);	//在指定地址开始写入指定长度的数据
void w25qxx_erase_chip();	//全片擦除，需等待很长时间
void w25qxx_erase_sector(uint32 Dst_Addr);	//擦除一个扇区
void w25qxx_power_down(); //w25qxx进入掉电模式
void w25qxx_wake_up();	//唤醒w25qxx

//========================================================================
// 描述: w25qxx初始化.
// 参数: none.
// 返回: none.
//========================================================================
void w25qxx_init()
{
    W25QXX_CS_MODE;//P22推挽输出
    W25QXX_CS = 1;
    hardspi_init(0);
}

//========================================================================
// 描述: 读取w25qxx的状态寄存器.
// 参数: none.
// 返回: none.
//=========================================================================
uint8 w25qxx_read_sr()   
{  
	uint8 byte=0;   
	W25QXX_CS=0;                           //选中spiflash
	hardspi_write_byte(W25QXX_READ_STATUS_REG);    //发送读取状态寄存器命令
	byte=hardspi_read_byte();                  //读取一个字节 
	W25QXX_CS=1;                           //取消片选     
	return byte;   
}

//========================================================================
// 描述: w25qxx等待空闲.
// 参数: none.
// 返回: none.
//=========================================================================
void w25qxx_wait_busy()   
{   
	while((w25qxx_read_sr()&0x01)==0x01);   //等待busy位清空
}

//========================================================================
// 描述: 写w25qxx的状态寄存器.
// 参数: none.
// 返回: none.
//=========================================================================
void w25qxx_write_sr(uint8 sr)   
{   
	W25QXX_CS=0;                             //选中spiflash  
	hardspi_write_byte(W25QXX_WRITE_STATUS_REG);     //发送写状态寄存器命令   
	hardspi_write_byte(sr);                      //写入一个字节
	W25QXX_CS=1;                             //取消片选 	      
} 

//========================================================================
// 描述: w25qxx写使能.
// 参数: none.
// 返回: none.
//=========================================================================
void w25qxx_write_enable()   
{
	W25QXX_CS=0;                              //选中spiflash   
    hardspi_write_byte(W25QXX_WRITE_ENABLE);         //发送写使能
	W25QXX_CS=1;                              //取消片选   	      
}

//========================================================================
// 描述: w25qxx写禁止.
// 参数: none.
// 返回: none.
//=========================================================================
void w25qxx_write_disable()   
{  
	W25QXX_CS=0;                              //选中spiflash   
    hardspi_write_byte(W25QXX_WRITE_DISABLE);    	  //发送写禁止指令    
	W25QXX_CS=1;                              //取消片选    
} 

//========================================================================
// 描述: 读取芯片ID.
// 参数: none.
// 返回: none.
//=========================================================================
uint16 w25qxx_read_id()
{
	uint16 Temp = 0;	  
	W25QXX_CS=0;				    
	hardspi_write_byte(0x90);                   //发送读取ID指令
	hardspi_write_byte(0x00); 	    
	hardspi_write_byte(0x00); 	    
	hardspi_write_byte(0x00); 	 			   
	Temp |= hardspi_read_byte()<<8;  
	Temp |= hardspi_read_byte();	 
	W25QXX_CS=1;				    
	return Temp;
} 

//========================================================================
// 描述: 全片擦除，需等待很长时间.
// 参数: none.
// 返回: none.
//=========================================================================
void w25qxx_erase_chip()   
{                                   
    w25qxx_write_enable();                  //写使能 
    w25qxx_wait_busy();   
  	W25QXX_CS=0;                            //片选使能   
    hardspi_write_byte(W25QXX_CHIP_ERASE);     //发送片擦除命令 
	W25QXX_CS=1;                            //取消片选     	      
	w25qxx_wait_busy();   				    //等待擦除完成
}  

//========================================================================
// 描述: 擦除一个扇区.
// 参数: Dst_Addr:扇区地址.(4K一个扇区，2M FLASH 扇区最大为512)
// 返回: none.
//=========================================================================
void w25qxx_erase_sector(uint32 Dst_Addr)   
{  	  
 	Dst_Addr*=4096;
    w25qxx_write_enable();                 		//写使能	 
    w25qxx_wait_busy();   
  	W25QXX_CS=0;                            	//片选使能
    hardspi_write_byte(W25QXX_SECTOR_ERASE);      		//发送扇区擦除指令
    hardspi_write_byte((uint8)((Dst_Addr)>>16));  		//发送24位地址 
    hardspi_write_byte((uint8)((Dst_Addr)>>8));   
    hardspi_write_byte((uint8)Dst_Addr);  
	W25QXX_CS=1;                            	//取消片选   	      
    w25qxx_wait_busy();   				   		//等待擦除完成
}

//========================================================================
// 描述: w25qxx进入掉电模式.
// 参数: none.
// 返回: none.
//=========================================================================
void w25qxx_power_down()   
{ 
  	W25QXX_CS=0;                            //片选使能   
    hardspi_write_byte(W25QXX_POWER_DOWN);     //发送掉电命令  
	W25QXX_CS=1;                            //取消片选 	      
    delay5us();                             //等待TPD 
}   

//========================================================================
// 描述: 唤醒w25qxx.
// 参数: none.
// 返回: none.
//=========================================================================
void w25qxx_wake_up()   
{  
  	W25QXX_CS=0;                            	//片选   
    hardspi_write_byte(W25QXX_RELEASE_POWER_DOWN);  	//发送唤醒命令   
	W25QXX_CS=1;                            	//取消片选     	      
    delay5us();                                 //等待TRES1
}  

//========================================================================
// 描述: 在指定地址开始读取指定长度的数据.
// 参数: none.
// 返回: none.
//=========================================================================
void w25qxx_read(uint8* pBuffer,uint32 ReadAddr,uint16 NumByteToRead)   
{ 
 	uint16 i;   										    
	W25QXX_CS=0;                           //选中spiflash      
    hardspi_write_byte(W25QXX_READ_DATA);         //发送读取命令
    hardspi_write_byte((uint8)((ReadAddr)>>16));  //发送24位地址    
    hardspi_write_byte((uint8)((ReadAddr)>>8));   
    hardspi_write_byte((uint8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=hardspi_read_byte();   //循环读数 
    }
	W25QXX_CS=1;  				    	      
} 

//========================================================================
// 描述: 在指定地址开始写入最大256字节的数据.
// 参数: none.
// 返回: none.
//=========================================================================
void w25qxx_write_page(uint8* pBuffer,uint32 WriteAddr,uint16 NumByteToWrite)
{
 	uint16 i;  
    w25qxx_write_enable();                  					//写使能
	W25QXX_CS=0;                            					//选中spiflash   
    hardspi_write_byte(W25QXX_PAGE_PROGRAM);      					//发送写页命令   
    hardspi_write_byte((uint8)((WriteAddr)>>16)); 					//发送24位地址   
    hardspi_write_byte((uint8)((WriteAddr)>>8));   
    hardspi_write_byte((uint8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)hardspi_write_byte(pBuffer[i]);//循环写数
	W25QXX_CS=1;                            					//取消片选
	w25qxx_wait_busy();					  						 //等待写入结束
} 

//========================================================================
// 描述: 无检验写spiflash.必须确保所写的地址范围内的数据全部为0XFF.确保地址不越界
// 参数: pBuffer:数据存储区; WriteAddr:开始写入的地址(24bit); 
//		NumByteToWrite:要写入的字节数(最大65535) .
// 返回: none.
//=========================================================================
void w25qxx_write_nocheck(uint8* pBuffer,uint32 WriteAddr,uint16 NumByteToWrite)   
{ 			 		 
	uint16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数	 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		w25qxx_write_page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else 	//NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	}	    
} 

//========================================================================
// 描述:在指定地址开始写入指定长度的数据
// 参数: pBuffer:数据存储区; WriteAddr:开始写入的地址(24bit); 
//		NumByteToWrite:要写入的字节数(最大65535) .
// 返回: none.
//=========================================================================	
xdata static uint8 *W25QXX_BUFFER; 
void w25qxx_write(uint8* pBuffer,uint32 WriteAddr,uint16 NumByteToWrite)   
{ 
	uint32 secpos;
	uint16 secoff;
	uint16 secremain;	   
 	uint16 i;
	uint8* W25QXX_BUF; 
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		w25qxx_read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除	  
		}
		if(i<secremain)//需要擦除
		{
			w25qxx_erase_sector(secpos);	//擦除整个扇区
			for(i=0;i<secremain;i++)	    //复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			w25qxx_write_nocheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区

		}else w25qxx_write_nocheck(pBuffer,WriteAddr,secremain);//写已经擦除了的，直接写入扇区剩余区间			   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //地址偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};	 
}

#endif  //w25qxx.h