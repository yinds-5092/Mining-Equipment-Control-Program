#ifndef _STC8HX_H_
#define _STC8HX_H_

#include <compiler.h>

#define _nop_() __asm nop __endasm
#define code __code
#define bit __bit
#define interrupt __interrupt
#define xdata __xdata
#define idata __idata
#define using __using
#define at __at
#define asm __asm
#define endasm __endasm

typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;
typedef signed char int8;
typedef signed int int16;
typedef signed long int32;
typedef volatile int8 vint8;
typedef volatile int16 vint16;
typedef volatile int32 vint32;
typedef volatile uint8 vuint8;
typedef volatile uint16 vuint16;
typedef volatile uint32 vuint32;
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;
typedef unsigned char uint8_t;
typedef unsigned long UINT32;

typedef struct
{
    BYTE bmRequestType;
    BYTE bRequest;
    BYTE wValueL;
    BYTE wValueH;
    BYTE wIndexL;
    BYTE wIndexH;
    BYTE wLengthL;
    BYTE wLengthH;
} SETUP;

typedef struct
{
    BYTE bStage;
    WORD wResidue;
    BYTE *pData;
} EP0STAGE;


/////////////////////////////////////////////////

//包含本头文件后,不用另外再包含"REG51.H"


/////////////////////////////////////////////////

SFR(P0, 0x80);
SBIT(P0_0, 0x80, 0);
SBIT(P0_1, 0x80, 1);
SBIT(P0_2, 0x80, 2);
SBIT(P0_3, 0x80, 3);
SBIT(P0_4, 0x80, 4);
SBIT(P0_5, 0x80, 5);
SBIT(P0_6, 0x80, 6);
SBIT(P0_7, 0x80, 7);

SFR(SP, 0x81);
SFR(DPL, 0x82);
SFR(DPH, 0x83);

SFR(S4CON, 0x84);
    #define S4SM0                   0x80
    #define S4ST4                   0x40
    #define S4SM2                   0x20
    #define S4REN                   0x10
    #define S4TB8                   0x08
    #define S4RB8                   0x04
    #define S4TI                    0x02
    #define S4RI                    0x01

SFR(S4BUF, 0x85);

SFR(PCON, 0x87);
    #define SMOD                    0x80
    #define SMOD0                   0x40
    #define LVDF                    0x20
    #define POF                     0x10
    #define GF1                     0x08
    #define GF0                     0x04
    #define PD                      0x02
    #define IDL                     0x01

SFR(TCON, 0x88);
SBIT(TF1, 0x88, 7);
SBIT(TR1, 0x88, 6);
SBIT(TF0, 0x88, 5);
SBIT(TR0, 0x88, 4);
SBIT(IE1, 0x88, 3);
SBIT(IT1, 0x88, 2);
SBIT(IE0, 0x88, 1);
SBIT(IT0, 0x88, 0);

SFR(TMOD, 0x89);
    #define T1_GATE                 0x80
    #define T1_CT                   0x40
    #define T1_M1                   0x20
    #define T1_M0                   0x10
    #define T0_GATE                 0x08
    #define T0_CT                   0x04
    #define T0_M1                   0x02
    #define T0_M0                   0x01

SFR(TL0, 0x8a);
SFR(TL1, 0x8b);
SFR(TH0, 0x8c);
SFR(TH1, 0x8d);

SFR(AUXR, 0x8e);
    #define T0x12                   0x80
    #define T1x12                   0x40
    #define S1M0x6                  0x20
    #define T2R                     0x10
    #define T2_CT                   0x08
    #define T2x12                   0x04
    #define EXTRAM                  0x02
    #define S1BRT                   0x01

SFR(INTCLKO, 0x8f);
    #define EX4                     0x40
    #define EX3                     0x20
    #define EX2                     0x10
    #define T2CLKO                  0x04
    #define T1CLKO                  0x02
    #define T0CLKO                  0x01

SFR(P1, 0x90);
SBIT(P1_0, 0x90, 0);
SBIT(P1_1, 0x90, 1);
SBIT(P1_2, 0x90, 2);
SBIT(P1_3, 0x90, 3);
SBIT(P1_4, 0x90, 4);
SBIT(P1_5, 0x90, 5);
SBIT(P1_6, 0x90, 6);
SBIT(P1_7, 0x90, 7);

SFR(P1M1, 0x91);
SFR(P1M0, 0x92);
SFR(P0M1, 0x93);
SFR(P0M0, 0x94);
SFR(P2M1, 0x95);
SFR(P2M0, 0x96);

SFR(SCON, 0x98);
SBIT(SM0, 0x98, 7);
SBIT(SM1, 0x98, 6);
SBIT(SM2, 0x98, 5);
SBIT(REN, 0x98, 4);
SBIT(TB8, 0x98, 3);
SBIT(RB8, 0x98, 2);
SBIT(TI, 0x98, 1);
SBIT(RI, 0x98, 0);

SFR(SBUF, 0x99);

SFR(S2CON, 0x9a);
    #define S2SM0                   0x80
    #define S2SM1                   0x40
    #define S2SM2                   0x20
    #define S2REN                   0x10
    #define S2TB8                   0x08
    #define S2RB8                   0x04
    #define S2TI                    0x02
    #define S2RI                    0x01

SFR(S2BUF, 0x9b);

SFR(IRCBAND, 0x9d);
    #define USBCKS                  0x80
    #define USBCKS2                 0x40
    #define HIRCSEL1                0x02
    #define HIRCSEL0                0x01

SFR(LIRTRIM, 0x9e);
SFR(IRTRIM, 0x9f);

SFR(P2, 0xa0);
SBIT(P2_0, 0xA0, 0);
SBIT(P2_1, 0xA0, 1);
SBIT(P2_2, 0xA0, 2);
SBIT(P2_3, 0xA0, 3);
SBIT(P2_4, 0xA0, 4);
SBIT(P2_5, 0xA0, 5);
SBIT(P2_6, 0xA0, 6);
SBIT(P2_7, 0xA0, 7);

SFR(BUS_SPEED, 0xa1);

SFR(P_SW1, 0xa2);
    #define UART1_S1                0x00
    #define UART1_S2                0x40
    #define UART1_S3                0x80
    #define UART1_S4                0xc0
    #define SPI_S1                  0x00
    #define SPI_S2                  0x04
    #define SPI_S3                  0x08
    #define SPI_S4                  0x0c

SFR(VRTRIM, 0xa6);

SFR(IE, 0xa8);
SBIT(EA, 0xA8, 7);
SBIT(ELVD, 0xA8, 6);
SBIT(EADC, 0xA8, 5);
SBIT(ES, 0xA8, 4);
SBIT(ET1, 0xA8, 3);
SBIT(EX1, 0xA8, 2);
SBIT(ET0, 0xA8, 1);
SBIT(EX0, 0xA8, 0);

SFR(SADDR, 0xa9);
SFR(WKTCL, 0xaa);
SFR(WKTCH, 0xab);
    #define WKTEN                   0x80

SFR(S3CON, 0xac);
    #define S3SM0                   0x80
    #define S3ST3                   0x40
    #define S3SM2                   0x20
    #define S3REN                   0x10
    #define S3TB8                   0x08
    #define S3RB8                   0x04
    #define S3TI                    0x02
    #define S3RI                    0x01

SFR(S3BUF, 0xad);
SFR(TA, 0xae);
SFR(IE2, 0xaf);
    #define EUSB                    0x80
    #define ET4                     0x40
    #define ET3                     0x20
    #define ES4                     0x10
    #define ES3                     0x08
    #define ET2                     0x04
    #define ESPI                    0x02
    #define ES2                     0x01

SFR(P3, 0xb0);
SBIT(P3_0, 0xB0, 0);
SBIT(P3_1, 0xB0, 1);
SBIT(P3_2, 0xB0, 2);
SBIT(P3_3, 0xB0, 3);
SBIT(P3_4, 0xB0, 4);
SBIT(P3_5, 0xB0, 5);
SBIT(P3_6, 0xB0, 6);
SBIT(P3_7, 0xB0, 7);

SBIT(RD, 0xB0, 7);
SBIT(WR, 0xB0, 6);
SBIT(T1, 0xB0, 5);
SBIT(T0, 0xB0, 4);
SBIT(INT1, 0xB0, 3);
SBIT(INT0, 0xB0, 2);
SBIT(TXD, 0xB0, 1);
SBIT(RXD, 0xB0, 0);

SFR(P3M1, 0xb1);
SFR(P3M0, 0xb2);
SFR(P4M1, 0xb3);
SFR(P4M0, 0xb4);

SFR(IP2, 0xb5);
    #define PUSB                    0x80
    #define PI2C                    0x40
    #define PCMP                    0x20
    #define PX4                     0x10
    #define PPWMB                   0x08
    #define PPWMA                   0x04
    #define PSPI                    0x02
    #define PS2                     0x01

SFR(IP2H, 0xb6);
    #define PUSBH                   0x80
    #define PI2CH                   0x40
    #define PCMPH                   0x20
    #define PX4H                    0x10
    #define PPWMBH                  0x08
    #define PPWMAH                  0x04
    #define PSPIH                   0x02
    #define PS2H                    0x01

SFR(IPH, 0xb7);
    #define PLVDH                   0x40
    #define PADCH                   0x20
    #define PSH                     0x10
    #define PT1H                    0x08
    #define PX1H                    0x04
    #define PT0H                    0x02
    #define PX0H                    0x01

SFR(IP, 0xb8);
SBIT(PLVD, 0xB8, 6);
SBIT(PADC, 0xB8, 5);
SBIT(PS, 0xB8, 4);
SBIT(PT1, 0xB8, 3);
SBIT(PX1, 0xB8, 2);
SBIT(PT0, 0xB8, 1);
SBIT(PX0, 0xB8, 0);

SFR(SADEN, 0xb9);

SFR(P_SW2, 0xba);
    #define EAXFR                   0x80
    #define I2C_S1                  0x00
    #define I2C_S2                  0x10
    #define I2C_S3                  0x20
    #define I2C_S4                  0x30
    #define CMPO_S1                 0x00
    #define CMPO_S2                 0x08
    #define UART4_S1                0x00
    #define UART4_S2                0x04
    #define UART3_S1                0x00
    #define UART3_S2                0x02
    #define UART2_S1                0x00
    #define UART2_S2                0x01

SFR(ADC_CONTR, 0xbc);
    #define ADC_POWER               0x80
    #define ADC_START               0x40
    #define ADC_FLAG                0x20
    #define ADC_EPWMT               0x10

SFR(ADC_RES, 0xbd);
SFR(ADC_RESL, 0xbe);

SFR(P4, 0xc0);
SBIT(P4_0, 0xC0, 0);
SBIT(P4_1, 0xC0, 1);
SBIT(P4_2, 0xC0, 2);
SBIT(P4_3, 0xC0, 3);
SBIT(P4_4, 0xC0, 4);
SBIT(P4_5, 0xC0, 5);
SBIT(P4_6, 0xC0, 6);
SBIT(P4_7, 0xC0, 7);

SFR(WDT_CONTR, 0xc1);
    #define WDT_FLAG                0x80
    #define EN_WDT                  0x20
    #define CLR_WDT                 0x10
    #define IDL_WDT                 0x08

SFR(IAP_DATA, 0xc2);
SFR(IAP_ADDRH, 0xc3);
SFR(IAP_ADDRL, 0xc4);
SFR(IAP_CMD, 0xc5);
SFR(IAP_TRIG, 0xc6);

SFR(IAP_CONTR, 0xc7);
    #define IAPEN                   0x80
    #define SWBS                    0x40
    #define SWRST                   0x20
    #define CMD_FAIL                0x10

SFR(P5, 0xc8);
SBIT(P5_0, 0xC8, 0);
SBIT(P5_1, 0xC8, 1);
SBIT(P5_2, 0xC8, 2);
SBIT(P5_3, 0xC8, 3);
SBIT(P5_4, 0xC8, 4);
SBIT(P5_5, 0xC8, 5);
SBIT(P5_6, 0xC8, 6);
SBIT(P5_7, 0xC8, 7);

SFR(P5M1, 0xc9);
SFR(P5M0, 0xca);
SFR(P6M1, 0xcb);
SFR(P6M0, 0xcc);

SFR(SPSTAT, 0xcd);
    #define SPIF                    0x80
    #define WCOL                    0x40

