// CS1237 库实现文件
// 注意：此文件应通过 #include "myLib/cs1237.c" 包含到 main.c 中
// cs1237.h 已在 main.c 中 included，无需重复包含

#include "lib/delay.h"
#include "lib/ntc.h"

// ==================== CS1237驱动函数 ====================

// CS1237引脚初始化
void cs1237_init()
{
    // P5.0 SCLK - 推挽输出
    P5M1 &= ~0x01;    // M1 = 0
    P5M0 |= 0x01;     // M0 = 1
    
    // P5.1 DOUT - 高阻输入
    P5M1 |= 0x02;     // M1 = 1
    P5M0 &= ~0x02;    // M0 = 0
    
    CS1237_SCLK_PIN = 0;
}

// 短延时函数（用于 CS1237 时序）
void cs1237_delay_us()
{
    uint8 i;
    for(i = 0; i < 3; i++);
}

// CS1237读取24位ADC值
// 返回值：24位ADC数据（有符号，需要转换）
int32 cs1237_read_adc()
{
    uint8 i;
    int32 adc_value;
    uint8 bit_val;
    
    adc_value = 0;
    
    // 发送24个时钟脉冲启动转换
    for(i = 0; i < 24; i++)
    {
        CS1237_SCLK_PIN = 1;
        cs1237_delay_us();
        CS1237_SCLK_PIN = 0;
        cs1237_delay_us();
    }
    
    // 等待DOUT变低（转换完成）
    // 实际应用中可添加超时检测
    while(CS1237_DOUT_PIN);
    
    // 读取24位数据（MSB first）
    for(i = 0; i < 24; i++)
    {
        CS1237_SCLK_PIN = 1;
        cs1237_delay_us();
        
        adc_value <<= 1;
        bit_val = CS1237_DOUT_PIN;
        if(bit_val)
        {
            adc_value |= 1;
        }
        
        CS1237_SCLK_PIN = 0;
        cs1237_delay_us();
    }
    
    // 读取第25位（状态位，忽略）
    CS1237_SCLK_PIN = 1;
    cs1237_delay_us();
    CS1237_SCLK_PIN = 0;
    cs1237_delay_us();
    
    // 转换为有符号数（24位补码）
    if(adc_value & 0x800000)
    {
        adc_value = adc_value - 0x1000000;
    }
    
    return adc_value;
}

// ==================== K型热电偶温度计算 ====================

// K型热电偶参数
// 灵敏度约 41μV/°C
// CS1237增益默认128倍
// 参考电压 5V
// LSB = 5V / (2^24 * 128) ≈ 2.328μV

#define K_TYPE_SENSITIVITY 41.0f        // K型热电偶灵敏度 μV/°C
#define CS1237_GAIN 128.0f              // CS1237增益
#define CS1237_VREF 5.0f                // 参考电压 5V
#define CS1237_LSB (CS1237_VREF / (16777216.0f * CS1237_GAIN) * 1000000.0f)  // LSB值 μV

// K型热电偶冷端补偿表（简化版，0-50°C）
// 单位：mV
code float k_type_cold_compensation[] = {
    0.000,  // 0°C
    0.039,  // 1°C
    0.079,  // 2°C
    0.119,  // 3°C
    0.158,  // 4°C
    0.198,  // 5°C
    0.238,  // 6°C
    0.277,  // 7°C
    0.317,  // 8°C
    0.357,  // 9°C
    0.397,  // 10°C
    0.437,  // 11°C
    0.477,  // 12°C
    0.517,  // 13°C
    0.557,  // 14°C
    0.597,  // 15°C
    0.637,  // 16°C
    0.677,  // 17°C
    0.718,  // 18°C
    0.758,  // 19°C
    0.798,  // 20°C
    0.838,  // 21°C
    0.879,  // 22°C
    0.919,  // 23°C
    0.960,  // 24°C
    1.000,  // 25°C
    1.041,  // 26°C
    1.081,  // 27°C
    1.122,  // 28°C
    1.163,  // 29°C
    1.203,  // 30°C
    1.244,  // 31°C
    1.285,  // 32°C
    1.326,  // 33°C
    1.366,  // 34°C
    1.407,  // 35°C
    1.448,  // 36°C
    1.489,  // 37°C
    1.530,  // 38°C
    1.571,  // 39°C
    1.612,  // 40°C
    1.653,  // 41°C
    1.694,  // 42°C
    1.735,  // 43°C
    1.776,  // 44°C
    1.817,  // 45°C
    1.858,  // 46°C
    1.899,  // 47°C
    1.941,  // 48°C
    1.982,  // 49°C
    2.023   // 50°C
};

// 获取冷端补偿电压（mV）
float get_cold_compensation_mv(uint8 temp_c)
{
    uint8 index;
    float compensation;
    
    if(temp_c > 50)
    {
        temp_c = 50;
    }
    
    index = temp_c;
    compensation = k_type_cold_compensation[index];
    
    return compensation;
}

// K型热电偶温度计算（带冷端补偿）
float cs1237_calc_k_type_temp(int32 adc_value, float cold_temp)
{
    float voltage_mv;
    float compensated_mv;
    float temperature;
    float cold_compensation;
    
    // 计算输入电压（mV）
    voltage_mv = (float)adc_value * CS1237_LSB / 1000.0f;
    
    // 获取冷端补偿电压（mV）
    cold_compensation = get_cold_compensation_mv((uint8)cold_temp);
    
    // 补偿后的电压 = 测量电压 + 冷端补偿电压
    compensated_mv = voltage_mv + cold_compensation;
    
    // 计算温度（K型热电偶约41μV/°C = 0.041mV/°C）
    temperature = compensated_mv / 0.041f;
    
    return temperature;
}

// NTC温度读取（使用板载NTC）
float cs1237_read_ntc_temp()
{
    float temp;
    temp = ntc_read_temp();
    return temp;
}
