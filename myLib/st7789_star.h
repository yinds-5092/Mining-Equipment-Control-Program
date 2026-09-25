/*
 * ST7789 五角星绘制库（支持旋转）
 * 功能：在 ST7789 彩屏上绘制空心和实心五角星，支持旋转角度
 * 作者：zlyrobot
 * 日期：2026-04-20
 * 
 * 使用方法：
 * 1. 在 main.c 中先包含 ST7789.h：#include "myLib/ST7789.h"
 * 2. 再包含此头文件：#include "lib/st7789_star.h"
 * 3. 调用 st7789_fill_star_rotate() 或 st7789_draw_hollow_star_rotate()
 */

#ifndef __ST7789_STAR_H
#define __ST7789_STAR_H

#include <STC8HX.h>
#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

/**
 * @brief 绘制空心五角星（无旋转）
 * @param cx 中心 X 坐标
 * @param cy 中心 Y 坐标
 * @param out_r 外顶点半径
 * @param in_r 内顶点半径（推荐 out_r * 0.382）
 * @param color 颜色值
 */
void st7789_draw_hollow_star(uint16 cx, uint16 cy, uint16 out_r, uint16 in_r, uint16 color);

/**
 * @brief 绘制实心五角星（无旋转）
 * @param cx 中心 X 坐标
 * @param cy 中心 Y 坐标
 * @param out_r 外顶点半径
 * @param in_r 内顶点半径（推荐 out_r * 0.382）
 * @param color 颜色值
 */
void st7789_fill_star(uint16 cx, uint16 cy, uint16 out_r, uint16 in_r, uint16 color);

/**
 * @brief 绘制空心五角星（支持旋转）
 * @param cx 中心 X 坐标
 * @param cy 中心 Y 坐标
 * @param out_r 外顶点半径
 * @param in_r 内顶点半径（推荐 out_r * 0.382）
 * @param color 颜色值
 * @param angle_deg 旋转角度（度，0-360）
 */
void st7789_draw_hollow_star_rotate(uint16 cx, uint16 cy, uint16 out_r, uint16 in_r, uint16 color, uint8 angle_deg);

/**
 * @brief 绘制实心五角星（支持旋转）
 * @param cx 中心 X 坐标
 * @param cy 中心 Y 坐标
 * @param out_r 外顶点半径
 * @param in_r 内顶点半径（推荐 out_r * 0.382）
 * @param color 颜色值
 * @param angle_deg 旋转角度（度，0-360）
 */
void st7789_fill_star_rotate(uint16 cx, uint16 cy, uint16 out_r, uint16 in_r, uint16 color, uint8 angle_deg);

//========================================================================
// 实现代码
//========================================================================

/**
 * @brief 绘制空心五角星（无旋转）
 */
void st7789_draw_hollow_star(uint16 cx, uint16 cy, uint16 out_r, uint16 in_r, uint16 color) {
    st7789_draw_hollow_star_rotate(cx, cy, out_r, in_r, color, 0);
}

/**
 * @brief 绘制实心五角星（无旋转）
 */
void st7789_fill_star(uint16 cx, uint16 cy, uint16 out_r, uint16 in_r, uint16 color) {
    st7789_fill_star_rotate(cx, cy, out_r, in_r, color, 0);
}

/**
 * @brief 绘制空心五角星（支持旋转）
 */
void st7789_draw_hollow_star_rotate(uint16 cx, uint16 cy, uint16 out_r, uint16 in_r, uint16 color, uint8 angle_deg) {
    int16 vx[10], vy[10];
    uint8 i;
    float angle, base_angle;
    float rot_rad = angle_deg * PI / 180.0;  // 旋转角度转弧度
    
    for(i = 0; i < 10; i++) {
        // 基础角度：从 -90 度（顶部）开始，每 36 度一个顶点
        base_angle = -PI/2 + (i * PI / 5);
        
        // 加上旋转角度
        angle = base_angle + rot_rad;
        
        // 偶数索引为外顶点，奇数索引为内顶点
        if(i % 2 == 0) {
            vx[i] = cx + (int16)(out_r * cosf(angle));
            vy[i] = cy + (int16)(out_r * sinf(angle));
        } else {
            vx[i] = cx + (int16)(in_r * cosf(angle));
            vy[i] = cy + (int16)(in_r * sinf(angle));
        }
    }
    
    // 依次连接顶点
    for(i = 0; i < 10; i++) {
        uint8 next = (i + 1) % 10;
        ST7789_draw_line(vx[i], vy[i], vx[next], vy[next], color);
    }
}

/**
 * @brief 绘制实心五角星（支持旋转，扫描线填充）
 */
void st7789_fill_star_rotate(uint16 cx, uint16 cy, uint16 out_r, uint16 in_r, uint16 color, uint8 angle_deg) {
    int16 vx[10], vy[10];
    uint8 i, j;
    float angle, base_angle;
    float rot_rad = angle_deg * PI / 180.0;  // 旋转角度转弧度
    int16 min_y, max_y, y;
    
    // 计算 10 个顶点（带旋转）
    for(i = 0; i < 10; i++) {
        base_angle = -PI/2 + (i * PI / 5);
        angle = base_angle + rot_rad;
        
        if(i % 2 == 0) {
            vx[i] = cx + (int16)(out_r * cosf(angle));
            vy[i] = cy + (int16)(out_r * sinf(angle));
        } else {
            vx[i] = cx + (int16)(in_r * cosf(angle));
            vy[i] = cy + (int16)(in_r * sinf(angle));
        }
    }
    
    // 绘制轮廓
    for(i = 0; i < 10; i++) {
        uint8 next = (i + 1) % 10;
        ST7789_draw_line(vx[i], vy[i], vx[next], vy[next], color);
    }
    
    // 找 Y 范围
    min_y = vy[0];
    max_y = vy[0];
    for(i = 1; i < 10; i++) {
        if(vy[i] < min_y) min_y = vy[i];
        if(vy[i] > max_y) max_y = vy[i];
    }
    
    // 扫描线填充
    for(y = min_y; y <= max_y; y++) {
        int16 x_int[20];
        uint8 cnt = 0;
        
        for(i = 0; i < 10; i++) {
            uint8 next = (i + 1) % 10;
            int16 y1 = vy[i], y2 = vy[next];
            int16 x1 = vx[i], x2 = vx[next];
            
            // 确保 y1 <= y2
            if(y1 > y2) {
                int16 t = y1; y1 = y2; y2 = t;
                t = x1; x1 = x2; x2 = t;
            }
            
            // 检查相交
            if(y >= y1 && y < y2 && y2 != y1) {
                x_int[cnt++] = x1 + (int16)((float)(y - y1) / (float)(y2 - y1) * (x2 - x1));
            }
        }
        
        // 排序（冒泡）
        for(i = 0; i < cnt - 1; i++) {
            for(j = 0; j < cnt - i - 1; j++) {
                if(x_int[j] > x_int[j + 1]) {
                    int16 t = x_int[j];
                    x_int[j] = x_int[j + 1];
                    x_int[j + 1] = t;
                }
            }
        }
        
        // 成对填充
        for(i = 0; i + 1 < cnt; i += 2) {
            ST7789_draw_line(x_int[i], y, x_int[i + 1], y, color);
        }
    }
}

#endif