SFR(SPCTL, 0xce);
    #define SSIG                    0x80
    #define SPEN                    0x40
    #define DORD                    0x20
    #define MSTR                    0x10
    #define CPOL                    0x08
    #define CPHA                    0x04
    #define SPR1                    0x02
    #define SPR0                    0x01

SFR(SPDAT, 0xcf);

SFR(PSW, 0xd0);
SBIT(CY, 0xD0, 7);
SBIT(AC, 0xD0, 6);
SBIT(F0, 0xD0, 5);
SBIT(RS1, 0xD0, 4);
SBIT(RS0, 0xD0, 3);
SBIT(OV, 0xD0, 2);
SBIT(F1, 0xD0, 1);
SBIT(P, 0xD0, 0);

SFR(T4T3M, 0xd1);
    #define T4R                     0x80
    #define T4_CT                   0x40
    #define T4x12                   0x20
    #define T4CLKO                  0x10
    #define T3R                     0x08
    #define T3_CT                   0x04
    #define T3x12                   0x02
    #define T3CLKO                  0x01

SFR(T4H, 0xd2);
SFR(T4L, 0xd3);
SFR(T3H, 0xd4);
SFR(T3L, 0xd5);
SFR(T2H, 0xd6);
SFR(T2L, 0xd7);

SFR(T3T4M, 0xd1);
SFR(TH4, 0xd2);
SFR(TL4, 0xd3);
SFR(TH3, 0xd4);
SFR(TL3, 0xd5);
SFR(TH2, 0xd6);
SFR(TL2, 0xd7);

SFR(USBCLK, 0xdc);
SFR(PLLCR, 0xdc);
SFR(MDU32SR, 0xdd);

SFR(ADCCFG, 0xde);
    #define RESFMT                  0x20

SFR(IP3, 0xdf);
    #define PRTC                    0x04
    #define PS4                     0x02
    #define PS3                     0x01

SFR(ACC, 0xE0);
SBIT(ACC0, 0xE0, 0);
SBIT(ACC1, 0xE0, 1);
SBIT(ACC2, 0xE0, 2);
SBIT(ACC3, 0xE0, 3);
SBIT(ACC4, 0xE0, 4);
SBIT(ACC5, 0xE0, 5);
SBIT(ACC6, 0xE0, 6);
SBIT(ACC7, 0xE0, 7);

SFR(P7M1, 0xe1);
SFR(P7M0, 0xe2);
SFR(DPS, 0xe3);
SFR(DPL1, 0xe4);
SFR(DPH1, 0xe5);

SFR(CMPCR1, 0xe6);
    #define CMPEN                   0x80
    #define CMPIF                   0x40
    #define PIE                     0x20
    #define NIE                     0x10
    #define CMPOE                   0x02
    #define CMPRES                  0x01

SFR(CMPCR2, 0xe7);
    #define INVCMPO                 0x80
    #define DISFLT                  0x40

SFR(P6, 0xe8);
SBIT(P6_0, 0xE8, 0);
SBIT(P6_1, 0xE8, 1);
SBIT(P6_2, 0xE8, 2);
SBIT(P6_3, 0xE8, 3);
SBIT(P6_4, 0xE8, 4);
SBIT(P6_5, 0xE8, 5);
SBIT(P6_6, 0xE8, 6);
SBIT(P6_7, 0xE8, 7);

SFR(USBDAT, 0xec);
SFR(MDU32CR, 0xed);

SFR(IP3H, 0xee);
    #define PRTCH                   0x04
    #define PS4H                    0x02
    #define PS3H                    0x01

SFR(AUXINTIF, 0xef);
    #define INT4IF                  0x40
    #define INT3IF                  0x20
    #define INT2IF                  0x10
    #define T4IF                    0x04
    #define T3IF                    0x02
    #define T2IF                    0x01

SFR(B, 0xF0);
SBIT(B0, 0xF0, 0);
SBIT(B1, 0xF0, 1);
SBIT(B2, 0xF0, 2);
SBIT(B3, 0xF0, 3);
SBIT(B4, 0xF0, 4);
SBIT(B5, 0xF0, 5);
SBIT(B6, 0xF0, 6);
SBIT(B7, 0xF0, 7);

SFR(USBCON, 0xf4);
    #define ENUSB                   0x80
    #define ENUSBRST                0x40
    #define PS2M                    0x20
    #define PUEN                    0x10
    #define PDEN                    0x08
    #define DFREC                   0x04
    #define DP                      0x02
    #define DM                      0x01

SFR(IAP_TPS, 0xf5);

SFR(P7, 0xf8);
SBIT(P7_0, 0xF8, 0);
SBIT(P7_1, 0xF8, 1);
SBIT(P7_2, 0xF8, 2);
SBIT(P7_3, 0xF8, 3);
SBIT(P7_4, 0xF8, 4);
SBIT(P7_5, 0xF8, 5);
SBIT(P7_6, 0xF8, 6);
SBIT(P7_7, 0xF8, 7);

SFR(USBADR, 0xfc);

SFR(RSTCFG, 0xff);
    #define ENLVR                   0x40
    #define P54RST                  0x10

/////////////////////////////////////////////////
//
//如下特殊功能寄存器位于扩展RAM区域
//访问这些寄存器,需先将EAXFR设置为1,才可正常读写
//    EAXFR = 1;
//或者
//    P_SW2 |= 0x80;
///////////////////////////////////////////////////

/////////////////////////////////////////////////
//FF00H-FFFFH
/////////////////////////////////////////////////



/////////////////////////////////////////////////
//FE00H-FEFFH
/////////////////////////////////////////////////

#define     CLKSEL                  (*(unsigned char volatile xdata *)0xfe00)
#define     CLKDIV                  (*(unsigned char volatile xdata *)0xfe01)
#define     HIRCCR                  (*(unsigned char volatile xdata *)0xfe02)
#define     XOSCCR                  (*(unsigned char volatile xdata *)0xfe03)
#define     IRC32KCR                (*(unsigned char volatile xdata *)0xfe04)
#define     MCLKOCR                 (*(unsigned char volatile xdata *)0xfe05)
#define     IRCDB                   (*(unsigned char volatile xdata *)0xfe06)
#define     IRC48MCR                (*(unsigned char volatile xdata *)0xfe07)
#define     X32KCR                  (*(unsigned char volatile xdata *)0xfe08)
#define     HSCLKDIV                (*(unsigned char volatile xdata *)0xfe0b)

#define     P0PU                    (*(unsigned char volatile xdata *)0xfe10)
#define     P1PU                    (*(unsigned char volatile xdata *)0xfe11)
#define     P2PU                    (*(unsigned char volatile xdata *)0xfe12)
#define     P3PU                    (*(unsigned char volatile xdata *)0xfe13)
#define     P4PU                    (*(unsigned char volatile xdata *)0xfe14)
#define     P5PU                    (*(unsigned char volatile xdata *)0xfe15)
#define     P6PU                    (*(unsigned char volatile xdata *)0xfe16)
#define     P7PU                    (*(unsigned char volatile xdata *)0xfe17)
#define     P0NCS                   (*(unsigned char volatile xdata *)0xfe18)
#define     P1NCS                   (*(unsigned char volatile xdata *)0xfe19)
#define     P2NCS                   (*(unsigned char volatile xdata *)0xfe1a)
#define     P3NCS                   (*(unsigned char volatile xdata *)0xfe1b)
#define     P4NCS                   (*(unsigned char volatile xdata *)0xfe1c)
#define     P5NCS                   (*(unsigned char volatile xdata *)0xfe1d)
#define     P6NCS                   (*(unsigned char volatile xdata *)0xfe1e)
#define     P7NCS                   (*(unsigned char volatile xdata *)0xfe1f)
#define     P0SR                    (*(unsigned char volatile xdata *)0xfe20)
#define     P1SR                    (*(unsigned char volatile xdata *)0xfe21)
#define     P2SR                    (*(unsigned char volatile xdata *)0xfe22)
#define     P3SR                    (*(unsigned char volatile xdata *)0xfe23)
#define     P4SR                    (*(unsigned char volatile xdata *)0xfe24)
#define     P5SR                    (*(unsigned char volatile xdata *)0xfe25)
#define     P6SR                    (*(unsigned char volatile xdata *)0xfe26)
#define     P7SR                    (*(unsigned char volatile xdata *)0xfe27)
#define     P0DR                    (*(unsigned char volatile xdata *)0xfe28)
#define     P1DR                    (*(unsigned char volatile xdata *)0xfe29)
#define     P2DR                    (*(unsigned char volatile xdata *)0xfe2a)
#define     P3DR                    (*(unsigned char volatile xdata *)0xfe2b)
#define     P4DR                    (*(unsigned char volatile xdata *)0xfe2c)
#define     P5DR                    (*(unsigned char volatile xdata *)0xfe2d)
#define     P6DR                    (*(unsigned char volatile xdata *)0xfe2e)
#define     P7DR                    (*(unsigned char volatile xdata *)0xfe2f)
#define     P0IE                    (*(unsigned char volatile xdata *)0xfe30)
#define     P1IE                    (*(unsigned char volatile xdata *)0xfe31)
#define     P2IE                    (*(unsigned char volatile xdata *)0xfe32)
#define     P3IE                    (*(unsigned char volatile xdata *)0xfe33)
#define     P4IE                    (*(unsigned char volatile xdata *)0xfe34)
#define     P5IE                    (*(unsigned char volatile xdata *)0xfe35)
#define     P6IE                    (*(unsigned char volatile xdata *)0xfe36)
#define     P7IE                    (*(unsigned char volatile xdata *)0xfe37)
#define     P0PD                    (*(unsigned char volatile xdata *)0xfe40)
#define     P1PD                    (*(unsigned char volatile xdata *)0xfe41)
#define     P2PD                    (*(unsigned char volatile xdata *)0xfe42)
#define     P3PD                    (*(unsigned char volatile xdata *)0xfe43)
#define     P4PD                    (*(unsigned char volatile xdata *)0xfe44)
#define     P5PD                    (*(unsigned char volatile xdata *)0xfe45)
#define     P6PD                    (*(unsigned char volatile xdata *)0xfe46)
#define     P7PD                    (*(unsigned char volatile xdata *)0xfe47)

#define     LCMIFCFG                (*(unsigned char volatile xdata *)0xfe50)
#define     LCMIFCFG2               (*(unsigned char volatile xdata *)0xfe51)
#define     LCMIFCR                 (*(unsigned char volatile xdata *)0xfe52)
#define     LCMIFSTA                (*(unsigned char volatile xdata *)0xfe53)
#define     LCMIFDATL               (*(unsigned char volatile xdata *)0xfe54)
#define     LCMIFDATH               (*(unsigned char volatile xdata *)0xfe55)

#define     RTCCR                   (*(unsigned char volatile xdata *)0xfe60)
#define     RTCCFG                  (*(unsigned char volatile xdata *)0xfe61)
#define     RTCIEN                  (*(unsigned char volatile xdata *)0xfe62)
#define     RTCIF                   (*(unsigned char volatile xdata *)0xfe63)
#define     ALAHOUR                 (*(unsigned char volatile xdata *)0xfe64)
#define     ALAMIN                  (*(unsigned char volatile xdata *)0xfe65)
#define     ALASEC                  (*(unsigned char volatile xdata *)0xfe66)
#define     ALASSEC                 (*(unsigned char volatile xdata *)0xfe67)
#define     INIYEAR                 (*(unsigned char volatile xdata *)0xfe68)
#define     INIMONTH                (*(unsigned char volatile xdata *)0xfe69)
#define     INIDAY                  (*(unsigned char volatile xdata *)0xfe6a)
#define     INIHOUR                 (*(unsigned char volatile xdata *)0xfe6b)
#define     INIMIN                  (*(unsigned char volatile xdata *)0xfe6c)
#define     INISEC                  (*(unsigned char volatile xdata *)0xfe6d)
#define     INISSEC                 (*(unsigned char volatile xdata *)0xfe6e)
#define     INIWEEK                 (*(unsigned char volatile xdata *)0xfe6f)
#define     RTCWEEK                 (*(unsigned char volatile xdata *)0xfe6f)
#define     RTCYEAR                 (*(unsigned char volatile xdata *)0xfe70)
#define     RTCMONTH                (*(unsigned char volatile xdata *)0xfe71)
#define     RTCDAY                  (*(unsigned char volatile xdata *)0xfe72)
#define     RTCHOUR                 (*(unsigned char volatile xdata *)0xfe73)
#define     RTCMIN                  (*(unsigned char volatile xdata *)0xfe74)
#define     RTCSEC                  (*(unsigned char volatile xdata *)0xfe75)
#define     RTCSSEC                 (*(unsigned char volatile xdata *)0xfe76)

