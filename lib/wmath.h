/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/
#ifndef _WIRING_MATH_
#define _WIRING_MATH_
#include <STC8HX.h>
#include "stdlib.h"

#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

void randomSeed( uint32 dwSeed )
{
  if ( dwSeed != 0 )
  {
    srand( dwSeed ) ;
  }
}

long random( long howsmall, long howbig )
{
  long diff;
  if (howsmall >= howbig)
  {
    return howsmall;
  }

  diff = howbig - howsmall;

  if ( diff == 0 )
  {
    return 0 ;
  }

  return rand() % diff + howsmall;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif