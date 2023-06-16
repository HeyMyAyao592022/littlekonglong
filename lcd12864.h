#ifndef _LCD_H_
#define _LCD_H_

#include "main.h"
#include "konglong.h"

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
 * @brief 清除一条痕迹
 *
 * @param y
 * @param y
 */
void clearLine(uint8 x, uint8 y);

/**
 * @brief 绘小板块
 *
 * @param x
 * @param y
 * @param standing 表明是否有小恐龙站在上面，或者是否达到了顶层
 */
void drawBlock(uint8 x, uint8 y, bit standing);

/**
 * @brief 绘图 16x16
 *
 */
void drawKonglong(KongLong *konglong);

/**
 * @brief 绘制左右边界的墙
 *
 */
// void drawWall();

/**
 * @brief 绘制顶部
 *
 */
void drawRoof();

#endif