#define     T11CR                   (*(unsigned char volatile xdata *)0xfe78)
#define     T11PS                   (*(unsigned char volatile xdata *)0xfe79)
#define     T11H                    (*(unsigned char volatile xdata *)0xfe7a)
#define     T11L                    (*(unsigned char volatile xdata *)0xfe7b)

#define     I2CCFG                  (*(unsigned char volatile xdata *)0xfe80)
#define     I2CMSCR                 (*(unsigned char volatile xdata *)0xfe81)
#define     I2CMSST                 (*(unsigned char volatile xdata *)0xfe82)
#define     I2CSLCR                 (*(unsigned char volatile xdata *)0xfe83)
#define     I2CSLST                 (*(unsigned char volatile xdata *)0xfe84)
#define     I2CSLADR                (*(unsigned char volatile xdata *)0xfe85)
#define     I2CTXD                  (*(unsigned char volatile xdata *)0xfe86)
#define     I2CRXD                  (*(unsigned char volatile xdata *)0xfe87)
#define     I2CMSAUX                (*(unsigned char volatile xdata *)0xfe88)

#define     RSTFLAG                 (*(unsigned char volatile xdata *)0xfe99)

#define     TM0PS                   (*(unsigned char volatile xdata *)0xfea0)
#define     TM1PS                   (*(unsigned char volatile xdata *)0xfea1)
#define     TM2PS                   (*(unsigned char volatile xdata *)0xfea2)
#define     TM3PS                   (*(unsigned char volatile xdata *)0xfea3)
#define     TM4PS                   (*(unsigned char volatile xdata *)0xfea4)
#define     ADCTIM                  (*(unsigned char volatile xdata *)0xfea8)
#define     T3T4PIN                 (*(unsigned char volatile xdata *)0xfeac)
#define     ADCEXCFG                (*(unsigned char volatile xdata *)0xfead)
#define     CMPEXCFG                (*(unsigned char volatile xdata *)0xfeae)

#define     PWM1_ETRPS              (*(unsigned char volatile xdata *)0xfeb0)
#define     PWM1_ENO                (*(unsigned char volatile xdata *)0xfeb1)
#define     PWM1_PS                 (*(unsigned char volatile xdata *)0xfeb2)
#define     PWM1_IOAUX              (*(unsigned char volatile xdata *)0xfeb3)
#define     PWM2_ETRPS              (*(unsigned char volatile xdata *)0xfeb4)
#define     PWM2_ENO                (*(unsigned char volatile xdata *)0xfeb5)
#define     PWM2_PS                 (*(unsigned char volatile xdata *)0xfeb6)
#define     PWM2_IOAUX              (*(unsigned char volatile xdata *)0xfeb7)

#define     PWM1_CR1                (*(unsigned char volatile xdata *)0xfec0)
#define     PWM1_CR2                (*(unsigned char volatile xdata *)0xfec1)
#define     PWM1_SMCR               (*(unsigned char volatile xdata *)0xfec2)
#define     PWM1_ETR                (*(unsigned char volatile xdata *)0xfec3)
#define     PWM1_IER                (*(unsigned char volatile xdata *)0xfec4)
#define     PWM1_SR1                (*(unsigned char volatile xdata *)0xfec5)
#define     PWM1_SR2                (*(unsigned char volatile xdata *)0xfec6)
#define     PWM1_EGR                (*(unsigned char volatile xdata *)0xfec7)
#define     PWM1_CCMR1              (*(unsigned char volatile xdata *)0xfec8)
#define     PWM1_CCMR2              (*(unsigned char volatile xdata *)0xfec9)
#define     PWM1_CCMR3              (*(unsigned char volatile xdata *)0xfeca)
#define     PWM1_CCMR4              (*(unsigned char volatile xdata *)0xfecb)
#define     PWM1_CCER1              (*(unsigned char volatile xdata *)0xfecc)
#define     PWM1_CCER2              (*(unsigned char volatile xdata *)0xfecd)
#define     PWM1_CNTR               (*(unsigned  int volatile xdata *)0xfece)
#define     PWM1_CNTRH              (*(unsigned char volatile xdata *)0xfece)
#define     PWM1_CNTRL              (*(unsigned char volatile xdata *)0xfecf)
#define     PWM1_PSCR               (*(unsigned  int volatile xdata *)0xfed0)
#define     PWM1_PSCRH              (*(unsigned char volatile xdata *)0xfed0)
#define     PWM1_PSCRL              (*(unsigned char volatile xdata *)0xfed1)
#define     PWM1_ARR                (*(unsigned  int volatile xdata *)0xfed2)
#define     PWM1_ARRH               (*(unsigned char volatile xdata *)0xfed2)
#define     PWM1_ARRL               (*(unsigned char volatile xdata *)0xfed3)
#define     PWM1_RCR                (*(unsigned char volatile xdata *)0xfed4)
#define     PWM1_CCR1               (*(unsigned  int volatile xdata *)0xfed5)
#define     PWM1_CCR1H              (*(unsigned char volatile xdata *)0xfed5)
#define     PWM1_CCR1L              (*(unsigned char volatile xdata *)0xfed6)
#define     PWM1_CCR2               (*(unsigned  int volatile xdata *)0xfed7)
#define     PWM1_CCR2H              (*(unsigned char volatile xdata *)0xfed7)
#define     PWM1_CCR2L              (*(unsigned char volatile xdata *)0xfed8)
#define     PWM1_CCR3               (*(unsigned  int volatile xdata *)0xfed9)
#define     PWM1_CCR3H              (*(unsigned char volatile xdata *)0xfed9)
#define     PWM1_CCR3L              (*(unsigned char volatile xdata *)0xfeda)
#define     PWM1_CCR4               (*(unsigned  int volatile xdata *)0xfedb)
#define     PWM1_CCR4H              (*(unsigned char volatile xdata *)0xfedb)
#define     PWM1_CCR4L              (*(unsigned char volatile xdata *)0xfedc)
#define     PWM1_BKR                (*(unsigned char volatile xdata *)0xfedd)
#define     PWM1_DTR                (*(unsigned char volatile xdata *)0xfede)
#define     PWM1_OISR               (*(unsigned char volatile xdata *)0xfedf)

#define     PWM2_CR1                (*(unsigned char volatile xdata *)0xfee0)
#define     PWM2_CR2                (*(unsigned char volatile xdata *)0xfee1)
#define     PWM2_SMCR               (*(unsigned char volatile xdata *)0xfee2)
#define     PWM2_ETR                (*(unsigned char volatile xdata *)0xfee3)
#define     PWM2_IER                (*(unsigned char volatile xdata *)0xfee4)
#define     PWM2_SR1                (*(unsigned char volatile xdata *)0xfee5)
#define     PWM2_SR2                (*(unsigned char volatile xdata *)0xfee6)
#define     PWM2_EGR                (*(unsigned char volatile xdata *)0xfee7)
#define     PWM2_CCMR1              (*(unsigned char volatile xdata *)0xfee8)
#define     PWM2_CCMR2              (*(unsigned char volatile xdata *)0xfee9)
#define     PWM2_CCMR3              (*(unsigned char volatile xdata *)0xfeea)
#define     PWM2_CCMR4              (*(unsigned char volatile xdata *)0xfeeb)
#define     PWM2_CCER1              (*(unsigned char volatile xdata *)0xfeec)
#define     PWM2_CCER2              (*(unsigned char volatile xdata *)0xfeed)
#define     PWM2_CNTR               (*(unsigned  int volatile xdata *)0xfeee)
#define     PWM2_CNTRH              (*(unsigned char volatile xdata *)0xfeee)
#define     PWM2_CNTRL              (*(unsigned char volatile xdata *)0xfeef)
#define     PWM2_PSCR               (*(unsigned  int volatile xdata *)0xfef0)
#define     PWM2_PSCRH              (*(unsigned char volatile xdata *)0xfef0)
#define     PWM2_PSCRL              (*(unsigned char volatile xdata *)0xfef1)
#define     PWM2_ARR                (*(unsigned  int volatile xdata *)0xfef2)
#define     PWM2_ARRH               (*(unsigned char volatile xdata *)0xfef2)
#define     PWM2_ARRL               (*(unsigned char volatile xdata *)0xfef3)
#define     PWM2_RCR                (*(unsigned char volatile xdata *)0xfef4)
#define     PWM2_CCR1               (*(unsigned  int volatile xdata *)0xfef5)
#define     PWM2_CCR1H              (*(unsigned char volatile xdata *)0xfef5)
#define     PWM2_CCR1L              (*(unsigned char volatile xdata *)0xfef6)
#define     PWM2_CCR2               (*(unsigned  int volatile xdata *)0xfef7)
#define     PWM2_CCR2H              (*(unsigned char volatile xdata *)0xfef7)
#define     PWM2_CCR2L              (*(unsigned char volatile xdata *)0xfef8)
#define     PWM2_CCR3               (*(unsigned  int volatile xdata *)0xfef9)
#define     PWM2_CCR3H              (*(unsigned char volatile xdata *)0xfef9)
#define     PWM2_CCR3L              (*(unsigned char volatile xdata *)0xfefa)
#define     PWM2_CCR4               (*(unsigned  int volatile xdata *)0xfefb)
#define     PWM2_CCR4H              (*(unsigned char volatile xdata *)0xfefb)
#define     PWM2_CCR4L              (*(unsigned char volatile xdata *)0xfefc)
#define     PWM2_BKR                (*(unsigned char volatile xdata *)0xfefd)
#define     PWM2_DTR                (*(unsigned char volatile xdata *)0xfefe)
#define     PWM2_OISR               (*(unsigned char volatile xdata *)0xfeff)

#define     PWMA_ETRPS              (*(unsigned char volatile xdata *)0xfeb0)
#define     PWMA_ENO                (*(unsigned char volatile xdata *)0xfeb1)
#define     PWMA_PS                 (*(unsigned char volatile xdata *)0xfeb2)
#define     PWMA_IOAUX              (*(unsigned char volatile xdata *)0xfeb3)
#define     PWMB_ETRPS              (*(unsigned char volatile xdata *)0xfeb4)
#define     PWMB_ENO                (*(unsigned char volatile xdata *)0xfeb5)
#define     PWMB_PS                 (*(unsigned char volatile xdata *)0xfeb6)
#define     PWMB_IOAUX              (*(unsigned char volatile xdata *)0xfeb7)

