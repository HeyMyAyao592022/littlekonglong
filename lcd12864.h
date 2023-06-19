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
 * @brief 展示 8*16 大小的图片
 *
 * @param x
 * @param y
 * @param addr
 */
void play8(uint8 x, uint8 y, uint8 *addr);

/**
 * @brief 展示 16*16 大小的图片
 *
 * @param x
 * @param y
 * @param addr
 */
void play16(uint8 x, uint8 y, uint8 *addr);

/**
 * @brief 显示数字
 *
 * @param x
 * @param y
 * @param num
 */
void playNums(uint8 x, uint8 y, uint8 num);

/**
 * @brief 展示 32*32 大小的图片
 *
 * @param x
 * @param y
 * @param addr
 */
// void play32(uint8 x, uint8 y, uint8 *addr);

/**
 * @brief 单纯画一片黑色
 *
 * @param x
 * @param y
 */
void drawDark(uint8 x, uint8 y, uint8 dark);

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
 * @brief 绘制小刺
 *
 * @param x
 * @param y
 */
void drawThurn(uint8 x, uint8 y);

/**
 * @brief 绘图 16x16
 *
 */
void drawKonglong(KongLong *konglong);

/**
 * @brief 显示表盘
 *
 */
void drawPanel();

/**
 * @brief 显示分
 *
 * @param score
 */
void updateScores(uint8 scores);

/**
 * @brief 绘制顶部
 *
 */
void drawRoof();

#endif
