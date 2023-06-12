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
 * @brief 绘图 8x16
 *
 * @param x
 * @param y
 * @param addr
 */
void drawPicture8(uint8 x, uint8 y, uint8 *addr);

/**
 * @brief 绘图 16x16
 *
 * @param x
 * @param y
 * @param addr
 */
void drawPicture16(uint8 x, uint8 y, uint8 *addr);

#endif
