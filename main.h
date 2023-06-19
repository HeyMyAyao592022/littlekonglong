#ifndef _MAIN_H_
#define _MAIN_H_

// #include <AT89X55.H>
// #include <REGX52.H>

#include <reg52.H>
#include <intrins.h>
#include <math.h>
#include <stddef.h>

// 小恐龙的方向
#define DIR_LEFT 0
#define DIR_RIGHT 1
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_STILL -1
#define STANDINGBLOCK 2

#define DEFUALT_STANDINGBLCOK -1

// 坐标的边界值
#define BLCOK_MAX_Y 55
#define BLOCK_MIN_Y 1
#define KONGLONG_MAX_X 111
#define KONGLONG_MIN_X 1
#define KONGLONG_MAX_Y 48
#define BORDER_X 94

// 小板块最大数量
#define BLOCK_NUMBER 5

// 一些信号
#define CMD_CLEAR_BLOCK 1    // 清理到达最顶层的小板块
#define CMD_CLEAR_KONGLONG 2 // 清理小恐龙图像
#define CMD_REDRAW_WALL 3    // 重新画墙
#define CMD_WRITE_SCORE 4    // 显示分数的命令

// 游戏运行状态
#define STATUS_RUNNING 0 // 游戏正在进行
#define STATUS_VECTORY 2 // 游戏胜利
#define STATUS_LOSE 3    // 游戏失败

typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;
typedef char int8;
typedef int int16;

sbit RS = P1 ^ 0;
sbit RW = P1 ^ 1;
sbit E = P1 ^ 2;
sbit CS1 = P1 ^ 3;
sbit CS2 = P1 ^ 4;

sbit SCK = P1 ^ 5;
sbit SDA = P1 ^ 6;
sbit RST = P1 ^ 7; // DS1302

sbit KeyIn1 = P3 ^ 4;
sbit KeyIn2 = P3 ^ 5;
sbit KeyIn3 = P3 ^ 6;

sbit BEEP = P3 ^ 3; //

void delay(uint8 n); //

/**
 * @brief 定义小板块
 *
 */
typedef struct Block
{
    uint8 x, y;
} Block;

/**
 * @brief 定义小恐龙结构体
 *
 */
typedef struct KongLong
{
    uint8 x, y;              // x,y 坐标
    uint8 *image;            // 图像数组
    int8 horizon_direction;  // 左右方向，-1 保存水平, 0左 1 右
    int8 vertical_direction; // 上下方向, -1 保持垂直，0 上，1下, 2 站在小板块上
    uint8 towards;           // 小恐龙朝向方向
    int8 standingBlock;      // 代表小恐龙正在站着的小板块的编号
} KongLong;                  // 把 struct Konglong 定义成了 Konglong

#endif
