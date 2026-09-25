#ifndef __CS1237_H__
#define __CS1237_H__

#include <STC8HX.h>

// ==================== CS1237引脚定义 ====================
// 可在包含此头文件前自定义引脚
// 默认：P5.0 -> SCLK, P5.1 -> DOUT
#ifndef CS1237_SCLK_PIN
#define CS1237_SCLK_PIN P5_0
#endif

#ifndef CS1237_DOUT_PIN
#define CS1237_DOUT_PIN P5_1
#endif

// ==================== 函数声明 ====================

// CS1237引脚初始化
void cs1237_init();

// CS1237读取24位ADC值
// 返回值：24位ADC数据（有符号）
int32 cs1237_read_adc();

// K型热电偶温度计算（带冷端补偿）
// 参数：adc_value - ADC原始值，cold_temp - 冷端温度（°C）
// 返回值：热电偶温度（°C）
float cs1237_calc_k_type_temp(int32 adc_value, float cold_temp);

// NTC温度读取（使用板载NTC）
// 返回值：NTC温度（°C）
float cs1237_read_ntc_temp();

#endif
