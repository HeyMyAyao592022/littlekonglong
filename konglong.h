#ifndef __KONGLONG_H__
#define __KONGLONG_H__

#include "main.h"

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

#endif // __KONGLONG_H__