#define     PWMA_CR1                (*(unsigned char volatile xdata *)0xfec0)
#define     PWMA_CR2                (*(unsigned char volatile xdata *)0xfec1)
#define     PWMA_SMCR               (*(unsigned char volatile xdata *)0xfec2)
#define     PWMA_ETR                (*(unsigned char volatile xdata *)0xfec3)
#define     PWMA_IER                (*(unsigned char volatile xdata *)0xfec4)
#define     PWMA_SR1                (*(unsigned char volatile xdata *)0xfec5)
#define     PWMA_SR2                (*(unsigned char volatile xdata *)0xfec6)
#define     PWMA_EGR                (*(unsigned char volatile xdata *)0xfec7)
#define     PWMA_CCMR1              (*(unsigned char volatile xdata *)0xfec8)
#define     PWMA_CCMR2              (*(unsigned char volatile xdata *)0xfec9)
#define     PWMA_CCMR3              (*(unsigned char volatile xdata *)0xfeca)
#define     PWMA_CCMR4              (*(unsigned char volatile xdata *)0xfecb)
#define     PWMA_CCER1              (*(unsigned char volatile xdata *)0xfecc)
#define     PWMA_CCER2              (*(unsigned char volatile xdata *)0xfecd)
#define     PWMA_CNTR               (*(unsigned  int volatile xdata *)0xfece)
#define     PWMA_CNTRH              (*(unsigned char volatile xdata *)0xfece)
#define     PWMA_CNTRL              (*(unsigned char volatile xdata *)0xfecf)
#define     PWMA_PSCR               (*(unsigned  int volatile xdata *)0xfed0)
#define     PWMA_PSCRH              (*(unsigned char volatile xdata *)0xfed0)
#define     PWMA_PSCRL              (*(unsigned char volatile xdata *)0xfed1)
#define     PWMA_ARR                (*(unsigned  int volatile xdata *)0xfed2)
#define     PWMA_ARRH               (*(unsigned char volatile xdata *)0xfed2)
#define     PWMA_ARRL               (*(unsigned char volatile xdata *)0xfed3)
#define     PWMA_RCR                (*(unsigned char volatile xdata *)0xfed4)
#define     PWMA_CCR1               (*(unsigned  int volatile xdata *)0xfed5)
#define     PWMA_CCR1H              (*(unsigned char volatile xdata *)0xfed5)
#define     PWMA_CCR1L              (*(unsigned char volatile xdata *)0xfed6)
#define     PWMA_CCR2               (*(unsigned  int volatile xdata *)0xfed7)
#define     PWMA_CCR2H              (*(unsigned char volatile xdata *)0xfed7)
#define     PWMA_CCR2L              (*(unsigned char volatile xdata *)0xfed8)
#define     PWMA_CCR3               (*(unsigned  int volatile xdata *)0xfed9)
#define     PWMA_CCR3H              (*(unsigned char volatile xdata *)0xfed9)
#define     PWMA_CCR3L              (*(unsigned char volatile xdata *)0xfeda)
#define     PWMA_CCR4               (*(unsigned  int volatile xdata *)0xfedb)
#define     PWMA_CCR4H              (*(unsigned char volatile xdata *)0xfedb)
#define     PWMA_CCR4L              (*(unsigned char volatile xdata *)0xfedc)
#define     PWMA_BKR                (*(unsigned char volatile xdata *)0xfedd)
#define     PWMA_DTR                (*(unsigned char volatile xdata *)0xfede)
#define     PWMA_OISR               (*(unsigned char volatile xdata *)0xfedf)

#define     PWMB_CR1                (*(unsigned char volatile xdata *)0xfee0)
#define     PWMB_CR2                (*(unsigned char volatile xdata *)0xfee1)
#define     PWMB_SMCR               (*(unsigned char volatile xdata *)0xfee2)
#define     PWMB_ETR                (*(unsigned char volatile xdata *)0xfee3)
#define     PWMB_IER                (*(unsigned char volatile xdata *)0xfee4)
#define     PWMB_SR1                (*(unsigned char volatile xdata *)0xfee5)
#define     PWMB_SR2                (*(unsigned char volatile xdata *)0xfee6)
#define     PWMB_EGR                (*(unsigned char volatile xdata *)0xfee7)
#define     PWMB_CCMR1              (*(unsigned char volatile xdata *)0xfee8)
#define     PWMB_CCMR2              (*(unsigned char volatile xdata *)0xfee9)
#define     PWMB_CCMR3              (*(unsigned char volatile xdata *)0xfeea)
#define     PWMB_CCMR4              (*(unsigned char volatile xdata *)0xfeeb)
#define     PWMB_CCER1              (*(unsigned char volatile xdata *)0xfeec)
#define     PWMB_CCER2              (*(unsigned char volatile xdata *)0xfeed)
#define     PWMB_CNTR               (*(unsigned  int volatile xdata *)0xfeee)
#define     PWMB_CNTRH              (*(unsigned char volatile xdata *)0xfeee)
#define     PWMB_CNTRL              (*(unsigned char volatile xdata *)0xfeef)
#define     PWMB_PSCR               (*(unsigned  int volatile xdata *)0xfef0)
#define     PWMB_PSCRH              (*(unsigned char volatile xdata *)0xfef0)
#define     PWMB_PSCRL              (*(unsigned char volatile xdata *)0xfef1)
#define     PWMB_ARR                (*(unsigned  int volatile xdata *)0xfef2)
#define     PWMB_ARRH               (*(unsigned char volatile xdata *)0xfef2)
#define     PWMB_ARRL               (*(unsigned char volatile xdata *)0xfef3)
#define     PWMB_RCR                (*(unsigned char volatile xdata *)0xfef4)
#define     PWMB_CCR5               (*(unsigned  int volatile xdata *)0xfef5)
#define     PWMB_CCR5H              (*(unsigned char volatile xdata *)0xfef5)
#define     PWMB_CCR5L              (*(unsigned char volatile xdata *)0xfef6)
#define     PWMB_CCR6               (*(unsigned  int volatile xdata *)0xfef7)
#define     PWMB_CCR6H              (*(unsigned char volatile xdata *)0xfef7)
#define     PWMB_CCR6L              (*(unsigned char volatile xdata *)0xfef8)
#define     PWMB_CCR7               (*(unsigned  int volatile xdata *)0xfef9)
#define     PWMB_CCR7H              (*(unsigned char volatile xdata *)0xfef9)
#define     PWMB_CCR7L              (*(unsigned char volatile xdata *)0xfefa)
#define     PWMB_CCR8               (*(unsigned  int volatile xdata *)0xfefb)
#define     PWMB_CCR8H              (*(unsigned char volatile xdata *)0xfefb)
#define     PWMB_CCR8L              (*(unsigned char volatile xdata *)0xfefc)
#define     PWMB_BKR                (*(unsigned char volatile xdata *)0xfefd)
#define     PWMB_DTR                (*(unsigned char volatile xdata *)0xfefe)
#define     PWMB_OISR               (*(unsigned char volatile xdata *)0xfeff)

/////////////////////////////////////////////////
//FD00H-FDFFH
/////////////////////////////////////////////////

#define     P0INTE                  (*(unsigned char volatile xdata *)0xfd00)
#define     P1INTE                  (*(unsigned char volatile xdata *)0xfd01)
#define     P2INTE                  (*(unsigned char volatile xdata *)0xfd02)
#define     P3INTE                  (*(unsigned char volatile xdata *)0xfd03)
#define     P4INTE                  (*(unsigned char volatile xdata *)0xfd04)
#define     P5INTE                  (*(unsigned char volatile xdata *)0xfd05)
#define     P6INTE                  (*(unsigned char volatile xdata *)0xfd06)
#define     P7INTE                  (*(unsigned char volatile xdata *)0xfd07)
#define     P0INTF                  (*(unsigned char volatile xdata *)0xfd10)
#define     P1INTF                  (*(unsigned char volatile xdata *)0xfd11)
#define     P2INTF                  (*(unsigned char volatile xdata *)0xfd12)
#define     P3INTF                  (*(unsigned char volatile xdata *)0xfd13)
#define     P4INTF                  (*(unsigned char volatile xdata *)0xfd14)
#define     P5INTF                  (*(unsigned char volatile xdata *)0xfd15)
#define     P6INTF                  (*(unsigned char volatile xdata *)0xfd16)
#define     P7INTF                  (*(unsigned char volatile xdata *)0xfd17)
#define     P0IM0                   (*(unsigned char volatile xdata *)0xfd20)
#define     P1IM0                   (*(unsigned char volatile xdata *)0xfd21)
#define     P2IM0                   (*(unsigned char volatile xdata *)0xfd22)
#define     P3IM0                   (*(unsigned char volatile xdata *)0xfd23)
#define     P4IM0                   (*(unsigned char volatile xdata *)0xfd24)
#define     P5IM0                   (*(unsigned char volatile xdata *)0xfd25)
#define     P6IM0                   (*(unsigned char volatile xdata *)0xfd26)
#define     P7IM0                   (*(unsigned char volatile xdata *)0xfd27)
#define     P0IM1                   (*(unsigned char volatile xdata *)0xfd30)
#define     P1IM1                   (*(unsigned char volatile xdata *)0xfd31)
#define     P2IM1                   (*(unsigned char volatile xdata *)0xfd32)
#define     P3IM1                   (*(unsigned char volatile xdata *)0xfd33)
#define     P4IM1                   (*(unsigned char volatile xdata *)0xfd34)
#define     P5IM1                   (*(unsigned char volatile xdata *)0xfd35)
#define     P6IM1                   (*(unsigned char volatile xdata *)0xfd36)
#define     P7IM1                   (*(unsigned char volatile xdata *)0xfd37)
#define     P0WKUE                  (*(unsigned char volatile xdata *)0xfd40)
#define     P1WKUE                  (*(unsigned char volatile xdata *)0xfd41)
#define     P2WKUE                  (*(unsigned char volatile xdata *)0xfd42)
#define     P3WKUE                  (*(unsigned char volatile xdata *)0xfd43)
#define     P4WKUE                  (*(unsigned char volatile xdata *)0xfd44)
#define     P5WKUE                  (*(unsigned char volatile xdata *)0xfd45)
#define     P6WKUE                  (*(unsigned char volatile xdata *)0xfd46)
#define     P7WKUE                  (*(unsigned char volatile xdata *)0xfd47)

#define     PINIPL                  (*(unsigned char volatile xdata *)0xfd60)
#define     PINIPH                  (*(unsigned char volatile xdata *)0xfd61)
#define     P_SWX1                  (*(unsigned char volatile xdata *)0xfd69)

#define     UR1TOCR                 (*(unsigned char volatile xdata *)0xfd70)
#define     UR1TOSR                 (*(unsigned char volatile xdata *)0xfd71)
#define     UR1TOTH                 (*(unsigned char volatile xdata *)0xfd72)
#define     UR1TOTL                 (*(unsigned char volatile xdata *)0xfd73)
#define     UR2TOCR                 (*(unsigned char volatile xdata *)0xfd74)
#define     UR2TOSR                 (*(unsigned char volatile xdata *)0xfd75)
#define     UR2TOTH                 (*(unsigned char volatile xdata *)0xfd76)
#define     UR2TOTL                 (*(unsigned char volatile xdata *)0xfd77)

#define     SPITOCR                 (*(unsigned char volatile xdata *)0xfd80)
#define     SPITOSR                 (*(unsigned char volatile xdata *)0xfd81)
#define     SPITOTH                 (*(unsigned char volatile xdata *)0xfd82)
#define     SPITOTL                 (*(unsigned char volatile xdata *)0xfd83)
#define     I2CTOCR                 (*(unsigned char volatile xdata *)0xfd84)
#define     I2CTOSR                 (*(unsigned char volatile xdata *)0xfd85)
#define     I2CTOTH                 (*(unsigned char volatile xdata *)0xfd86)
#define     I2CTOTL                 (*(unsigned char volatile xdata *)0xfd87)

#define     UR1TOTE                 (*(unsigned char volatile xdata *)0xfd88)
#define     UR2TOTE                 (*(unsigned char volatile xdata *)0xfd89)
#define     SPITOTE                 (*(unsigned char volatile xdata *)0xfd8c)
#define     I2CTOTE                 (*(unsigned char volatile xdata *)0xfd8d)

#define     CRECR                   (*(unsigned char volatile xdata *)0xfda8)
#define     CRECNTH                 (*(unsigned char volatile xdata *)0xfda9)
#define     CRECNTL                 (*(unsigned char volatile xdata *)0xfdaa)
#define     CRERES                  (*(unsigned char volatile xdata *)0xfdab)

#define     USARTCR2                (*(unsigned char volatile xdata *)0xfdc1)
#define     USART2CR2               (*(unsigned char volatile xdata *)0xfdc9)

#define     CHIPID                  ( (unsigned char volatile xdata *)0xfde0)

