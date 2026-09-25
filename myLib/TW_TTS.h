#ifndef __TW_TTS_H_
#define __TW_TTS_H_


#include "delay.h"
#include "UART.h"
#include <string.h>

// 状态宏定义
#define TTS_STATE_ERROR   0   // 查询失败（命令错误）
#define TTS_STATE_PLAYING 1   // 播放中（对应芯片返回0x4E）
#define TTS_STATE_IDLE    2   // 空闲（对应芯片返回0x4F）
#define TTS_STATE_INIT    3   // 初始化（对应芯片返回0x4A）
#define TTS_STATE_CHECK   4   // 检查（对应芯片返回0x41）


// static  TTS_PLAY[] = {0xfd, 0x00, 0x0a, 0x01, 0x00, 0xba, 0xc3, 0xba, 0xc3, 0xb4, 0xee, 0xb4, 0xee};
static uint8  TTS_VOLUME_CMD[] = {0xfd, 0x00, 0x06, 0x01, 0x01, 0x5b, 0x76, 0x30, 0x5d};
static uint8  TTS_SPEED_CMD[] = {0xfd, 0x00, 0x06, 0x01, 0x01, 0x5b, 0x73, 0x30, 0x5d};
static uint8  TTS_TONE_CMD[] = {0xfd, 0x00, 0x06, 0x01, 0x01, 0x5b, 0x74, 0x30, 0x5d};
static uint8  TTS_STOP_CMD[] = {0xfd, 0x00, 0x01, 0x02};
static uint8  TTS_PAUSE_CMD[] = {0xfd, 0x00, 0x01, 0x03};
static uint8  TTS_RESUME_CMD[] = {0xfd, 0x00, 0x01, 0x04};
static uint8  TTS_QUERY_CMD[] = {0xFD, 0x00, 0x02, 0x21};

/***
 * 
 * 协议格式
 * 帧头    数据长度     命令字    编码参数     文本
 * 0xfd    0x00 0xLL   0x01      0x00        text(n字节)
 * 
*/

/**
 * @brief 初始化TTS模块
 * @param uart_n 串口号
 * @param rx_pin 接收引脚
 * @param tx_pin 发送引脚
 * @param baud 波特率（默认9600）
 * @param tim_n 定时器号
 */
void TTS_begin(UART_Name uart_n, UART_PIN rx_pin, UART_PIN tx_pin, uint32 baud, UART_TIMN tim_n)
{

    delay(1000);uart_init(uart_n, rx_pin, tx_pin, baud, tim_n);
}

/**
 * @brief 写数据函数
 * 
 * @param uart_n 串口号
 * @param buf 数据
 * @param len 数据长度
 */
void tts_writeData(UART_Name uart_n, uint8 *buf, uint16 len)
{
    uart_putbuff(uart_n, buf, len);
}

/**
 * @brief 播放函数
 * 
 * @param uart_n 串口号
 * @param str 播放内容（字符串）
 */
void TTS_play(UART_Name uart_n, uint8* str)
{
    unsigned  char  Frame_Info[50];
    uint16 str_len = 0;
    uint16 i;
    str_len = strlen((char*)str); 
    
    if (str_len == 0 || str_len > 4094) {
        return;
    }

    Frame_Info[0] = 0xFD ; 			
    Frame_Info[1] = 0x00 ; 			
    Frame_Info[2] = str_len + 2; 		
    Frame_Info[3] = 0x01 ; 			
    Frame_Info[4] = 0x00;

    // // 设置帧头后的数据长度字段
    Frame_Info[1] = (str_len + 2) >> 8;
    Frame_Info[2] = (str_len + 2) & 0xFF;
    // 发送帧头
    tts_writeData(uart_n, Frame_Info, 5);

    for(i = 0; i < str_len; i++) {
        uart_putchar(uart_n, str[i]);
    }
}

/**
 * @brief 设置音量
 * 
 * @param uart_n 串口号
 * @param vol 音量值，范围0-9，默认5
 */
void TTS_volume(UART_Name uart_n, uint8 vol)
{
    uint8 volume_buf[9];
    uint8 i;
    
    if (vol > 9) 
    {
        vol = 9;
    }
    
    // 复制常量数据到临时数组
    for(i = 0; i < 9; i++)
    {
        volume_buf[i] = TTS_VOLUME_CMD[i];
    }
    
    volume_buf[7] = 0x30 + vol;
    tts_writeData(uart_n, volume_buf, 9);
}

/**
 * @brief 设置播放速度
 * 
 * @param uart_n 串口号
 * @param speed 速度值，范围0-9，默认5
 */
void TTS_speed(UART_Name uart_n, uint8 speed)
{
    uint8 speed_buf[9];
    uint8 i;
    
    if (speed > 9) 
    {
        speed = 9;
    }
    
    // 复制常量数据到临时数组
    for(i = 0; i < 9; i++)
    {
        speed_buf[i] = TTS_SPEED_CMD[i];
    }
    
    speed_buf[7] = 0x30 + speed;
    tts_writeData(uart_n, speed_buf, 9);
}

/**
 * @brief 设置语调
 * 
 * @param uart_n 串口号
 * @param tone 语调值，范围0-9，默认5
 */
void TTS_tone(UART_Name uart_n, uint8 tone)
{
    uint8 tone_buf[9];
    uint8 i;
    
    if (tone > 9) 
    {
        tone = 9;
    }
    
    // 复制常量数据到临时数组
    for(i = 0; i < 9; i++)
    {
        tone_buf[i] = TTS_TONE_CMD[i];
    }
    
    tone_buf[7] = 0x30 + tone;
    tts_writeData(uart_n, tone_buf, 9);
}

