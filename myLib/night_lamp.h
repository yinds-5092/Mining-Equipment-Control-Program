#ifndef __NIGHT_LAMP_H
#define __NIGHT_LAMP_H

#define PWM_DUTY_MAX 1000

#define IR_REC_PIN P3_6
#define IR_REC_PIN_MODE {P3M1|=0x40;P3M0&=~0x40;}//P3_6高阻输入

#define RGB_PIN P1_2//RGB灯的引脚
#define RGB_NUMLEDS 4 //RGB灯的个数
#define RGB_PIN_MODE {P1M1&=~0x04;P1M0|=0x04;}//推挽输出

#include "ir.h"
#include "rgb.h"

//根据实际情况做修改
#ifndef NIGHT_LAMP_KEY_LONG_TIME
#define NIGHT_LAMP_KEY_LONG_TIME 1000//1000ms 长按短按区分阈值
#endif

//按键
uint16 _night_lamp_timeKeyPressed; //按键按下的时间
#define NIGHT_LAMP_KEY_STATUS_PRESSED 0
#define NIGHT_LAMP_KEY_STATUS_RELEASED 1
uint8 _night_lamp_statusCntingKeyTime; //是否开始计算按键时间
#define NIGHT_LAMP_STATUS_CNTING 1
#define NIGHT_LAMP_STATUS_NOT_CNTING 0
#define NIGHT_LAMP_KEY_STATE P3_2
uint8 _night_lamp_key_type = 0;
#define NIGHT_LAMP_NO_PRESSED 0
#define NIGHT_LAMP_SHORT_PRESSED 1
#define NIGHT_LAMP_LONG_PRESSED 2

uint8 night_lamp_ir_rec_flag = 0;
uint8 night_lamp_ir_key = 0;
uint32 night_lamp_count = 0;

//小夜灯初始化
void night_lamp_init()
{
  //定时器0，100微秒@24.000MHz 给红外
  AUXR &= 0x7f;		//定时器时钟12T模式
  TMOD &= 0xf0;		//设置定时器模式
  TL0 = 0x38;			//设定定时初值
  TH0 = 0xff;			//设定定时初值
  
  //定时器2，1000微秒@24.000MHz 给按键
  AUXR &= 0xfb;		//定时器时钟12T模式
  T2L = 0x30;			//设定定时初值
  T2H = 0xf8;			//设定定时初值
  
  ET0 = 1; // 控制定时器中断
  TR0 = 1;// 定时器0开始计时
  AUXR |= 0x10; // 定时器2开始计时
  IE2 |= 0x04; // 控制定时器中断
  EA = 1; // 控制总中断
  
  ir_rx_init();//红外接收初始化
  
  //红外供电引脚打开
  P3M1&=~0x80;P3M0|=0x80;//推挽输出
  P3_7 = 1;
  
  rgb_init();//RGB初始化
  
  //RGB 供电引脚打开
  P3M1&=~0x08;P3M0|=0x08;//推挽输出
  P3_3 = 0;
  
  //按键引脚设置双向IO
  P3M1&=~0x04;P3M0&=~0x04;//双向IO口
}

//按键扫描
void night_lamp_key_scan()
{
  if(NIGHT_LAMP_KEY_STATE==NIGHT_LAMP_KEY_STATUS_PRESSED && _night_lamp_statusCntingKeyTime==NIGHT_LAMP_STATUS_NOT_CNTING )
  {
    _night_lamp_timeKeyPressed=0; //设置键按下后,重新开始计时
    _night_lamp_statusCntingKeyTime=NIGHT_LAMP_STATUS_CNTING;//开始计算按下时间
  }

  if(NIGHT_LAMP_KEY_STATE==NIGHT_LAMP_KEY_STATUS_RELEASED && _night_lamp_statusCntingKeyTime==NIGHT_LAMP_STATUS_CNTING)
  {
    _night_lamp_statusCntingKeyTime=NIGHT_LAMP_STATUS_NOT_CNTING;//停止计时
    
    //短按
    if(_night_lamp_timeKeyPressed<NIGHT_LAMP_KEY_LONG_TIME )
    {   
      _night_lamp_key_type = NIGHT_LAMP_SHORT_PRESSED;
    }
    else//长按
    {
      _night_lamp_key_type = NIGHT_LAMP_LONG_PRESSED;
    }
  }
}

//小夜灯按键检测
void night_lamp_key_handle(void (*short_pressed_callback)(),void (*long_pressed_callback)())
{
	night_lamp_key_scan();
  
	if(_night_lamp_key_type == NIGHT_LAMP_LONG_PRESSED)
	{
		_night_lamp_key_type = NIGHT_LAMP_NO_PRESSED;
		long_pressed_callback();
	}
	else if(_night_lamp_key_type == NIGHT_LAMP_SHORT_PRESSED)
	{
		_night_lamp_key_type = NIGHT_LAMP_NO_PRESSED;
		short_pressed_callback();
	} 
}

// //红外按键
// void night_lamp_ir_handle(void (*ir_callback)())
// {
	// if(ir_rx_available())
	// {
		// ir_callback();
	// }
// }

//RGB全部设置
void night_lamp_rgb_show_all(uint8 r, uint8 g, uint8 b)
{
	rgb_show(0,r,g,b);
	rgb_show(1,r,g,b);
	rgb_show(2,r,g,b);
	rgb_show(3,r,g,b);
}

//中断
void night_lamp_T_IRQ0(void) interrupt 1 using 1{
  ir_rec_callback();//红外接收回调函数
  if(ir_rx_available())
	{
		night_lamp_ir_key = ir_rx_ircode();
		night_lamp_ir_rec_flag = 1;
	}
}

void night_lamp_T_IRQ2(void) interrupt 12 using 1{
  _night_lamp_timeKeyPressed++;//按键计时
  night_lamp_count++;
}

#endif