#define     CHIPID0                 (*(unsigned char volatile xdata *)0xfde0)
#define     CHIPID1                 (*(unsigned char volatile xdata *)0xfde1)
#define     CHIPID2                 (*(unsigned char volatile xdata *)0xfde2)
#define     CHIPID3                 (*(unsigned char volatile xdata *)0xfde3)
#define     CHIPID4                 (*(unsigned char volatile xdata *)0xfde4)
#define     CHIPID5                 (*(unsigned char volatile xdata *)0xfde5)
#define     CHIPID6                 (*(unsigned char volatile xdata *)0xfde6)
#define     CHIPID7                 (*(unsigned char volatile xdata *)0xfde7)
#define     CHIPID8                 (*(unsigned char volatile xdata *)0xfde8)
#define     CHIPID9                 (*(unsigned char volatile xdata *)0xfde9)
#define     CHIPID10                (*(unsigned char volatile xdata *)0xfdea)
#define     CHIPID11                (*(unsigned char volatile xdata *)0xfdeb)
#define     CHIPID12                (*(unsigned char volatile xdata *)0xfdec)
#define     CHIPID13                (*(unsigned char volatile xdata *)0xfded)
#define     CHIPID14                (*(unsigned char volatile xdata *)0xfdee)
#define     CHIPID15                (*(unsigned char volatile xdata *)0xfdef)
#define     CHIPID16                (*(unsigned char volatile xdata *)0xfdf0)
#define     CHIPID17                (*(unsigned char volatile xdata *)0xfdf1)
#define     CHIPID18                (*(unsigned char volatile xdata *)0xfdf2)
#define     CHIPID19                (*(unsigned char volatile xdata *)0xfdf3)
#define     CHIPID20                (*(unsigned char volatile xdata *)0xfdf4)
#define     CHIPID21                (*(unsigned char volatile xdata *)0xfdf5)
#define     CHIPID22                (*(unsigned char volatile xdata *)0xfdf6)
#define     CHIPID23                (*(unsigned char volatile xdata *)0xfdf7)
#define     CHIPID24                (*(unsigned char volatile xdata *)0xfdf8)
#define     CHIPID25                (*(unsigned char volatile xdata *)0xfdf9)
#define     CHIPID26                (*(unsigned char volatile xdata *)0xfdfa)
#define     CHIPID27                (*(unsigned char volatile xdata *)0xfdfb)
#define     CHIPID28                (*(unsigned char volatile xdata *)0xfdfc)
#define     CHIPID29                (*(unsigned char volatile xdata *)0xfdfd)
#define     CHIPID30                (*(unsigned char volatile xdata *)0xfdfe)
#define     CHIPID31                (*(unsigned char volatile xdata *)0xfdff)

/////////////////////////////////////////////////
//FC00H-FCFFH
/////////////////////////////////////////////////

#define     MD3                     (*(unsigned char volatile xdata *)0xfcf0)
#define     MD2                     (*(unsigned char volatile xdata *)0xfcf1)
#define     MD1                     (*(unsigned char volatile xdata *)0xfcf2)
#define     MD0                     (*(unsigned char volatile xdata *)0xfcf3)
#define     MD5                     (*(unsigned char volatile xdata *)0xfcf4)
#define     MD4                     (*(unsigned char volatile xdata *)0xfcf5)
#define     ARCON                   (*(unsigned char volatile xdata *)0xfcf6)
#define     OPCON                   (*(unsigned char volatile xdata *)0xfcf7)

/////////////////////////////////////////////////
//FB00H-FBFFH
/////////////////////////////////////////////////

#define     COMEN                   (*(unsigned char volatile xdata *)0xfb00)
#define     SEGENL                  (*(unsigned char volatile xdata *)0xfb01)
#define     SEGENH                  (*(unsigned char volatile xdata *)0xfb02)
#define     LEDCTRL                 (*(unsigned char volatile xdata *)0xfb03)
#define     LEDCKS                  (*(unsigned char volatile xdata *)0xfb04)
#define     COM0_DA_L               (*(unsigned char volatile xdata *)0xfb10)
#define     COM1_DA_L               (*(unsigned char volatile xdata *)0xfb11)
#define     COM2_DA_L               (*(unsigned char volatile xdata *)0xfb12)
#define     COM3_DA_L               (*(unsigned char volatile xdata *)0xfb13)
#define     COM4_DA_L               (*(unsigned char volatile xdata *)0xfb14)
#define     COM5_DA_L               (*(unsigned char volatile xdata *)0xfb15)
#define     COM6_DA_L               (*(unsigned char volatile xdata *)0xfb16)
#define     COM7_DA_L               (*(unsigned char volatile xdata *)0xfb17)
#define     COM0_DA_H               (*(unsigned char volatile xdata *)0xfb18)
#define     COM1_DA_H               (*(unsigned char volatile xdata *)0xfb19)
#define     COM2_DA_H               (*(unsigned char volatile xdata *)0xfb1a)
#define     COM3_DA_H               (*(unsigned char volatile xdata *)0xfb1b)
#define     COM4_DA_H               (*(unsigned char volatile xdata *)0xfb1c)
#define     COM5_DA_H               (*(unsigned char volatile xdata *)0xfb1d)
#define     COM6_DA_H               (*(unsigned char volatile xdata *)0xfb1e)
#define     COM7_DA_H               (*(unsigned char volatile xdata *)0xfb1f)
#define     COM0_DC_L               (*(unsigned char volatile xdata *)0xfb20)
#define     COM1_DC_L               (*(unsigned char volatile xdata *)0xfb21)
#define     COM2_DC_L               (*(unsigned char volatile xdata *)0xfb22)
#define     COM3_DC_L               (*(unsigned char volatile xdata *)0xfb23)
#define     COM4_DC_L               (*(unsigned char volatile xdata *)0xfb24)
#define     COM5_DC_L               (*(unsigned char volatile xdata *)0xfb25)
#define     COM6_DC_L               (*(unsigned char volatile xdata *)0xfb26)
#define     COM7_DC_L               (*(unsigned char volatile xdata *)0xfb27)
#define     COM0_DC_H               (*(unsigned char volatile xdata *)0xfb28)
#define     COM1_DC_H               (*(unsigned char volatile xdata *)0xfb29)
#define     COM2_DC_H               (*(unsigned char volatile xdata *)0xfb2a)
#define     COM3_DC_H               (*(unsigned char volatile xdata *)0xfb2b)
#define     COM4_DC_H               (*(unsigned char volatile xdata *)0xfb2c)
#define     COM5_DC_H               (*(unsigned char volatile xdata *)0xfb2d)
#define     COM6_DC_H               (*(unsigned char volatile xdata *)0xfb2e)
#define     COM7_DC_H               (*(unsigned char volatile xdata *)0xfb2f)

#define     CRC16_CR                (*(unsigned char volatile xdata *)0xfb30)
#define     CRC16_DIN               (*(unsigned char volatile xdata *)0xfb31)
#define     CRC16_DOH               (*(unsigned char volatile xdata *)0xfb32)
#define     CRC16_DOL               (*(unsigned char volatile xdata *)0xfb33)

#define     TSCHEN1                 (*(unsigned char volatile xdata *)0xfb40)
#define     TSCHEN2                 (*(unsigned char volatile xdata *)0xfb41)
#define     TSCFG1                  (*(unsigned char volatile xdata *)0xfb42)
#define     TSCFG2                  (*(unsigned char volatile xdata *)0xfb43)
#define     TSWUTC                  (*(unsigned char volatile xdata *)0xfb44)
#define     TSCTRL                  (*(unsigned char volatile xdata *)0xfb45)
#define     TSSTA1                  (*(unsigned char volatile xdata *)0xfb46)
#define     TSSTA2                  (*(unsigned char volatile xdata *)0xfb47)
#define     TSRT                    (*(unsigned char volatile xdata *)0xfb48)
#define     TSDAT                   (*(unsigned int  volatile xdata *)0xfb49)
#define     TSDATH                  (*(unsigned char volatile xdata *)0xfb49)
#define     TSDATL                  (*(unsigned char volatile xdata *)0xfb4A)
#define     TSTH00                  (*(unsigned int  volatile xdata *)0xfb50)
#define     TSTH00H                 (*(unsigned char volatile xdata *)0xfb50)
#define     TSTH00L                 (*(unsigned char volatile xdata *)0xfb51)
#define     TSTH01                  (*(unsigned int  volatile xdata *)0xfb52)
#define     TSTH01H                 (*(unsigned char volatile xdata *)0xfb52)
#define     TSTH01L                 (*(unsigned char volatile xdata *)0xfb53)
#define     TSTH02                  (*(unsigned int  volatile xdata *)0xfb54)
#define     TSTH02H                 (*(unsigned char volatile xdata *)0xfb54)
#define     TSTH02L                 (*(unsigned char volatile xdata *)0xfb55)
#define     TSTH03                  (*(unsigned int  volatile xdata *)0xfb56)
#define     TSTH03H                 (*(unsigned char volatile xdata *)0xfb56)
#define     TSTH03L                 (*(unsigned char volatile xdata *)0xfb57)
#define     TSTH04                  (*(unsigned int  volatile xdata *)0xfb58)
#define     TSTH04H                 (*(unsigned char volatile xdata *)0xfb58)
#define     TSTH04L                 (*(unsigned char volatile xdata *)0xfb59)
#define     TSTH05                  (*(unsigned int  volatile xdata *)0xfb5a)
#define     TSTH05H                 (*(unsigned char volatile xdata *)0xfb5a)
#define     TSTH05L                 (*(unsigned char volatile xdata *)0xfb5b)
#define     TSTH06                  (*(unsigned int  volatile xdata *)0xfb5c)
#define     TSTH06H                 (*(unsigned char volatile xdata *)0xfb5c)
#define     TSTH06L                 (*(unsigned char volatile xdata *)0xfb5d)
#define     TSTH07                  (*(unsigned int  volatile xdata *)0xfb5e)
#define     TSTH07H                 (*(unsigned char volatile xdata *)0xfb5e)
#define     TSTH07L                 (*(unsigned char volatile xdata *)0xfb5f)
#define     TSTH08                  (*(unsigned int  volatile xdata *)0xfb60)
#define     TSTH08H                 (*(unsigned char volatile xdata *)0xfb60)
#define     TSTH08L                 (*(unsigned char volatile xdata *)0xfb61)
#define     TSTH09                  (*(unsigned int  volatile xdata *)0xfb62)
#define     TSTH09H                 (*(unsigned char volatile xdata *)0xfb62)
#define     TSTH09L                 (*(unsigned char volatile xdata *)0xfb63)
#define     TSTH10                  (*(unsigned int  volatile xdata *)0xfb64)
#define     TSTH10H                 (*(unsigned char volatile xdata *)0xfb64)
#define     TSTH10L                 (*(unsigned char volatile xdata *)0xfb65)
#define     TSTH11                  (*(unsigned int  volatile xdata *)0xfb66)
#define     TSTH11H                 (*(unsigned char volatile xdata *)0xfb66)
#define     TSTH11L                 (*(unsigned char volatile xdata *)0xfb67)
#define     TSTH12                  (*(unsigned int  volatile xdata *)0xfb68)
#define     TSTH12H                 (*(unsigned char volatile xdata *)0xfb68)
#define     TSTH12L                 (*(unsigned char volatile xdata *)0xfb69)
#define     TSTH13                  (*(unsigned int  volatile xdata *)0xfb6a)
#define     TSTH13H                 (*(unsigned char volatile xdata *)0xfb6a)
#define     TSTH13L                 (*(unsigned char volatile xdata *)0xfb6b)
#define     TSTH14                  (*(unsigned int  volatile xdata *)0xfb6c)
#define     TSTH14H                 (*(unsigned char volatile xdata *)0xfb6c)
#define     TSTH14L                 (*(unsigned char volatile xdata *)0xfb6d)
#define     TSTH15                  (*(unsigned int  volatile xdata *)0xfb6e)
#define     TSTH15H                 (*(unsigned char volatile xdata *)0xfb6e)
#define     TSTH15L                 (*(unsigned char volatile xdata *)0xfb6f)

