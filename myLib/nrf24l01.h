#ifndef  __NRF24L01_H
#define  __NRF24L01_H

#include <stdlib.h>
#if defined (_C51)
#include "C51_softspi.h"
#elif defined (_STC12)
#include "STC12_softspi.h"
#elif defined (_STC15)
#include "STC15_softspi.h"
#else
#include "softspi.h"
#endif

#ifndef NRF24L01_CE_PIN
#define NRF24L01_CE_PIN        P5_4
#endif
#ifndef NRF24L01_CE_MODE
#define NRF24L01_CE_MODE       {P5M1&=~0x10;P5M0&=~0x10;}//双向IO口
#endif 

#ifndef NRF24L01_CSN_PIN
#define NRF24L01_CSN_PIN        P3_7
#endif
#ifndef NRF24L01_CSN_MODE
#define NRF24L01_CSN_MODE       {P3M1&=~0x80;P3M0&=~0x80;}//双向IO口
#endif 

#ifndef NRF24L01_IRQ_PIN
#define NRF24L01_IRQ_PIN        P2_2
#endif
#ifndef NRF24L01_IRQ_MODE
#define NRF24L01_IRQ_MODE       {P2M1&=~0x04;P2M0&=~0x04;}//双向IO口
#endif 

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

#define TX_ADR_WIDTH   5		// 5字节宽度的发送/接收地址
#define TX_PLOAD_WIDTH 32		// 数据通道有效数据宽度

xdata uint8 TX_ADDRESS[TX_ADR_WIDTH] = { 0x00, 0x66, 0x66, 0x66, 0x66 };	// 定义一个静态发送地址
xdata uint8 RX_ADDRESS[TX_ADR_WIDTH] = { 0x00, 0x66, 0x66, 0x66, 0x66 };	// 定义一个静态接收地址

xdata uint8 _NRF24L01_RX_BUF[TX_PLOAD_WIDTH];
// uint8 TX_BUF[TX_PLOAD_WIDTH];
xdata uint8 ch=7;
xdata uint8 rch=7;

xdata uint8  sta;

#define  RX_DR	 0x40
#define  TX_DS	 0x20
#define  MAX_RT  0x10

// SPI(nRF24L01) commands
#define READ_REG    0x00		// Define read command to register
#define WRITE_REG   0x20		// Define write command to register
#define RD_RX_PLOAD 0x61		// Define RX payload register address
#define WR_TX_PLOAD 0xA0		// Define TX payload register address
#define FLUSH_TX    0xE1		// Define flush TX register command
#define FLUSH_RX    0xE2		// Define flush RX register command
#define REUSE_TX_PL 0xE3		// Define reuse TX payload register command
#define NOP         0xFF		// Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses)
#define CONFIG      0x00		// 'Config' register address
#define EN_AA       0x01		// 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02		// 'Enabled RX addresses' register address
#define SETUP_AW    0x03		// 'Setup address width' register address
#define SETUP_RETR  0x04		// 'Setup Auto. Retrans' register address
#define RF_CH       0x05		// 'RF channel' register address
#define RF_SETUP    0x06		// 'RF setup' register address
#define STATUS      0x07		// 'Status' register address
#define OBSERVE_TX  0x08		// 'Observe TX' register address
#define CD          0x09		// 'Carrier Detect' register address
#define RX_ADDR_P0  0x0A		// 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B		// 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C		// 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D		// 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E		// 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F		// 'RX address pipe5' register address
#define TX_ADDR     0x10		// 'TX address' register address
#define RX_PW_P0    0x11		// 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12		// 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13		// 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14		// 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15		// 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16		// 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17		// 'FIFO Status Register' register address

void nrf24l01_radio_init();	//初始化

void nrf24l01_radio_tx_ch(uint8 channel);//设置无线发送通道
void nrf24l01_radio_tx_add(uint8* address); //设置无线发送地址
void nrf24l01_radio_tx_setup();//配置无线发送
void nrf24l01_radio_tx_nbyte(uint8 *buf); //无线发送

