#include "main.h"
#include "lcd12864.h"
#include "imgarr.h"

/**
 * @brief 定义小恐龙结构体
 *
 */
typedef struct KongLong
{
	uint8 x, y;	  // x,y 坐标
	uint8 *image; // 图像数组
} KongLong;		  // 把 struct Konglong 定义成了 Konglong

// 定义一个实例
KongLong konglongBo = {0, 0, img_konglong};

/// @brief 一些变量
uint16 counter1 = 0;
uint16 counter2 = 0;

/**
 * @brief 初始化定时器
 *
 */
void initializeTimer()
{
	TMOD = 0x01;		   // 使用模式一
	TL0 = 64535 % 256 + 1; // 1ms 中断一次
	TH0 = 64535 / 256;

	TF0 = 0;
	TR0 = 1; // 定时器0 开始

	ET0 = 1; // 允许定时器0
	EA = 1;	 // 允许定时
	PT0 = 0; // 低优先级
}

/**
 * @brief 定时器 0
 *
 */
void onTimer1() interrupt 0
{
	// counter 用于计数
	++counter2;

	TL0 = 64535 % 256 + 1; // 重新装初值
	TH0 = 64535 / 256;

	if (counter2 >= 500)
	{
		++konglongBo.x;
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
		++counter1;

		drawPicture16(konglongBo.x, konglongBo.y, konglongBo.image);

		if (counter1 > 20)
		{
			counter1 = 0;
			++konglongBo.x;
		}
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