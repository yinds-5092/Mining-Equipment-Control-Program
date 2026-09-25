/*************  技术支持与购买说明    **************
产品主页：http://twen51.com/
淘宝搜索：天问51，可购买基础版、带彩屏标准版、旗舰版
技术支持QQ群一：1138055784
******************************************/
#ifndef __TM1638_H_
#define __TM1638_H_

#if defined(_C51)
#include <REG52.h>

#elif defined(_STC12)
#include "STC12_delay.h"

#elif defined(_STC15)
#include "STC15_delay.h"

#elif defined(_STC8)
#include "delay.h"

#else
#include "delay.h"
#endif

#ifndef TM1638_STB
#define TM1638_STB P1_6
#endif

#ifndef TM1638_STB_OUT
#define TM1638_STB_OUT \
	{                  \
		P1M1 &= ~0x40; \
		P1M0 |= 0x40;  \
	} //推挽输出
#endif

#ifndef TM1638_SCL
#define TM1638_SCL P1_5
#endif

#ifndef TM1638_SCL_OUT
#define TM1638_SCL_OUT \
	{                  \
		P1M1 &= ~0x20; \
		P1M0 |= 0x20;  \
	} //推挽输出
#endif

#ifndef TM1638_SDA
#define TM1638_SDA P1_4
#endif

#ifndef TM1638_SDA_IN
#define TM1638_SDA_IN  \
	{                  \
		P1M1 |= 0x10;  \
		P1M0 &= ~0x10; \
	} //INPUT高阻输入
#endif

#ifndef TM1638_SDA_OUT
#define TM1638_SDA_OUT \
	{                  \
		P1M1 &= ~0x10; \
		P1M0 |= 0x10;  \
	} //推挽输出
#endif

//                    0    1     2     3      4    5      6    7     8     9     A      B     C    D     E     F     -     .     0.    1.    2.    3.    4.    5.    6.    7.    8.    9. 
uint8 code tab[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x40, 0x80, 0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};

void TM1638_Init();					//初始化
void TM1638_clear();				//清除
void TM1638_displayNum(uint16 num); //显示数字
//void TM1638_displayFloat(float f);	//显示小数
void TM1638_displayBit(uint8 num, uint8 _bit, uint8 point); //在bit位显示数num
//void TM1638_displayTime(uint8 m, uint8 s, uint8 point);		//显示时间

//========================================================================
// 描述: 延时函数（仅本模块内可调用）.
// 参数: 延时的us数（不精确的）.
// 返回: none.
//========================================================================
static void tm1638_delay_us(uint8 time)
{
#if defined(_C51)
	do
	{
		_nop_();
	} while (--time);
#else
	do
	{
		delay1us();
	} while (--time);
#endif
}
//========================================================================
// 描述: TM1638写数据.
// 参数: unsigned char	dat.
// 返回: none.
//========================================================================
void TM1638_Write(unsigned char dat) //写数据函数
{

	unsigned char i;
	for (i = 0; i < 8; i++)
	{
		TM1638_SCL = 0;
		if (dat & 0X01)
			TM1638_SDA = 1;
		else
			TM1638_SDA = 0;
		dat >>= 1;
		TM1638_SCL = 1;
		tm1638_delay_us(5);
	}
}
//========================================================================
// 描述: TM1638读数据.
// 参数: none.
// 返回: unsigned char.
//========================================================================
unsigned char TM1638_Read(void) //读数据函数
{
	unsigned char i;
	unsigned char temp = 0;

//设置为输入
#ifndef _C51
	TM1638_SDA_IN;
#endif

	TM1638_SDA = 1;
	for (i = 0; i < 8; i++)
	{
		temp >>= 1;
		TM1638_SCL = 0;
		tm1638_delay_us(2);
		TM1638_SCL = 1;
		if (TM1638_SDA)
			temp |= 0x80;
		TM1638_SCL = 1;
		//tm1638_delay_us(5);
	}

#ifndef _C51
	TM1638_SDA_OUT;
#endif

	return temp;
}

//========================================================================
// 描述: TM1638发送命令.
// 参数: cmd.
// 返回: none.
//========================================================================
void Write_COM(unsigned char cmd) //发送命令字
{

	//在STB下降沿后由DIO输入的第一个字节作为一条指令，最高B7、B6两位比特位以区别不同的指令：
	// B7B6=01为数据命令设置；=10为显示控制命令设置；=11为地址命令
	TM1638_STB = 0;
	TM1638_Write(cmd);
	TM1638_STB = 1;
}

//========================================================================
// 描述: TM1638初始化.
// 参数: none.
// 返回: none.
//========================================================================
void TM1638_Init(void)
{
	unsigned char i;

#ifndef _C51
	TM1638_STB_OUT;
	TM1638_SCL_OUT;
	TM1638_SDA_OUT;
#endif

	Write_COM(0x8a); //亮度
	Write_COM(0x8a); //亮度
	Write_COM(0x40); //写 数据命令  ；写数据到显示缓冲区，地址自动增加模式
	TM1638_STB = 0;
	TM1638_Write(0xc0); //写地址命令
	for (i = 0; i < 16; i++)
		TM1638_Write(0x00); //16个显示缓冲区全写1
	TM1638_STB = 1;
}