void nrf24l01_radio_rx_ch(uint8 channel); //设置无线接收通道
void nrf24l01_radio_rx_add(uint8* address); //设置无线接收地址
void nrf24l01_radio_rx_setup(); //配置无线接收
uint8 nrf24l01_radio_rx_nbyte(uint8 *buf);//无线接收存入buf中，返回1代表接收到数据，否则没有接收到数据

//uint8 nrf24l01_radio_rx(); //无线接收数据,并存入_NRF24L01_RX_BUF中,返回1代表接收到数据，否则没有接收到数据

//========================================================================
// 描述: 根据SPI协议，写一字节数据到nRF24L01，同时从nRF24L01读出一字节.
// 参数: 写入的字节数据.
// 返回: 读出的字节数据.
//========================================================================
uint8 nrf24l01_spi_rw(uint8 dat)
{
    return softspi_wr_data(dat);
    // return hardspi_wr_data(dat);
}

//========================================================================
// 描述: 写数据value到reg寄存器.
// 参数: reg:寄存器;value:数据.
// 返回: 状态.
//========================================================================
uint8 nrf24l01_spi_rw_reg(uint8 reg, uint8 value)
{
    uint8 status;
    NRF24L01_CSN_PIN = 0;   // CSN置低，开始传输数据
    status = nrf24l01_spi_rw(reg);  // 选择寄存器，同时返回状态字
    nrf24l01_spi_rw(value);     //写数据到reg寄存器
    NRF24L01_CSN_PIN = 1;   // CSN拉高，结束数据传输
    return (status);
}

//========================================================================
// 描述: 从reg寄存器读一字节数据.
// 参数: reg:寄存器.
// 返回: 寄存器数据.
//========================================================================
uint8 nrf24l01_spi_read(uint8 reg)
{
    uint8 reg_val;
    NRF24L01_CSN_PIN = 0;   // CSN置低，开始传输数据
    nrf24l01_spi_rw(reg);     //选择寄存器
    reg_val = nrf24l01_spi_rw(0);  //从reg中读取数据
    NRF24L01_CSN_PIN = 1;   // CSN拉高，结束数据传输
    return (reg_val);
}

//========================================================================
// 描述: 从reg寄存器读出bytes个字节，通常用来读取接收通道数据或接收/发送地址.
// 参数: reg:寄存器;pBuf:返回的数据指针;bytes:需要读取的字节数;.
// 返回: 状态寄存器.
//========================================================================
uint8 nrf24l01_spi_read_buf(uint8 reg, uint8 * pBuf, uint8 bytes)
{
    uint8 status, i;
    NRF24L01_CSN_PIN = 0;   // CSN置低，开始传输数据
    status = nrf24l01_spi_rw(reg);     // 选择寄存器，同时返回状态字
    for(i = 0; i < bytes; i++)
    {
        pBuf[i] = nrf24l01_spi_rw(0);	// 逐个字节从nRF24L01读出
    }
    NRF24L01_CSN_PIN = 1;   // CSN拉高，结束数据传输
    return (status);
}

//========================================================================
// 描述: 把pBuf缓存中的数据写入到nRF24L01，通常用来写入发射通道数据或接收/发送地址.
// 参数: reg:寄存器;pBuf:写入的数据指针;bytes:需要写入的字节数.
// 返回: 状态寄存器.
//========================================================================
uint8 nrf24l01_spi_write_buf(uint8 reg, uint8 * pBuf, uint8 bytes)
{
    uint8 status, i;
    NRF24L01_CSN_PIN = 0;   // CSN置低，开始传输数据
    status = nrf24l01_spi_rw(reg);     // 选择寄存器，同时返回状态字
    for(i = 0; i < bytes; i++)
    {
        nrf24l01_spi_rw(pBuf[i]);	// 逐个字节写入nRF24L01
    }
    NRF24L01_CSN_PIN = 1;   // CSN拉高，结束数据传输
    return (status);
}

