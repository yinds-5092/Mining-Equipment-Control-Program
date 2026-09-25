/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef __TWEN_BOARD_H
#define __TWEN_BOARD_H	
#include <STC8HX.h>
#include "hc595.h"
#define RGB_NUMLEDS 6 //RGB灯的个数
#include "rgb.h"
#include "sysclock.h"
#include "delay.h"

void twen_board_init()
{
  uint16 i=0;
  //sysclock_set_xosc(0);//设置系统时钟为外部晶振分频系数0，即24.000MHz
  P_SW2 = 0x80;
  XOSCCR = 0xc0; //启动外部晶振
  while (!(XOSCCR & 1)) //等待时钟稳定
  {
	i++;
    delay1us();
    if(i >= 1000){
        break;
      }
  }
  if(i < 1000){
    CLKDIV = 0; //时钟分频
	CKSEL = 0x01; //选择外部晶振
  }else{
    XOSCCR = 0x00; //关闭外部晶振
    sysclock_set_hir_irc(0);  //系统内部高速时钟
  }
  P_SW2 = 0x00;

  P0M1=0x00;P0M0=0x00;//双向IO口
  P1M1=0x00;P1M0=0x00;//双向IO口
  P2M1=0x00;P2M0=0x00;//双向IO口
  P3M1=0x00;P3M0=0x00;//双向IO口
  P4M1=0x00;P4M0=0x00;//双向IO口
  P5M1=0x00;P5M0=0x00;//双向IO口
  P6M1=0x00;P6M0=0x00;//双向IO口
  P7M1=0x00;P7M0=0x00;//双向IO口
  hc595_init();//HC595初始化
  hc595_disable();//HC595禁止点阵和数码管输出
  rgb_init();//RGB初始化
  delay(10);
  for (i=0;i<6;i++) {
    rgb_show(i,0,0,0);//关闭RGB
  }
  delay(10);
}

#endif		//twen_board.h