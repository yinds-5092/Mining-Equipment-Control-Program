/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/

#ifndef __KEYPAD_H
#define __KEYPAD_H

#include <STC8HX.h>
#include "delay.h"

#ifndef     KEYPAD_PORT
#define     KEYPAD_PORT         P7
#endif

#ifndef     KEYPAD_MODE
#define     KEYPAD_MODE       {P7M1=0x00;P7M0=0x00;}  //双向IO口 
#endif

// code uint8 _keypad_tab[4][4]={
//     {7,8,9,10},
//     {4,5,6,11},
//     {1,2,3,12},
//     {14,0,15,13}
//     };

void keypad_init();         //按键初始化
int8 keypad_get_value();    //获取按键值
void io_key_scan();  
//========================================================================
// 描述: 按键初始化.
// 参数: none.
// 返回: none.
//========================================================================
void keypad_init()
{
    KEYPAD_MODE;   //双向IO口
}

//========================================================================
// 描述: 获取按键值.
// 参数: none.
// 返回: -1:无按键按下；0~15:0-9,+,-,x,/,*,#.
//========================================================================
// int8 keypad_get_value()
// {
//     uint8 tempH=0,tempL=0,x=0,y=0;
//     int8 key_value = -1;
//     KEYPAD_PORT=0xF0;
//     if(KEYPAD_PORT != 0xF0)//
//     {
//         delay(20);
//         if(KEYPAD_PORT != 0xF0)//
//         {
//             KEYPAD_PORT=0x0F;
//             delay(20);   
//             tempL = KEYPAD_PORT;

//             KEYPAD_PORT=0xF0;
//             delay(20);
//             tempH = KEYPAD_PORT;

//             switch(tempH)
//             {
//                 case 0xe0:x=0;break;
//                 case 0xd0:x=1;break;
//                 case 0xb0:x=2;break;
//                 case 0x70:x=3;break;
//                 //default:key_value = -1;
//             }

//             switch(tempL)
//             {
//                 case 0x0E:y=3;break;
//                 case 0x0D:y=2;break;
//                 case 0x0B:y=1;break;
//                 case 0x07:y=0;break;
//                // default:key_value = -1;
//             }

//             key_value=_keypad_tab[x][y];
//         }

//     }
//    return key_value;
// }

/*****************************************************
    行列键扫描程序
    使用XY查找4x4键的方法，只能单键，速度快

   Y     P70      P71      P72      P73
          |        |        |        |
X         |        |        |        |
P74 ----  +  ----  9  ----  8  ----  7  ----
          |        |        |        |
P75 ----  -  ----  6  ----  5  ----  4  ----
          |        |        |        |
P76 ----  x  ----  3  ----  2  ----  1  ----
          |        |        |        |
P77 ----  ÷  ----  #  ----  0  ----  * ----
          |        |        |        |
******************************************************/

code uint8 _keypad_tab[16]={
    7,8,9,10,
    4,5,6,11,
    1,2,3,12,
    14,0,15,13
};

static void io_key_delay()
{
    uint8 i;
    i = 60;
    while(--i)  ;
}

xdata  int8  _key_code = -1;    //给用户使用的键码, 1~16有效, 0~15:0-9,+,-,x,/,*,#.
//========================================================================
// 描述: 按键扫描函数(需在中断中50ms调用一次)
// 参数: none.
// 返回: none.
//========================================================================
void io_key_scan()
{
    static uint8  IO_KeyState, IO_KeyState1, IO_KeyHoldCnt;   //行列键盘变量
    static uint8  KeyHoldCnt; //键按下计时

    uint8  j,x=0,y=0;

    j = IO_KeyState1;   //保存上一次状态

    KEYPAD_PORT = 0xf0;  //X低，读Y
    io_key_delay();
    IO_KeyState1 = KEYPAD_PORT & 0xf0;

    KEYPAD_PORT = 0x0f;  //Y低，读X
    io_key_delay();
    IO_KeyState1 |= (KEYPAD_PORT & 0x0f);
    IO_KeyState1 ^= 0xff;   //取反
    
    if(j == IO_KeyState1)   //连续两次读相等
    {
        j = IO_KeyState;
        IO_KeyState = IO_KeyState1;
        if(IO_KeyState != 0)    //有键按下
        {
            F0 = 0;
            if(j == 0)  F0 = 1; //第一次按下
            else if(j == IO_KeyState)
            {
                if(++IO_KeyHoldCnt >= 20)   //1秒后重键
                {
                    IO_KeyHoldCnt = 18;
                    F0 = 1;
                }
            }
            if(F0)
            {
                switch(IO_KeyState >> 4)
                {
                    case 0x01:y=1;break;
                    case 0x02:y=2;break;
                    case 0x04:y=3;break;
                    case 0x08:y=4;break;
                    default:y=0;break;
                }
                switch(IO_KeyState & 0x0f)
                {
                    case 0x01:x=4;break;
                    case 0x02:x=3;break;
                    case 0x04:x=2;break;
                    case 0x08:x=1;break;
                    default:x=0;break;
                }
                if((x != 0) && (y != 0))
                {
                    _key_code = _keypad_tab[(y-1)*4+(x-1)];
                }
            }
        }
        else    IO_KeyHoldCnt = 0;
    }
    KEYPAD_PORT = 0xff;
}

// j = t_key_table[IO_KeyState >> 4];
// if((j != 0) && (t_key_table[IO_KeyState& 0x0f] != 0)) 
//     _key_code = (j - 1) * 4 + t_key_table[IO_KeyState & 0x0f];    //计算键码

//========================================================================
// 描述: 获取按键值.
// 参数: none.
// 返回: -1:无按键按下;其它值，键码
//========================================================================
int8 keypad_get_value()
{
    int8 tem_value;
    tem_value = _key_code;
    if(_key_code >= 0)
    {
        _key_code = -1;
        return tem_value;
    }
    return -1;
}

#endif