#define     LCDCFG                  (*(unsigned char volatile xdata *)0xfb80)
#define     LCDCFG2                 (*(unsigned char volatile xdata *)0xfb81)
#define     DBLEN                   (*(unsigned char volatile xdata *)0xfb82)
#define     COMLENL                 (*(unsigned char volatile xdata *)0xfb83)
#define     COMLENM                 (*(unsigned char volatile xdata *)0xfb84)
#define     COMLENH                 (*(unsigned char volatile xdata *)0xfb85)
#define     BLINKRATE               (*(unsigned char volatile xdata *)0xfb86)
#define     LCDCR                   (*(unsigned char volatile xdata *)0xfb87)
#define     COMON                   (*(unsigned char volatile xdata *)0xfb88)
#define     SEGON1                  (*(unsigned char volatile xdata *)0xfb8a)
#define     SEGON2                  (*(unsigned char volatile xdata *)0xfb8b)
#define     SEGON3                  (*(unsigned char volatile xdata *)0xfb8c)
#define     SEGON4                  (*(unsigned char volatile xdata *)0xfb8d)
#define     SEGON5                  (*(unsigned char volatile xdata *)0xfb8e)
#define     C0SEGV0                 (*(unsigned char volatile xdata *)0xfb90)
#define     C0SEGV1                 (*(unsigned char volatile xdata *)0xfb91)
#define     C0SEGV2                 (*(unsigned char volatile xdata *)0xfb92)
#define     C0SEGV3                 (*(unsigned char volatile xdata *)0xfb93)
#define     C0SEGV4                 (*(unsigned char volatile xdata *)0xfb94)
#define     C1SEGV0                 (*(unsigned char volatile xdata *)0xfb98)
#define     C1SEGV1                 (*(unsigned char volatile xdata *)0xfb99)
#define     C1SEGV2                 (*(unsigned char volatile xdata *)0xfb9a)
#define     C1SEGV3                 (*(unsigned char volatile xdata *)0xfb9b)
#define     C1SEGV4                 (*(unsigned char volatile xdata *)0xfb9c)
#define     C2SEGV0                 (*(unsigned char volatile xdata *)0xfba0)
#define     C2SEGV1                 (*(unsigned char volatile xdata *)0xfba1)
#define     C2SEGV2                 (*(unsigned char volatile xdata *)0xfba2)
#define     C2SEGV3                 (*(unsigned char volatile xdata *)0xfba3)
#define     C2SEGV4                 (*(unsigned char volatile xdata *)0xfba4)
#define     C3SEGV0                 (*(unsigned char volatile xdata *)0xfba8)
#define     C3SEGV1                 (*(unsigned char volatile xdata *)0xfba9)
#define     C3SEGV2                 (*(unsigned char volatile xdata *)0xfbaa)
#define     C3SEGV3                 (*(unsigned char volatile xdata *)0xfbab)
#define     C3SEGV4                 (*(unsigned char volatile xdata *)0xfbac)

#define     HSPWMA_CFG              (*(unsigned char volatile xdata *)0xfbf0)
#define     HSPWMA_ADR              (*(unsigned char volatile xdata *)0xfbf1)
#define     HSPWMA_DAT              (*(unsigned char volatile xdata *)0xfbf2)

#define     HSPWMB_CFG              (*(unsigned char volatile xdata *)0xfbf4)
#define     HSPWMB_ADR              (*(unsigned char volatile xdata *)0xfbf5)
#define     HSPWMB_DAT              (*(unsigned char volatile xdata *)0xfbf6)

/////////////////////////////////////////////////
//FA00H-FAFFH
/////////////////////////////////////////////////

#define     DMA_M2M_CFG             (*(unsigned char volatile xdata *)0xfa00)
#define     DMA_M2M_CR              (*(unsigned char volatile xdata *)0xfa01)
#define     DMA_M2M_STA             (*(unsigned char volatile xdata *)0xfa02)
#define     DMA_M2M_AMT             (*(unsigned char volatile xdata *)0xfa03)
#define     DMA_M2M_DONE            (*(unsigned char volatile xdata *)0xfa04)
#define     DMA_M2M_TXAH            (*(unsigned char volatile xdata *)0xfa05)
#define     DMA_M2M_TXAL            (*(unsigned char volatile xdata *)0xfa06)
#define     DMA_M2M_RXAH            (*(unsigned char volatile xdata *)0xfa07)
#define     DMA_M2M_RXAL            (*(unsigned char volatile xdata *)0xfa08)

#define     DMA_ADC_CFG             (*(unsigned char volatile xdata *)0xfa10)
#define     DMA_ADC_CR              (*(unsigned char volatile xdata *)0xfa11)
#define     DMA_ADC_STA             (*(unsigned char volatile xdata *)0xfa12)
#define     DMA_ADC_AMT             (*(unsigned char volatile xdata *)0xfa13)
#define     DMA_ADC_DONE            (*(unsigned char volatile xdata *)0xfa24)
#define     DMA_ADC_RXAH            (*(unsigned char volatile xdata *)0xfa17)
#define     DMA_ADC_RXAL            (*(unsigned char volatile xdata *)0xfa18)
#define     DMA_ADC_CFG2            (*(unsigned char volatile xdata *)0xfa19)
#define     DMA_ADC_CHSW0           (*(unsigned char volatile xdata *)0xfa1a)
#define     DMA_ADC_CHSW1           (*(unsigned char volatile xdata *)0xfa1b)
#define     DMA_ADC_ITVH            (*(unsigned char volatile xdata *)0xfa1e)
#define     DMA_ADC_ITVL            (*(unsigned char volatile xdata *)0xfa1f)

#define     DMA_SPI_CFG             (*(unsigned char volatile xdata *)0xfa20)
#define     DMA_SPI_CR              (*(unsigned char volatile xdata *)0xfa21)
#define     DMA_SPI_STA             (*(unsigned char volatile xdata *)0xfa22)
#define     DMA_SPI_AMT             (*(unsigned char volatile xdata *)0xfa23)
#define     DMA_SPI_DONE            (*(unsigned char volatile xdata *)0xfa24)
#define     DMA_SPI_TXAH            (*(unsigned char volatile xdata *)0xfa25)
#define     DMA_SPI_TXAL            (*(unsigned char volatile xdata *)0xfa26)
#define     DMA_SPI_RXAH            (*(unsigned char volatile xdata *)0xfa27)
#define     DMA_SPI_RXAL            (*(unsigned char volatile xdata *)0xfa28)
#define     DMA_SPI_CFG2            (*(unsigned char volatile xdata *)0xfa29)
#define     DMA_SPI_ITVH            (*(unsigned char volatile xdata *)0xfa2e)
#define     DMA_SPI_ITVL            (*(unsigned char volatile xdata *)0xfa2f)

#define     DMA_UR1T_CFG            (*(unsigned char volatile xdata *)0xfa30)
#define     DMA_UR1T_CR             (*(unsigned char volatile xdata *)0xfa31)
#define     DMA_UR1T_STA            (*(unsigned char volatile xdata *)0xfa32)
#define     DMA_UR1T_AMT            (*(unsigned char volatile xdata *)0xfa33)
#define     DMA_UR1T_DONE           (*(unsigned char volatile xdata *)0xfa34)
#define     DMA_UR1T_TXAH           (*(unsigned char volatile xdata *)0xfa35)
#define     DMA_UR1T_TXAL           (*(unsigned char volatile xdata *)0xfa36)
#define     DMA_UR1R_CFG            (*(unsigned char volatile xdata *)0xfa38)
#define     DMA_UR1R_CR             (*(unsigned char volatile xdata *)0xfa39)
#define     DMA_UR1R_STA            (*(unsigned char volatile xdata *)0xfa3a)
#define     DMA_UR1R_AMT            (*(unsigned char volatile xdata *)0xfa3b)
#define     DMA_UR1R_DONE           (*(unsigned char volatile xdata *)0xfa3c)
#define     DMA_UR1R_RXAH           (*(unsigned char volatile xdata *)0xfa3d)
#define     DMA_UR1R_RXAL           (*(unsigned char volatile xdata *)0xfa3e)

#define     DMA_UR2T_CFG            (*(unsigned char volatile xdata *)0xfa40)
#define     DMA_UR2T_CR             (*(unsigned char volatile xdata *)0xfa41)
#define     DMA_UR2T_STA            (*(unsigned char volatile xdata *)0xfa42)
#define     DMA_UR2T_AMT            (*(unsigned char volatile xdata *)0xfa43)
#define     DMA_UR2T_DONE           (*(unsigned char volatile xdata *)0xfa44)
#define     DMA_UR2T_TXAH           (*(unsigned char volatile xdata *)0xfa45)
#define     DMA_UR2T_TXAL           (*(unsigned char volatile xdata *)0xfa46)
#define     DMA_UR2R_CFG            (*(unsigned char volatile xdata *)0xfa48)
#define     DMA_UR2R_CR             (*(unsigned char volatile xdata *)0xfa49)
#define     DMA_UR2R_STA            (*(unsigned char volatile xdata *)0xfa4a)
#define     DMA_UR2R_AMT            (*(unsigned char volatile xdata *)0xfa4b)
#define     DMA_UR2R_DONE           (*(unsigned char volatile xdata *)0xfa4c)
#define     DMA_UR2R_RXAH           (*(unsigned char volatile xdata *)0xfa4d)
#define     DMA_UR2R_RXAL           (*(unsigned char volatile xdata *)0xfa4e)

#define     DMA_UR3T_CFG            (*(unsigned char volatile xdata *)0xfa50)
#define     DMA_UR3T_CR             (*(unsigned char volatile xdata *)0xfa51)
#define     DMA_UR3T_STA            (*(unsigned char volatile xdata *)0xfa52)
#define     DMA_UR3T_AMT            (*(unsigned char volatile xdata *)0xfa53)
#define     DMA_UR3T_DONE           (*(unsigned char volatile xdata *)0xfa54)
#define     DMA_UR3T_TXAH           (*(unsigned char volatile xdata *)0xfa55)
#define     DMA_UR3T_TXAL           (*(unsigned char volatile xdata *)0xfa56)
#define     DMA_UR3R_CFG            (*(unsigned char volatile xdata *)0xfa58)
#define     DMA_UR3R_CR             (*(unsigned char volatile xdata *)0xfa59)
#define     DMA_UR3R_STA            (*(unsigned char volatile xdata *)0xfa5a)
#define     DMA_UR3R_AMT            (*(unsigned char volatile xdata *)0xfa5b)
#define     DMA_UR3R_DONE           (*(unsigned char volatile xdata *)0xfa5c)
#define     DMA_UR3R_RXAH           (*(unsigned char volatile xdata *)0xfa5d)
#define     DMA_UR3R_RXAL           (*(unsigned char volatile xdata *)0xfa5e)

#define     DMA_UR4T_CFG            (*(unsigned char volatile xdata *)0xfa60)
#define     DMA_UR4T_CR             (*(unsigned char volatile xdata *)0xfa61)
#define     DMA_UR4T_STA            (*(unsigned char volatile xdata *)0xfa62)
#define     DMA_UR4T_AMT            (*(unsigned char volatile xdata *)0xfa63)
#define     DMA_UR4T_DONE           (*(unsigned char volatile xdata *)0xfa64)
#define     DMA_UR4T_TXAH           (*(unsigned char volatile xdata *)0xfa65)
#define     DMA_UR4T_TXAL           (*(unsigned char volatile xdata *)0xfa66)
#define     DMA_UR4R_CFG            (*(unsigned char volatile xdata *)0xfa68)
#define     DMA_UR4R_CR             (*(unsigned char volatile xdata *)0xfa69)
#define     DMA_UR4R_STA            (*(unsigned char volatile xdata *)0xfa6a)
#define     DMA_UR4R_AMT            (*(unsigned char volatile xdata *)0xfa6b)
#define     DMA_UR4R_DONE           (*(unsigned char volatile xdata *)0xfa6c)
#define     DMA_UR4R_RXAH           (*(unsigned char volatile xdata *)0xfa6d)
#define     DMA_UR4R_RXAL           (*(unsigned char volatile xdata *)0xfa6e)

#define     DMA_LCM_CFG             (*(unsigned char volatile xdata *)0xfa70)
#define     DMA_LCM_CR              (*(unsigned char volatile xdata *)0xfa71)
#define     DMA_LCM_STA             (*(unsigned char volatile xdata *)0xfa72)
#define     DMA_LCM_AMT             (*(unsigned char volatile xdata *)0xfa73)
#define     DMA_LCM_DONE            (*(unsigned char volatile xdata *)0xfa74)
#define     DMA_LCM_TXAH            (*(unsigned char volatile xdata *)0xfa75)
#define     DMA_LCM_TXAL            (*(unsigned char volatile xdata *)0xfa76)
#define     DMA_LCM_RXAH            (*(unsigned char volatile xdata *)0xfa77)
#define     DMA_LCM_RXAL            (*(unsigned char volatile xdata *)0xfa78)
#define     DMA_LCM_ITVH            (*(unsigned char volatile xdata *)0xfa7e)
#define     DMA_LCM_ITVL            (*(unsigned char volatile xdata *)0xfa7f)

