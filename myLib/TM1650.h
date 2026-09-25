/*************  技术支持与购买说明    **************
产品主页：http://twen51.com/
淘宝搜索：天问51，可购买基础版、带彩屏标准版、旗舰版
技术支持QQ群一：1138055784
******************************************/
#ifndef __TM1650_H_
#define __TM1650_H_

#if defined (_C51)
#include "C51_softiic.h"
#elif defined (_STC12)
#include "STC12_softiic.h"
#elif defined (_STC15)
#include "STC15_softiic.h"
#else
#include "softiic.h"
#endif

//                 0    1     2   3    4    5     6   7    8    9    A    B    C    D    E    F
uint8 tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

void TM1650_Init();	//初始化
void TM1650_clear();	//清除
void TM1650_displayNum(uint16 num);	//显示数字
void TM1650_displayFloat(float f);	//显示小数
void TM1650_displayBit(uint8 num, uint8 _bit,uint8 point);	//在bit位显示数num;point为1，小数点亮,否则为灭

//========================================================================
// 描述: TM1650写函数
// 参数: add:地址;dat:数据.
// 返回: none.
//========================================================================
void TM1650_write_data(uint8 add,uint8 dat)
{
    softiic_start();    //开始信号
    softiic_send_byte(add);//发送器件写地址
    softiic_wait_ack();
    softiic_send_byte(dat);                //发送寄存器地址
    softiic_wait_ack();
    softiic_stop();   //发送停止命令
}

//========================================================================
// 描述: TM1650初始化
// 参数: none.
// 返回: none.
//========================================================================
void TM1650_Init()
{
    softiic_init();
    TM1650_write_data(0x48,0x31); // 开显示、8段显示方式、3级亮度
}

//========================================================================
// 描述: TM1650清除
// 参数: none.
// 返回: none.
//========================================================================
void TM1650_clear()
{
    uint8 i;
	for(i=0; i<4;i++)
	{
		TM1650_write_data(0x68+i*2,0);  
	}
}

//========================================================================
// 描述: TM1650显示数字
// 参数: none.
// 返回: none.
//========================================================================
void TM1650_displayNum(uint16 num)
{
	uint8 a,b,c,d,_bit;
	_bit = 0;
	d = num%10;
	if(d != 0)_bit = 1;
	c = num%100/10;
	if(c != 0)_bit = 2;
	b = num%1000/100;
	if(b != 0)_bit = 3;
	a = num/1000;
	if(a != 0)_bit = 4;
	switch(_bit)
	{
		case 4:
			TM1650_write_data(0x68,tab[a]);
		case 3:
			TM1650_write_data(0x68+2,tab[b]);
		case 2:
			TM1650_write_data(0x68+4,tab[c]);
		case 1:
			TM1650_write_data(0x68+6,tab[d]);
		break;
	}
}

//========================================================================
// 描述: TM1650显示小数
// 参数: none.
// 返回: none.
//========================================================================
void TM1650_displayFloat(float f)
{
	uint8 a[4];
	uint8 b[4];
	uint8 int_bit,float_bit;
	uint16 int_part,float_part;
	uint8 i;

	TM1650_clear();

	if(f == 0)
	{
		TM1650_write_data(0x68+6,tab[0]);
	}
	else
	{
		if(f > 10000){TM1650_clear();return;}
		if((f*10000) < 1){TM1650_clear();return;}
		int_part = (uint16)f/1;
		if(int_part < 1)int_bit = 0;
		else
		{
			a[3] = int_part%10;if(a[3] != 0)int_bit = 1;//最低位
			a[2] = int_part%100/10;if(a[2] != 0)int_bit = 2;
			a[1] = int_part%1000/100;if(a[1] != 0)int_bit = 3;
			a[0] = int_part/1000;if(a[0] != 0)int_bit = 4;
		}

		float_part = (uint32)(f*10000)%10000;
		b[0] = float_part/1000;if(b[0] != 0)float_bit = 1;
		b[1] = float_part%1000/100;if(b[1] != 0)float_bit = 2;
		b[2] = float_part%100/10;if(b[2] != 0)float_bit = 3;
		b[3] = float_part%10;if(b[3] != 0)float_bit = 4;
		
		if(int_bit == 4)	//4位全为整数位
		{
			TM1650_displayNum(int_part);
//			TM1650_write_data(0x68+6,tab[a[3]]|0X80);
		}
		if(int_bit == 3)	//整数位为3位
		{
			if(b[0] == 0)
			{
				TM1650_displayNum(int_part);
//				TM1650_write_data(0x68+6,tab[a[3]]|0X80);
			}
			else
			{
				TM1650_write_data(0x68,tab[a[1]]);
				TM1650_write_data(0x68+2,tab[a[2]]);
				TM1650_write_data(0x68+4,tab[a[3]]|0X80);
				TM1650_write_data(0x68+6,tab[b[0]]);
			}
		}
		if(int_bit == 2)//整数有2位
		{
			if(b[1] == 0)
			{
				if(b[0] == 0)
				{
					TM1650_displayNum(int_part);
//					Write_DATA(0x68+6,tab[a[3]]|0X80);
				}
				else
				{
					TM1650_write_data(0x68+2,tab[int_part/10]);
					TM1650_write_data(0x68+4,tab[int_part%10]|0X80);
					TM1650_write_data(0x68+6,tab[b[0]]);
				}
			}
			else//小数点后第二位不为零
			{
				TM1650_write_data(0x68,tab[a[2]]);
				TM1650_write_data(0x68+2,tab[a[3]]|0X80);
				TM1650_write_data(0x68+4,tab[b[0]]);
				TM1650_write_data(0x68+6,tab[b[1]]);
			}
		}
		if(int_bit == 1)//整数只有1位
		{
			if(b[2] == 0)
			{
				if(b[1] == 0)
				{
					if(b[0] == 0)
					{
						TM1650_write_data(0x68+6,tab[int_part]);
//						Write_DATA(0x68+6,tab[int_part]|0X80);
					}
					else
					{
						TM1650_write_data(0x68+4,tab[int_part]|0X80);
						TM1650_write_data(0x68+6,tab[b[0]]);
					}
				}
				else
				{
					TM1650_write_data(0x68+2,tab[int_part]|0X80);
					TM1650_write_data(0x68+4,tab[b[0]]);
					TM1650_write_data(0x68+6,tab[b[1]]);
				}
			}
			else//小数点第三位不为零
			{
				TM1650_write_data(0x68,tab[a[3]]|0X80);
				TM1650_write_data(0x68+2,tab[b[0]]);
				TM1650_write_data(0x68+4,tab[b[1]]);
				TM1650_write_data(0x68+6,tab[b[2]]);
			}
		}
		if(int_bit == 0)
		{
			if(float_bit > 3)float_bit=3;
			for(i=0; i<float_bit; i++)
				TM1650_write_data(0x68+6-i*2, tab[b[(float_bit-1)-i]]);
			TM1650_write_data(0x68+6-2*float_bit, tab[0]|0x80);
		}
	}
}

//========================================================================
// 描述: TM1650在指定位显示
// 参数: num:显示的数字;bit:指定位;point:1,小数点亮，0,小数点灭.
// 返回: none.
//========================================================================
void TM1650_displayBit(uint8 num, uint8 _bit,uint8 point)
{
	if(num > 9)return;
	if(point==0){
        TM1650_write_data(0x68+6-2*(_bit-1),tab[num]&= ~(0X80));
	}else{
        TM1650_write_data(0x68+6-2*(_bit-1),tab[num]|=0x80);
	}
}
#endif