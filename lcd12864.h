#ifndef _LCD_H_
#define _LCD_H_

#include "main.h"

/**
 * @brief 初始化
 *
 */
void LCD_init(void);

/**
 * @brief 清屏
 *
 */
void clear12864(void);

/**
 * @brief 清楚制定位置的图像
 *
 * @param x
 * @param y
 */
void clearObject(uint8 x, uint8 y);

/**
 * @brief 显示字符 8x16 (16个8位16进制数据)
 *
 * @param x 0~128
 * @param y
 * @param addr
 */
void play8(uint8 x, uint8 y, uint8 *addr);

/**
 * @brief 显示字符 16x16
 *
 * @param x
 * @param y
 * @param addr
 */
void play16(uint8 x, uint8 y, uint8 *addr);

/**
 * @brief 绘小板块
 *
 * @param x
 * @param y
 */
void drawBlock(uint8 x, uint8 y);

/**
 * @brief 绘图 16x16
 *
 * @param x x 坐标 0~127
 * @param y y 坐标 0~3
 * @param addr 图像数组
 * @param dir 方向
 */
void drawKonglong(uint8 x, uint8 y, uint8 *addr, int8 dir_horizon, int8 dir_vertical);

#endif