#define     DMA_M2M_AMTH            (*(unsigned char volatile xdata *)0xfa80)
#define     DMA_M2M_DONEH           (*(unsigned char volatile xdata *)0xfa81)
#define     DMA_ADC_AMTH            (*(unsigned char volatile xdata *)0xfa82)
#define     DMA_ADC_DONEH           (*(unsigned char volatile xdata *)0xfa83)
#define     DMA_SPI_AMTH            (*(unsigned char volatile xdata *)0xfa84)
#define     DMA_SPI_DONEH           (*(unsigned char volatile xdata *)0xfa85)
#define     DMA_LCM_AMTH            (*(unsigned char volatile xdata *)0xfa86)
#define     DMA_LCM_DONEH           (*(unsigned char volatile xdata *)0xfa87)
#define     DMA_UR1T_AMTH           (*(unsigned char volatile xdata *)0xfa88)
#define     DMA_UR1T_DONEH          (*(unsigned char volatile xdata *)0xfa89)
#define     DMA_UR1R_AMTH           (*(unsigned char volatile xdata *)0xfa8a)
#define     DMA_UR1R_DONEH          (*(unsigned char volatile xdata *)0xfa8b)
#define     DMA_UR2T_AMTH           (*(unsigned char volatile xdata *)0xfa8c)
#define     DMA_UR2T_DONEH          (*(unsigned char volatile xdata *)0xfa8d)
#define     DMA_UR2R_AMTH           (*(unsigned char volatile xdata *)0xfa8e)
#define     DMA_UR2R_DONEH          (*(unsigned char volatile xdata *)0xfa8f)
#define     DMA_UR3T_AMTH           (*(unsigned char volatile xdata *)0xfa90)
#define     DMA_UR3T_DONEH          (*(unsigned char volatile xdata *)0xfa91)
#define     DMA_UR3R_AMTH           (*(unsigned char volatile xdata *)0xfa92)
#define     DMA_UR3R_DONEH          (*(unsigned char volatile xdata *)0xfa93)
#define     DMA_UR4T_AMTH           (*(unsigned char volatile xdata *)0xfa94)
#define     DMA_UR4T_DONEH          (*(unsigned char volatile xdata *)0xfa95)
#define     DMA_UR4R_AMTH           (*(unsigned char volatile xdata *)0xfa96)
#define     DMA_UR4R_DONEH          (*(unsigned char volatile xdata *)0xfa97)

#define     DMA_I2CT_CFG            (*(unsigned char volatile xdata *)0xfa98)
#define     DMA_I2CT_CR             (*(unsigned char volatile xdata *)0xfa99)
#define     DMA_I2CT_STA            (*(unsigned char volatile xdata *)0xfa9a)
#define     DMA_I2CT_AMT            (*(unsigned char volatile xdata *)0xfa9b)
#define     DMA_I2CT_DONE           (*(unsigned char volatile xdata *)0xfa9c)
#define     DMA_I2CT_TXAH           (*(unsigned char volatile xdata *)0xfa9d)
#define     DMA_I2CT_TXAL           (*(unsigned char volatile xdata *)0xfa9e)
#define     DMA_I2CR_CFG            (*(unsigned char volatile xdata *)0xfaa0)
#define     DMA_I2CR_CR             (*(unsigned char volatile xdata *)0xfaa1)
#define     DMA_I2CR_STA            (*(unsigned char volatile xdata *)0xfaa2)
#define     DMA_I2CR_AMT            (*(unsigned char volatile xdata *)0xfaa3)
#define     DMA_I2CR_DONE           (*(unsigned char volatile xdata *)0xfaa4)
#define     DMA_I2CR_RXAH           (*(unsigned char volatile xdata *)0xfaa5)
#define     DMA_I2CR_RXAL           (*(unsigned char volatile xdata *)0xfaa6)

#define     DMA_I2CT_AMTH           (*(unsigned char volatile xdata *)0xfaa8)
#define     DMA_I2CT_DONEH          (*(unsigned char volatile xdata *)0xfaa9)
#define     DMA_I2CR_AMTH           (*(unsigned char volatile xdata *)0xfaaa)
#define     DMA_I2CR_DONEH          (*(unsigned char volatile xdata *)0xfaab)
#define     DMA_I2C_CR              (*(unsigned char volatile xdata *)0xfaad)
#define     DMA_I2C_ST1             (*(unsigned char volatile xdata *)0xfaae)
#define     DMA_I2C_ST2             (*(unsigned char volatile xdata *)0xfaaf)

#define     DMA_I2C_ITVH            (*(unsigned char volatile xdata *)0xfac4)
#define     DMA_I2C_ITVL            (*(unsigned char volatile xdata *)0xfac5)
#define     DMA_UR1_ITVH            (*(unsigned char volatile xdata *)0xfac8)
#define     DMA_UR1_ITVL            (*(unsigned char volatile xdata *)0xfac9)
#define     DMA_UR2_ITVH            (*(unsigned char volatile xdata *)0xfaca)
#define     DMA_UR2_ITVL            (*(unsigned char volatile xdata *)0xfacb)
#define     DMA_UR3_ITVH            (*(unsigned char volatile xdata *)0xfacc)
#define     DMA_UR3_ITVL            (*(unsigned char volatile xdata *)0xfacd)
#define     DMA_UR4_ITVH            (*(unsigned char volatile xdata *)0xface)
#define     DMA_UR4_ITVL            (*(unsigned char volatile xdata *)0xfacf)

#define     DMA_QSPI_CFG            (*(unsigned char volatile xdata *)0xfad0)
#define     DMA_QSPI_CR             (*(unsigned char volatile xdata *)0xfad1)
#define     DMA_QSPI_STA            (*(unsigned char volatile xdata *)0xfad2)
#define     DMA_QSPI_AMT            (*(unsigned char volatile xdata *)0xfad3)
#define     DMA_QSPI_DONE           (*(unsigned char volatile xdata *)0xfad4)
#define     DMA_QSPI_TXAH           (*(unsigned char volatile xdata *)0xfad5)
#define     DMA_QSPI_TXAL           (*(unsigned char volatile xdata *)0xfad6)
#define     DMA_QSPI_RXAH           (*(unsigned char volatile xdata *)0xfad7)
#define     DMA_QSPI_RXAL           (*(unsigned char volatile xdata *)0xfad8)
#define     DMA_QSPI_AMTH           (*(unsigned char volatile xdata *)0xfadb)
#define     DMA_QSPI_DONEH          (*(unsigned char volatile xdata *)0xfadc)
#define     DMA_QSPI_ITVH           (*(unsigned char volatile xdata *)0xfade)
#define     DMA_QSPI_ITVL           (*(unsigned char volatile xdata *)0xfadf)

#define     DMA_P2P_CR1             (*(unsigned char volatile xdata *)0xfaf0)
#define     DMA_P2P_CR2             (*(unsigned char volatile xdata *)0xfaf1)
#define     DMA_ARB_CFG             (*(unsigned char volatile xdata *)0xfaf8)
#define     DMA_ARB_STA             (*(unsigned char volatile xdata *)0xfaf9)

/////////////////////////////////////////////////
//F900H-F9FFH
/////////////////////////////////////////////////

#define     QSPI_CR1                (*(unsigned char volatile xdata *)0xf900)
#define     QSPI_CR2                (*(unsigned char volatile xdata *)0xf901)
#define     QSPI_CR3                (*(unsigned char volatile xdata *)0xf902)
#define     QSPI_CR4                (*(unsigned char volatile xdata *)0xf903)
#define     QSPI_DCR1               (*(unsigned char volatile xdata *)0xf904)
#define     QSPI_DCR2               (*(unsigned char volatile xdata *)0xf905)
#define     QSPI_SR1                (*(unsigned char volatile xdata *)0xf906)
#define     QSPI_SR2                (*(unsigned char volatile xdata *)0xf907)
#define     QSPI_FCR                (*(unsigned char volatile xdata *)0xf908)
#define     QSPI_HCR1               (*(unsigned char volatile xdata *)0xf909)
#define     QSPI_HCR2               (*(unsigned char volatile xdata *)0xf90a)
#define     QSPI_DLR1               (*(unsigned char volatile xdata *)0xf910)
#define     QSPI_DLR2               (*(unsigned char volatile xdata *)0xf911)
#define     QSPI_CCR1               (*(unsigned char volatile xdata *)0xf914)
#define     QSPI_CCR2               (*(unsigned char volatile xdata *)0xf915)
#define     QSPI_CCR3               (*(unsigned char volatile xdata *)0xf916)
#define     QSPI_CCR4               (*(unsigned char volatile xdata *)0xf917)
#define     QSPI_AR1                (*(unsigned char volatile xdata *)0xf918)
#define     QSPI_AR2                (*(unsigned char volatile xdata *)0xf919)
#define     QSPI_AR3                (*(unsigned char volatile xdata *)0xf91a)
#define     QSPI_AR4                (*(unsigned char volatile xdata *)0xf91b)
#define     QSPI_ABR1               (*(unsigned char volatile xdata *)0xf91c)
#define     QSPI_DR                 (*(unsigned char volatile xdata *)0xf920)
#define     QSPI_PSMKR1             (*(unsigned char volatile xdata *)0xf924)
#define     QSPI_PSMAR1             (*(unsigned char volatile xdata *)0xf928)
#define     QSPI_PIR1               (*(unsigned char volatile xdata *)0xf92c)
#define     QSPI_PIR2               (*(unsigned char volatile xdata *)0xf92d)

#define     PWMA_ENO2               (*(unsigned char volatile xdata *)0xf930)
#define     PWMA_IOAUX2             (*(unsigned char volatile xdata *)0xf931)
#define     PWMA_CR3                (*(unsigned char volatile xdata *)0xf932)
#define     PWMA_SR3                (*(unsigned char volatile xdata *)0xf933)
#define     PWMA_CCER3              (*(unsigned char volatile xdata *)0xf934)
#define     PWMA_CCMR1X             (*(unsigned char volatile xdata *)0xf938)
#define     PWMA_CCMR2X             (*(unsigned char volatile xdata *)0xf939)
#define     PWMA_CCMR3X             (*(unsigned char volatile xdata *)0xf93a)
#define     PWMA_CCMR4X             (*(unsigned char volatile xdata *)0xf93b)
#define     PWMA_CCMR5              (*(unsigned char volatile xdata *)0xf93c)
#define     PWMA_CCMR5X             (*(unsigned char volatile xdata *)0xf93d)
#define     PWMA_CCMR6              (*(unsigned char volatile xdata *)0xf93e)
#define     PWMA_CCMR6X             (*(unsigned char volatile xdata *)0xf93f)
#define     PWMA_CCR5H              (*(unsigned char volatile xdata *)0xf940)
#define     PWMA_CCR5L              (*(unsigned char volatile xdata *)0xf941)
#define     PWMA_CCR5X              (*(unsigned char volatile xdata *)0xf942)
#define     PWMA_CCR6H              (*(unsigned char volatile xdata *)0xf943)
#define     PWMA_CCR6L              (*(unsigned char volatile xdata *)0xf944)
#define     PWMA_DER                (*(unsigned char volatile xdata *)0xf948)
#define     PWMA_DBA                (*(unsigned char volatile xdata *)0xf949)
#define     PWMA_DBL                (*(unsigned char volatile xdata *)0xf94a)
#define     PWMA_DMACR              (*(unsigned char volatile xdata *)0xf94b)

/////////////////////////////////////////////////
//USB Control Regiter
/////////////////////////////////////////////////

