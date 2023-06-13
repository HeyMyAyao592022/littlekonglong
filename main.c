#include "main.h"
#include "lcd12864.h"
#include "imgarr.h"

/**
 * @brief 定义小恐龙结构体
 *
 */
typedef struct KongLong
{
	uint8 x, y;				 // x,y 坐标
	uint8 *image;			 // 图像数组
	int8 horizon_direction;	 // 左右方向，-1 保存水平, 0左 1 右
	int8 vertical_direction; // 上下方向, -1 保持垂直，0 上，1下, 2 站在小板块上

} KongLong; // 把 struct Konglong 定义成了 Konglong

// 定义一个实例
KongLong konglongBo = {0, 6, rec_frame, DIR_STILL, DIR_DOWN};

/// @brief 一些变量
uint16 counter1 = 0;
uint16 timer0_counter1 = 0;
uint8 konglong_counter1 = 0;

/**
 * @brief 初始化定时器
 *
 */
void initializeTimer()
{
	TMOD = 0x01;				// 使用模式一
	TL0 = (65535 - 9216) % 256; // 10ms 中断一次
	TH0 = (65535 - 9216) / 256;

	TR0 = 1; // 定时器0 开始
	ET0 = 1; // 允许定时器0
	EA = 1;	 // 允许定时
	PT0 = 0; // 低优先级
}

/**
 * @brief 定时器 0
 *
 */
void onTimer1() interrupt 1
{
	// counter 用于计数
	++timer0_counter1;

	TH0 = (65535 - 9216) / 256; // 10ms溢出中断
	TL0 = (65535 - 9216) % 256;

	// 每50ms
	if (timer0_counter1 >= 10)
	{
		// 更新小恐龙水平位移
		switch (konglongBo.horizon_direction)
		{
		case DIR_LEFT: // 向左
			if (konglongBo.x != 0)
				--konglongBo.x;
			break;
		case DIR_RIGHT: // 向右
			if (konglongBo.x != 111)
				++konglongBo.x;
			break;
		default:
			break;
		}
		// 更新小恐龙垂直位移
		switch (konglongBo.vertical_direction)
		{
		case DIR_UP: // 向上
			++konglong_counter1;
			--konglongBo.y;
			if (konglong_counter1 == 10)
			{
				konglong_counter1 = 0;
				konglongBo.vertical_direction = 1;
			}
			break;
		case DIR_DOWN: // 向下
			++konglongBo.y;
			break;
		default:
			break;
		}
		// 归零
		timer0_counter1 = 0;
	}

	// TODO: 判断按键是否按下

	// TODO: 更新小恐龙的横坐标

	// TODO: 如果小恐龙未站在小板块上，那还需要更新小恐龙的纵坐标

	// TODO: 生成新的小块
}

/**
 * @brief 定时器 1
 *
 */
void onTimer2()
{
	// TODO: 更新小板块的坐标

	// TODO: 判断是否需要更改定时器初值，让小板块越来越快
}

void main(void)
{
	// 初始化定时器
	initializeTimer();

	// 初始化屏幕
	LCD_init();
	delay(5);

	while (1)
	{
		// TODO: 计算绘图地址
		// TODO: 绘图

		drawPicture16(konglongBo.x, konglongBo.y, konglongBo.image, konglongBo.horizon_direction, konglongBo.vertical_direction);
	}
}

/**
 * @brief 毫秒延迟
 *
 * @param n
 */
void delay(uint16 n) // 延时ms级
{
	uint8 counter;
	while (n--)
	{
		counter = 110;
		while (--counter)
			;
	}
}