//=======================================================================
// 描述: 初始化.
// 参数: none.
// 返回: none.
//========================================================================
void nrf24l01_radio_init()
{
#ifndef _C51
    NRF24L01_CE_MODE;
    NRF24L01_CSN_MODE;
    NRF24L01_IRQ_MODE;
#endif
    NRF24L01_IRQ_PIN = 0;
    NRF24L01_CE_PIN = 0;
    NRF24L01_CSN_PIN = 1;
    // hardspi_init(0); 
    softspi_init();
}

/***************************************************无线发送************************************/
//=======================================================================
// 描述: 设置无线发送通道.
// 参数: 通道.
// 返回: none.
//========================================================================
void nrf24l01_radio_tx_ch(uint8 channel)
{
    ch = channel;
}

//========================================================================
// 描述: 设置无线发送地址.
// 参数: 发送地址指针.
// 返回: none.
//========================================================================
void nrf24l01_radio_tx_add(uint8* address)
{
    uint8 i;
    for(i = 0; i < 5; i++){
        *(TX_ADDRESS+i)=*(address+i);
    }
}

//========================================================================
// 描述: 配置无线发送.
// 参数: none.
// 返回: none.
//========================================================================
void nrf24l01_radio_tx_setup()
{
    NRF24L01_CE_PIN = 0;

  	nrf24l01_spi_write_buf (WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	// 写入发送地址
	nrf24l01_spi_write_buf (WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// 为了应答接收设备，接收通道0地址和发送地址相同
	//nrf24l01_spi_write_buf (WR_TX_PLOAD, BUF, TX_PLOAD_WIDTH);	// 写数据包到TX FIFO
	//nrf24l01_spi_rw_reg (WRITE_REG + EN_AA, 0x01);	// 使能接收通道0自动应答
	nrf24l01_spi_rw_reg (WRITE_REG + EN_RXADDR, 0x01);	// 使能接收通道0
	nrf24l01_spi_rw_reg (WRITE_REG + SETUP_RETR, 0x03);	// 自动重发延时等待250us+86us，自动重发10次
	nrf24l01_spi_rw_reg (WRITE_REG + RF_CH, ch);	// 选择射频通道0x40
	nrf24l01_spi_rw_reg (WRITE_REG + RF_SETUP, 0x07);	// 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	nrf24l01_spi_rw_reg (WRITE_REG + CONFIG, 0x0e);	// CRC使能，16位CRC校验，上电
    //nrf24l01_spi_rw_reg (WRITE_REG + CONFIG, 0x02);

    NRF24L01_CE_PIN = 1;
}

//=======================================================================
// 描述: 无线发送32个字节数据.
// 参数: buf:数据.
// 返回: none.
//========================================================================
void nrf24l01_radio_tx_nbyte(uint8 *buf)
{
    uint8 sta;

    NRF24L01_CE_PIN = 0;
	//nrf24l01_spi_write_buf (WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	// 写入发送地址
	//nrf24l01_spi_write_buf (WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// 为了应答接收设备，接收通道0地址和发送地址相同
	nrf24l01_spi_write_buf (WR_TX_PLOAD, buf, TX_PLOAD_WIDTH);	// 写数据包到TX FIFO
	//nrf24l01_spi_write_buf (WRITE_REG + EN_AA, 0x01);	// 使能接收通道0自动应答
	//nrf24l01_spi_write_buf (WRITE_REG + EN_RXADDR, 0x01);	// 使能接收通道0
	//nrf24l01_spi_write_buf (WRITE_REG + SETUP_RETR, 0x0a);	// 自动重发延时等待250us+86us，自动重发10次
	//nrf24l01_spi_write_buf (WRITE_REG + RF_CH, ch);	// 选择射频通道0x40
	//nrf24l01_spi_write_buf (WRITE_REG + RF_SETUP, 0x07);	// 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	//nrf24l01_spi_write_buf (WRITE_REG + CONFIG, 0x0e);	// CRC使能，16位CRC校验，上电
	NRF24L01_CE_PIN = 1;
    // delay(1);
    while(NRF24L01_IRQ_PIN != 0);
    sta = nrf24l01_spi_read(STATUS);	  // 读状态寄存器
    nrf24l01_spi_rw_reg(WRITE_REG + STATUS, sta);  // 清除RX_DS中断标志
    if(sta&MAX_RT){
         nrf24l01_spi_rw_reg(FLUSH_TX, 0xff);  // 清除RX_DS中断标志
     }
}

/***************************************************无线接收************************************/
//=======================================================================
// 描述: 设置无线接收通道.
// 参数: 通道.
// 返回: none.
//========================================================================
void nrf24l01_radio_rx_ch(uint8 channel)
{
    ch = channel;
}

//========================================================================
// 描述: 设置无线接收地址.
// 参数: 接收地址指针.
// 返回: none.
//========================================================================
void nrf24l01_radio_rx_add(uint8* address)
{
    uint8 i;
    for(i = 0; i < 5; i++){
        *(RX_ADDRESS+i)=*(address+i);
    }
}

//========================================================================
// 描述: 配置无线接收.
// 参数: none.
// 返回: none.
//========================================================================
void nrf24l01_radio_rx_setup()
{
    NRF24L01_CE_PIN = 0;

	nrf24l01_spi_write_buf (WRITE_REG + RX_ADDR_P0, RX_ADDRESS, TX_ADR_WIDTH);	// 接收设备接收通道0使用和发送设备相同的发送地址
	nrf24l01_spi_rw_reg (WRITE_REG + EN_AA, 0x01);	// 使能接收通道0自动应答
	nrf24l01_spi_rw_reg (WRITE_REG + EN_RXADDR, 0x01);	// 使能接收通道0
	nrf24l01_spi_rw_reg (WRITE_REG + RF_CH, rch);	// 选择射频通道
	nrf24l01_spi_rw_reg (WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);	// 接收通道0选择和发送通道相同有效数据宽度
	nrf24l01_spi_rw_reg (WRITE_REG + RF_SETUP, 0x07);	// 0x07数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	nrf24l01_spi_rw_reg (WRITE_REG + CONFIG, 0x0f);	// CRC使能，16位CRC校验，上电，接收模式
    //nrf24l01_spi_rw_reg (WRITE_REG + CONFIG, 0x03);

    NRF24L01_CE_PIN = 1;
}

//=======================================================================
// 描述: 无线接收数据,并存入_NRF24L01_RX_BUF中.
// 参数: none.
// 返回: 1,接收到数据;0,没有接收到数据.
//========================================================================
//uint8 nrf24l01_radio_rx()
//{
//    uint8 sta;
//    sta = nrf24l01_spi_read(STATUS);	  // 读状态寄存器
//    nrf24l01_spi_rw_reg(WRITE_REG + STATUS, sta);  // 清除RX_DS中断标志
//	if(RX_DR&sta)				  // 判断是否接受到数据
//	{
//		nrf24l01_spi_read_buf(RD_RX_PLOAD,_NRF24L01_RX_BUF, TX_PLOAD_WIDTH);  // 从RX FIFO读出数据
//        nrf24l01_spi_rw_reg(FLUSH_RX, 0xff);  // 清除RX_DS中断标志
//        return 1;
//    }else{
//        return 0;
//    }
//}

//=======================================================================
// 描述: 无线接收存入buf中(最多接收32字节).
// 参数: buf:接收到的数据指针.
// 返回: 1,接收到数据;0,没有接收到数据.
//========================================================================
uint8 nrf24l01_radio_rx_nbyte(uint8 *buf)
{
    uint8 sta,i;
    sta = nrf24l01_spi_read(STATUS);	  // 读状态寄存器
    nrf24l01_spi_rw_reg(WRITE_REG + STATUS, sta);  // 清除RX_DS中断标志
	if(RX_DR&sta)				  // 判断是否接受到数据
	{
		nrf24l01_spi_read_buf(RD_RX_PLOAD, buf, TX_PLOAD_WIDTH);  // 从RX FIFO读出数据
        nrf24l01_spi_rw_reg(FLUSH_RX, 0xff);  // 清除RX_DS中断标志
        return 1;
    }else{
          return 0;
    }
}

#endif  //nrf24l01.h