#define     USBBASE                 0
#define     FADDR                   (USBBASE + 0)
#define     UPDATE                  0x80
#define     POWER                   (USBBASE + 1)
#define     ISOUD                   0x80
#define     USBRST                  0x08
#define     USBRSU                  0x04
#define     USBSUS                  0x02
#define     ENSUS                   0x01
#define     INTRIN1                 (USBBASE + 2)
#define     EP5INIF                 0x20
#define     EP4INIF                 0x10
#define     EP3INIF                 0x08
#define     EP2INIF                 0x04
#define     EP1INIF                 0x02
#define     EP0IF                   0x01
#define     INTROUT1                (USBBASE + 4)
#define     EP5OUTIF                0x20
#define     EP4OUTIF                0x10
#define     EP3OUTIF                0x08
#define     EP2OUTIF                0x04
#define     EP1OUTIF                0x02
#define     INTRUSB                 (USBBASE + 6)
#define     SOFIF                   0x08
#define     RSTIF                   0x04
#define     RSUIF                   0x02
#define     SUSIF                   0x01
#define     INTRIN1E                (USBBASE + 7)
#define     EP5INIE                 0x20
#define     EP4INIE                 0x10
#define     EP3INIE                 0x08
#define     EP2INIE                 0x04
#define     EP1INIE                 0x02
#define     EP0IE                   0x01
#define     INTROUT1E               (USBBASE + 9)
#define     EP5OUTIE                0x20
#define     EP4OUTIE                0x10
#define     EP3OUTIE                0x08
#define     EP2OUTIE                0x04
#define     EP1OUTIE                0x02
#define     INTRUSBE                (USBBASE + 11)
#define     SOFIE                   0x08
#define     RSTIE                   0x04
#define     RSUIE                   0x02
#define     SUSIE                   0x01
#define     FRAME1                  (USBBASE + 12)
#define     FRAME2                  (USBBASE + 13)
#define     INDEX                   (USBBASE + 14)
#define     INMAXP                  (USBBASE + 16)
#define     CSR0                    (USBBASE + 17)
#define     SSUEND                  0x80
#define     SOPRDY                  0x40
#define     SDSTL                   0x20
#define     SUEND                   0x10
#define     DATEND                  0x08
#define     STSTL                   0x04
#define     IPRDY                   0x02
#define     OPRDY                   0x01
#define     INCSR1                  (USBBASE + 17)
#define     INCLRDT                 0x40
#define     INSTSTL                 0x20
#define     INSDSTL                 0x10
#define     INFLUSH                 0x08
#define     INUNDRUN                0x04
#define     INFIFONE                0x02
#define     INIPRDY                 0x01
#define     INCSR2                  (USBBASE + 18)
#define     INAUTOSET               0x80
#define     INISO                   0x40
#define     INMODEIN                0x20
#define     INMODEOUT               0x00
#define     INENDMA                 0x10
#define     INFCDT                  0x08
#define     OUTMAXP                 (USBBASE + 19)
#define     OUTCSR1                 (USBBASE + 20)
#define     OUTCLRDT                0x80
#define     OUTSTSTL                0x40
#define     OUTSDSTL                0x20
#define     OUTFLUSH                0x10
#define     OUTDATERR               0x08
#define     OUTOVRRUN               0x04
#define     OUTFIFOFUL              0x02
#define     OUTOPRDY                0x01
#define     OUTCSR2                 (USBBASE + 21)
#define     OUTAUTOCLR              0x80
#define     OUTISO                  0x40
#define     OUTENDMA                0x20
#define     OUTDMAMD                0x10
#define     COUNT0                  (USBBASE + 22)
#define     OUTCOUNT1               (USBBASE + 22)
#define     OUTCOUNT2               (USBBASE + 23)
#define     FIFO0                   (USBBASE + 32)
#define     FIFO1                   (USBBASE + 33)
#define     FIFO2                   (USBBASE + 34)
#define     FIFO3                   (USBBASE + 35)
#define     FIFO4                   (USBBASE + 36)
#define     FIFO5                   (USBBASE + 37)
#define     UTRKCTL                 (USBBASE + 48)
#define     UTRKSTS                 (USBBASE + 49)

/////////////////////////////////////////////////
//Interrupt Vector
/////////////////////////////////////////////////

#define     INT0_VECTOR             0       //0003H
#define     TMR0_VECTOR             1       //000BH
#define     INT1_VECTOR             2       //0013H
#define     TMR1_VECTOR             3       //001BH
#define     UART1_VECTOR            4       //0023H
#define     ADC_VECTOR              5       //002BH
#define     LVD_VECTOR              6       //0033H
#define     UART2_VECTOR            8       //0043H
#define     SPI_VECTOR              9       //004BH
#define     INT2_VECTOR             10      //0053H
#define     INT3_VECTOR             11      //005BH
#define     TMR2_VECTOR             12      //0063H
#define     USER_VECTOR             13      //006BH
#define     INT4_VECTOR             16      //0083H
#define     UART3_VECTOR            17      //008BH
#define     UART4_VECTOR            18      //0093H
#define     TMR3_VECTOR             19      //009BH
#define     TMR4_VECTOR             20      //00A3H
#define     CMP_VECTOR              21      //00ABH
#define     I2C_VECTOR              24      //00C3H
#define     USB_VECTOR              25      //00CBH
#define     PWMA_VECTOR             26      //00D3H
#define     PWMB_VECTOR             27      //00DBH

#define     TKSU_VECTOR             35      //011BH
#define     RTC_VECTOR              36      //0123H
#define     P0INT_VECTOR            37      //012BH
#define     P1INT_VECTOR            38      //0133H
#define     P2INT_VECTOR            39      //013BH
#define     P3INT_VECTOR            40      //0143H
#define     P4INT_VECTOR            41      //014BH
#define     P5INT_VECTOR            42      //0153H
#define     P6INT_VECTOR            43      //015BH
#define     P7INT_VECTOR            44      //0163H
#define     DMA_M2M_VECTOR          47      //017BH
#define     DMA_ADC_VECTOR          48      //0183H
#define     DMA_SPI_VECTOR          49      //018BH
#define     DMA_UR1T_VECTOR         50      //0193H
#define     DMA_UR1R_VECTOR         51      //019BH
#define     DMA_UR2T_VECTOR         52      //01A3H
#define     DMA_UR2R_VECTOR         53      //01ABH
#define     DMA_UR3T_VECTOR         54      //01B3H
#define     DMA_UR3R_VECTOR         55      //01BBH
#define     DMA_UR4T_VECTOR         56      //01C3H
#define     DMA_UR4R_VECTOR         57      //01CBH
#define     DMA_LCM_VECTOR          58      //01D3H
#define     LCM_VECTOR              59      //01DBH
#define     DMA_I2CT_VECTOR         60      //01E3H
#define     DMA_I2CR_VECTOR         61      //01EBH
#define     DMA_QSPI_VECTOR         65      //020BH
#define     QSPI_VECTOR             66      //0213H
#define     TMR11_VECTOR            67      //021BH

/////////////////////////////////////////////////

#define	EAXSFR()		P_SW2 |= 0x80		/* MOVX A,@DPTR/MOVX @DPTR,A指令的操作对象为扩展SFR(XSFR) */
#define	EAXRAM()		P_SW2 &= ~0x80		/* MOVX A,@DPTR/MOVX @DPTR,A指令的操作对象为扩展RAM(XRAM) */

/////////////////////////////////////////////////
#define NOP1()  _nop_()
#define NOP2() do { NOP1(); NOP1(); } while (0)
#define NOP3() do { NOP2(); NOP1(); } while (0)
#define NOP4() do { NOP3(); NOP1(); } while (0)
#define NOP5() do { NOP4(); NOP1(); } while (0)
#define NOP6() do { NOP5(); NOP1(); } while (0)
#define NOP7() do { NOP6(); NOP1(); } while (0)
#define NOP8() do { NOP7(); NOP1(); } while (0)
#define NOP9() do { NOP8(); NOP1(); } while (0)
#define NOP10() do { NOP9(); NOP1(); } while (0)
#define NOP11() do { NOP10(); NOP1(); } while (0)
#define NOP12() do { NOP11(); NOP1(); } while (0)
#define NOP13() do { NOP12(); NOP1(); } while (0)
#define NOP14() do { NOP13(); NOP1(); } while (0)
#define NOP15() do { NOP14(); NOP1(); } while (0)
#define NOP16() do { NOP15(); NOP1(); } while (0)
#define NOP17() do { NOP16(); NOP1(); } while (0)
#define NOP18() do { NOP17(); NOP1(); } while (0)
#define NOP19() do { NOP18(); NOP1(); } while (0)
#define NOP20() do { NOP19(); NOP1(); } while (0)
#define NOP21() do { NOP20(); NOP1(); } while (0)
#define NOP22() do { NOP21(); NOP1(); } while (0)
#define NOP23() do { NOP22(); NOP1(); } while (0)
#define NOP24() do { NOP23(); NOP1(); } while (0)
#define NOP25() do { NOP24(); NOP1(); } while (0)
#define NOP26() do { NOP25(); NOP1(); } while (0)
#define NOP27() do { NOP26(); NOP1(); } while (0)
#define NOP28() do { NOP27(); NOP1(); } while (0)
#define NOP29() do { NOP28(); NOP1(); } while (0)
#define NOP30() do { NOP29(); NOP1(); } while (0)
#define NOP31() do { NOP30(); NOP1(); } while (0)
#define NOP32() do { NOP31(); NOP1(); } while (0)
#define NOP33() do { NOP32(); NOP1(); } while (0)
#define NOP34() do { NOP33(); NOP1(); } while (0)
#define NOP35() do { NOP34(); NOP1(); } while (0)
#define NOP36() do { NOP35(); NOP1(); } while (0)
#define NOP37() do { NOP36(); NOP1(); } while (0)
#define NOP38() do { NOP37(); NOP1(); } while (0)
#define NOP39() do { NOP38(); NOP1(); } while (0)
#define NOP40() do { NOP39(); NOP1(); } while (0)
#define NOP(N)  NOP##N()


/////////////////////////////////////////////////


/////////////////////////////////////////////////
// TwenBlock/legacy STC8 compatibility aliases
/////////////////////////////////////////////////

/* Traditional interrupt-vector names used by older libraries. */
#define IE0_VECTOR INT0_VECTOR
#define TF0_VECTOR TMR0_VECTOR
#define IE1_VECTOR INT1_VECTOR
#define TF1_VECTOR TMR1_VECTOR
#define SI0_VECTOR UART1_VECTOR

/* Register bit masks exposed by the former SDCC header. */
#define IDL 0x01
#define PD 0x02
#define GF0 0x04
#define GF1 0x08
#define SMOD 0x80
#define T0_M0 0x01
#define T0_M1 0x02
#define T0_CT 0x04
#define T0_GATE 0x08
#define T1_M0 0x10
#define T1_M1 0x20
#define T1_CT 0x40
#define T1_GATE 0x80
#define T0_MASK 0x0F
#define T1_MASK 0xF0

/* The old SDCC header used CKSEL for the register now named CLKSEL. */
#define CKSEL CLKSEL

/* Preserve the old PWM-B channel names at their historical addresses. */
#define PWMB_CCR1 PWM2_CCR1
#define PWMB_CCR1H PWM2_CCR1H
#define PWMB_CCR1L PWM2_CCR1L
#define PWMB_CCR2 PWM2_CCR2
#define PWMB_CCR2H PWM2_CCR2H
#define PWMB_CCR2L PWM2_CCR2L
#define PWMB_CCR3 PWM2_CCR3
#define PWMB_CCR3H PWM2_CCR3H
#define PWMB_CCR3L PWM2_CCR3L
#define PWMB_CCR4 PWM2_CCR4
#define PWMB_CCR4H PWM2_CCR4H
#define PWMB_CCR4L PWM2_CCR4L

/* USB request constants retained from the former TwenBlock header. */
#define EPIDLE 0
#define EPSTATUS 1
#define EPDATAIN 2
#define EPDATAOUT 3
#define EPSTALL -1
#define GET_STATUS 0x00
#define CLEAR_FEATURE 0x01
#define SET_FEATURE 0x03
#define SET_ADDRESS 0x05
#define GET_DESCRIPTOR 0x06
#define SET_DESCRIPTOR 0x07
#define GET_CONFIG 0x08
#define SET_CONFIG 0x09
#define GET_INTERFACE 0x0A
#define SET_INTERFACE 0x0B
#define SYNCH_FRAME 0x0C
#define GET_REPORT 0x01
#define GET_IDLE 0x02
#define GET_PROTOCOL 0x03
#define SET_REPORT 0x09
#define SET_IDLE 0x0A
#define SET_PROTOCOL 0x0B
#define DESC_DEVICE 0x01
#define DESC_CONFIG 0x02
#define DESC_STRING 0x03
#define DESC_HIDREPORT 0x22
#define STANDARD_REQUEST 0x00
#define CLASS_REQUEST 0x20
#define VENDOR_REQUEST 0x40
#define REQUEST_MASK 0x60
#define USB_REQ_RECIP_MASK 0x1F
#define USB_REQ_RECIP_DEVICE 0x00
#define USB_REQ_RECIP_INTERF 0x01
#define USB_REQ_RECIP_ENDP 0x02

/* Compatibility with the former header's legacy PCA priority bit. */
SBIT(PPCA, 0xB8, 7);

#endif /* _STC8HX_H_ */