/**
 * @brief 播报警示音
 * 
 * @param uart_n 串口号
 * @param alert 警示音标号，1-5
 */
void TTS_alert(UART_Name uart_n, uint8 alert)
{
    char str[8];
    
    if (alert < 1) 
    {
        alert = 1;
    }
    else if (alert > 5) 
    {
        alert = 5;
    }
    
    // 直接构造字符串，避免使用字符串常量
    str[0] = 'a';
    str[1] = 'l';
    str[2] = 'e';
    str[3] = 'r';
    str[4] = 't';
    str[5] = '_';
    str[6] = 0x30 + alert;
    str[7] = '\0';
    
    TTS_play(uart_n, str);
}

/**
 * @brief 播报信息提示音
 * 
 * @param uart_n 串口号
 * @param msg 标号，1-5
 */
void TTS_play_msg(UART_Name uart_n, uint8 msg)
{
    char str[10];
    
    if (msg < 1) 
    {
        msg = 1;
    }
    else if (msg > 5) 
    {
        msg = 5;
    }
    
    str[0] = 'm';
    str[1] = 'e';
    str[2] = 's';
    str[3] = 's';
    str[4] = 'a';
    str[5] = 'g';
    str[6] = 'e';
    str[7] = '_';
    str[8] = 0x30 + msg;
    str[9] = '\0';
    
    TTS_play(uart_n, str);
}

/**
 * @brief 播放铃声
 * 
 * @param uart_n 串口号
 * @param ring 标号，1-5
 */
void TTS_play_ring(UART_Name uart_n, uint8 ring)
{
    char str[7];
    
    if (ring < 1) 
    {
        ring = 1;
    }
    else if (ring > 5) 
    {
        ring = 5;
    }
    
    str[0] = 'r';
    str[1] = 'i';
    str[2] = 'n';
    str[3] = 'g';
    str[4] = '_';
    str[5] = 0x30 + ring;
    str[6] = '\0';
    
    TTS_play(uart_n, str);
}

/**
 * @brief 停止播放
 * 
 * @param uart_n 串口号
 */
void TTS_stop(UART_Name uart_n)
{
    uint8 TW_tts_buf[4];
    uint8 i;
    
    // 复制常量数据到临时数组
    for(i = 0; i < 4; i++)
    {
        TW_tts_buf[i] = TTS_STOP_CMD[i];
    }
    
    tts_writeData(uart_n, TW_tts_buf, 4);
}

/**
 * @brief 暂停播放
 * 
 * @param uart_n 串口号
 */
void TTS_pause(UART_Name uart_n)
{
    uint8 TW_tts_buf[4];
    uint8 i;
    
    // 复制常量数据到临时数组
    for(i = 0; i < 4; i++)
    {
        TW_tts_buf[i] = TTS_PAUSE_CMD[i];
    }
    
    tts_writeData(uart_n, TW_tts_buf, 4);
}

/**
 * @brief 继续播放
 * 
 * @param uart_n 串口号
 */
void TTS_resume(UART_Name uart_n)
{
    uint8 TW_tts_buf[4];
    uint8 i;
    
    // 复制常量数据到临时数组
    for(i = 0; i < 4; i++)
    {
        TW_tts_buf[i] = TTS_RESUME_CMD[i];
    }
    
    tts_writeData(uart_n, TW_tts_buf, 4);
}

/**
 * @brief 读取CITW-TTS芯片当前播放状态
 * 
 * @param uart_n 串口号
 * @return uint8：TTS_STATE_ERROR(0)、TTS_STATE_PLAYING(1)、TTS_STATE_IDLE(2)
 */
uint8 TTS_queryState(UART_Name uart_n)
{
    uint8 response = 0;
    uint8 query_buf[4];
    uint8 i;
    
    // 复制常量数据到临时数组
    for(i = 0; i < 4; i++)
    {
        query_buf[i] = TTS_QUERY_CMD[i];
    }
    
    // 发送查询命令
    tts_writeData(uart_n, query_buf, 4);
    
    // 短暂延时等待响应
    delay(10);
    
    // 检查串口接收标志
    switch(uart_n)
    {
        case UART_1:
            if(UART1_GET_RX_FLAG)
            {
                response = SBUF;
                UART1_CLEAR_RX_FLAG;
            }
            break;
        case UART_2:
            if(UART2_GET_RX_FLAG)
            {
                response = S2BUF;
                UART2_CLEAR_RX_FLAG;
            }
            break;
        case UART_3:
            if(UART3_GET_RX_FLAG)
            {
                response = S3BUF;
                UART3_CLEAR_RX_FLAG;
            }
            break;
        case UART_4:
            if(UART4_GET_RX_FLAG)
            {
                response = S4BUF;
                UART4_CLEAR_RX_FLAG;
            }
            break;
    }
    
    if (response == 0x4E)
    {
        return TTS_STATE_PLAYING; // 播放中
    }
    else if (response == 0x4F)
    {
        return TTS_STATE_IDLE;    // 空闲
    }
    else if (response == 0x4A)
    {
        return TTS_STATE_INIT;   // 初始化
    }
    else if (response == 0x41)
    {
        return TTS_STATE_CHECK;   // 检查
    }
    else
    {
        return TTS_STATE_ERROR;   // 查询失败（命令错误）
    }
}

#endif