//========================================================================
// 描述: TM1638读按键.
// 参数: none.
// 返回: unsigned char，键码1-16, 没有键按下则返回0.
//========================================================================
unsigned char TM1638_Read_key(void)
{
	unsigned char c[4], i, key_value = 0;
	TM1638_STB = 0;

	TM1638_Write(0x42); //读按键扫描数据指令
	tm1638_delay_us(10);
	for (i = 0; i < 4; i++)
		c[i] = TM1638_Read();
	TM1638_STB = 1; //4个字节数据合成一个字节
	if (c[0] == 0x04)
		key_value = 1;
	if (c[0] == 0x40)
		key_value = 2;
	if (c[1] == 0x04)
		key_value = 3;
	if (c[1] == 0x40)
		key_value = 4;
	if (c[2] == 0x04)
		key_value = 5;
	if (c[2] == 0x40)
		key_value = 6;
	if (c[3] == 0x04)
		key_value = 7;
	if (c[3] == 0x40)
		key_value = 8;
	if (c[0] == 0x02)
		key_value = 9;
	if (c[0] == 0x20)
		key_value = 10;
	if (c[1] == 0x02)
		key_value = 11;
	if (c[1] == 0x20)
		key_value = 12;
	if (c[2] == 0x02)
		key_value = 13;
	if (c[2] == 0x20)
		key_value = 14;
	if (c[3] == 0x02)
		key_value = 15;
	if (c[3] == 0x20)
		key_value = 16;
	return (key_value);
}

//========================================================================
// 描述: TM1638写显示缓冲区函数
// 参数: add:显示缓冲区地址（0-15）; 偶数地址为8段笔画
//		dat:数据.
// 返回: none.
//========================================================================
void TM1638_write_data(uint8 add, uint8 dat)
{
	//共阴极接法的地址add：0、2、4、6、8、10、12、14分别对应数码管的8位
	//共阳极接法的地址dat：8位分别对应数码管的8位，1对应右边第一位，0x80对应最左边位
	Write_COM(0x8a); //亮度
	Write_COM(0x8a); //亮度
	Write_COM(0x44); //写 数据命令  ；写数据到特定显示缓冲区，地址固定模式
	TM1638_STB = 0;
	TM1638_Write(0xc0 | add); //写地址命令，

	TM1638_Write(dat); //显示缓冲区数据

	TM1638_STB = 1;
}

//========================================================================
// 描述: TM1638指定位显示单个数字
// 参数: uint16 num：0-9、A-F
//		 int8 _bit：(1-8).
// 返回: none.
//========================================================================
void TM1638_displayNumAtBit(uint8 _bit, uint16 num)
{
	//共阳极接法，需要转化为笔画，SEG引脚上为数码管的位，DRID引脚上接数码管的段
	//共阴极接法，num转化为需要的笔画，bit（0-7）转化为相关的地址，即对应的Grig，DRID引脚上接数码管的位
	unsigned char a;
	a = tab[num];

	TM1638_write_data(_bit * 2, a);
}

//========================================================================
// 描述: TM1638清除
// 参数: none.
// 返回: none.
//========================================================================
void TM1638_clear()
{
	uint8 i;
	for (i = 0; i < 16; i++)
	{
		TM1638_write_data(i, 0);
	}
}

//========================================================================
// 描述: TM1638显示数字
// 参数: none.
// 返回: none.
//========================================================================
void TM1638_displayNum(uint16 num)
{

	unsigned char i, ch;
	TM1638_clear();

	for (i = 0; i < 8; i++)
	{
		if (num > 0 || i < 1)
		{
			ch = num % 10;
			TM1638_displayNumAtBit(i, ch);
		}
		num = num / 10;
	}
}

//========================================================================
// 描述: 求一个整数数字的位数
// 参数: none.
// 返回: none.
//========================================================================
uint8 NumOfInt(uint16 num)
{

	unsigned char i, temp;
	temp = 1;

	for (i = 0; i < 8; i++)
	{
		num = num / 10;
		if (num > 0)
			temp++;
	}
	return temp;
}

//========================================================================
// 描述: TM1650显示小数
// 参数: 浮点数，精度.
// 返回: none.
//========================================================================
void TM1638_displayFloat(float f, uint8 pointNum)
{
	float a;
	uint8 i,ch;
	uint16 int_part, float_part, intBit;
	

	TM1638_clear();

	int_part = (int)f;
	a = (f-int_part);
	for(i=0;i<pointNum;i++)
		a = a*10;
	float_part = (int) a;

	if( (NumOfInt(int_part)+pointNum)>=8)
	{
		TM1638_displayNumAtBit(0,17);
		TM1638_displayNumAtBit(1,17);
		TM1638_displayNumAtBit(2,17);
	}
	else
	{
		intBit = NumOfInt(int_part);
		for (i = 0; i < intBit; i++)
		{
		
			ch = int_part % 10;
			if(i==0) ch=ch+18;
			TM1638_displayNumAtBit(pointNum+i, ch);
			int_part = int_part / 10;
		}	
		//TM1638_displayNumAtBit(pointNum,17);	
		for (i = 0; i < pointNum; i++)
		{
			
			ch = float_part % 10;
			TM1638_displayNumAtBit(i, ch);
			float_part = float_part / 10;
		}		
	}

}

#endif