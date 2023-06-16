#include "main.h"
#include "konglong.h"
#include "lcd12864.h"
#include "imgarr.h"
// #include "seqqueue.h"

sbit monitorLight = P3 ^ 3;

/**
 * @brief 初始化定时器
 *
 */
void initializeTimer();

/**
 * @brief 检查小恐龙是否站在板子上
 *
 */
void checkStandingBlock();

/**
 * @brief 更新小恐龙 x 坐标
 *
 */
void updataKonglongHorizonPos();

/**
 * @brief 更新小恐龙 y 坐标
 *
 */
void updataKonglongVerticalPos();

/**
 * @brief 更新小板块坐标
 *
 */
void updateBlockPos();

/// @brief 游戏的状态
data uint8 gameStatus = STATUS_RUNNING;
data uint8 gameScores = 0;

/// @brief 小恐龙
data KongLong konglongBo = {KONGLONG_MIN_X, BLCOK_MAX_Y - 16, img_konglong,
							DIR_STILL, DIR_UP, DIR_LEFT, DEFUALT_STANDINGBLCOK};

/// @brief 小板块
data Block blocks[] = {
	{KONGLONG_MIN_X, BLCOK_MAX_Y},
	{30, BLCOK_MAX_Y + 31},
	{30, BLCOK_MAX_Y + 73},
	{80, BLCOK_MAX_Y + 117},
	{110, BLCOK_MAX_Y + 151}};

/// @brief 定时器计数
data uint8 counter1 = 0;
data uint8 counter2 = 0;

/// @brief 定时器发出的信号，在主函数中相应
data bit onUpdateKonglong1 = 0;
data bit onUpdateKonglong2 = 0;
data bit onUpdateBlock = 0;
data bit onRedrawRoof = 0;
// data bit onRedrawWall = 0;
data bit onCheckStanding = 1;

/// @brief 给小板块生成新的 x 坐标
xdata uint8 blockHorizonPos[] = {30, 4, 45, 70, 110, 90};
data uint8 currentHorizonPos = 0;
#define BLOCK_HORIZON_POS_MAX 6

void main(void)
{
	// 遍历
	data uint8 i;

	// 按下 key1 开始
	while (1)
	{
		// 按 key1 开始
		if (!KeyIn1)
		{
			delay(10);
			break;
		}
	}

	// 初始化定时器
	initializeTimer();

	// 初始化屏幕
	LCD_init();
	delay(5);

	// 画顶部
	drawRoof();
	// drawWall();

	// 只要游戏还在运行中，那就不退出循环
	// game statues 为 0 则说明在运行中
	while (!gameStatus)
	{
		//  绘图
		if (konglongBo.standingBlock != -1)
		{
			drawBlock(blocks[konglongBo.standingBlock].x, blocks[konglongBo.standingBlock].y,
					  1);
		}
		drawKonglong(&konglongBo);
		for (i = 0; i < BLOCK_NUMBER; i++)
		{
			if ((i != (konglongBo.standingBlock)) && (blocks[i].y <= BLCOK_MAX_Y))
				drawBlock(blocks[i].x, blocks[i].y, blocks[i].y <= 7);
		}

		// scan key
		if (!KeyIn1) // 向左
		{
			// 改变方向，则需要清除残影
			if (konglongBo.towards == DIR_RIGHT)
			{
				clearLine(konglongBo.x, konglongBo.y);
			}
			konglongBo.horizon_direction = DIR_LEFT;
			konglongBo.towards = DIR_LEFT;
		}
		else if (!KeyIn2) // 向右
		{
			// 改变方向，则需要清除残影
			if (konglongBo.towards == DIR_LEFT)
			{
				clearLine(konglongBo.x, konglongBo.y);
			}
			konglongBo.horizon_direction = DIR_RIGHT;
			konglongBo.towards = DIR_RIGHT;
		}
		else
			konglongBo.horizon_direction = DIR_STILL;

		if (onCheckStanding)
		{
			onCheckStanding = 0;
			checkStandingBlock();
		}

		if (onUpdateKonglong1)
		{
			onUpdateKonglong1 = 0;
			updataKonglongHorizonPos();
		}

		if (onUpdateKonglong2)
		{
			onUpdateKonglong2 = 0;
			updataKonglongVerticalPos();
		}

		if (onUpdateBlock)
		{
			onUpdateBlock = 0;
			updateBlockPos();
		}

		if (onRedrawRoof)
		{
			onRedrawRoof = 0;
			drawRoof();
		}

		// if (onRedrawWall)
		// {
		// 	onRedrawWall = 0;
		// 	drawWall();
		// }
	}

	// 若退出上面的循环，说明游戏结束，显示结果界面
	clear12864();

	while (1)
	{
	}
}

/**
 * @brief 毫秒延迟
 *
 * @param n
 */
void delay(uint8 n) // 延时ms级
{
	data uint8 counter;
	while (n--)
	{
		counter = 110;
		while (--counter)
			;
	}
}

/**
 * @brief 初始化定时器
 *
 */
void initializeTimer()
{
	TMOD = 0x11;				// 定时器0、1使用模式一
	TL0 = (65535 - 9216) % 256; // 10ms 中断一次
	TH0 = (65535 - 9216) / 256;
	TL1 = 0xff;
	TH1 = 0xff;

	TR0 = 1; // 定时器0 开始
	ET0 = 1; // 允许定时器0
	TR1 = 1;
	ET1 = 1;

	EA = 1;	 // 允许定时
	PT0 = 1; // 优先级
}

/**
 * @brief 定时器 0
 *
 */
void onTimer1() interrupt 1
{
	// counter 用于计数
	++counter1;

	// 定时器重新装初值
	TH0 = (65535 - 9216) / 256;
	TL0 = (65535 - 9216) % 256;

	// 每50ms
	if (!(counter1 % 5) && counter1)
	{
		onUpdateKonglong1 = 1;
	}

	// 更新小恐龙垂直位移
	if (!(counter1 % 10) && counter1)
	{
		onUpdateKonglong2 = 1;
	}

	// 更新小板块的坐标
	if (!(counter1 % 10) && counter1)
	{
		onUpdateBlock = 1;
	}
}

/**
 * @brief 定时器 1
 *
 */
void onTimer2() interrupt 3
{
	if (!(counter2++ % 50) && counter2)
	{
		onRedrawRoof = 1;
	}
}

void checkStandingBlock()
{
	data int8 standingFlag = -1;
	data uint8 diff;
	data uint8 i;

	// 如果小恐龙之前站在小板块上，则判断它是否离开了板块
	if (konglongBo.standingBlock != -1)
	{
		if ((konglongBo.y + 16) > blocks[konglongBo.standingBlock].y)
			diff = (konglongBo.y + 16) - blocks[konglongBo.standingBlock].y;
		else
			diff = blocks[konglongBo.standingBlock].y - (konglongBo.y + 16);
		if (diff <= 3 &&
			((konglongBo.x + 10) >= blocks[konglongBo.standingBlock].x &&
			 konglongBo.x <= (blocks[konglongBo.standingBlock].x + 15)))
		{
			// 如果小恐龙还站在那块板子上，就不需要重新检测了
			goto skipCheckStanding;
		}
		else
			// 如果小恐龙离开了那块板子，就需要重新检测
			goto checkStanding;
	}
	else
	{
	checkStanding:
		// 碰撞检测，判断小恐龙是否站在小板块上
		for (i = 0; i < BLOCK_NUMBER; i++)
		{
			if ((konglongBo.y + 16) > blocks[i].y)
				diff = (konglongBo.y + 16) - blocks[i].y;
			else
				diff = blocks[i].y - (konglongBo.y + 16);
			if (diff <= 3 &&
				(konglongBo.x + 10 >= blocks[i].x && konglongBo.x <= (blocks[i].x + 15)))
			{
				konglongBo.vertical_direction = DIR_UP;
				// 让小恐龙的 y 坐标与板块保持一直
				konglongBo.y = blocks[i].y - 16;
				standingFlag = i;
				break;
			}
		}
		// 如果发现小恐龙没有站在任何小板块上，则让小恐龙一直下落
		konglongBo.standingBlock = standingFlag;
		if (konglongBo.standingBlock == -1)
		{
			konglongBo.vertical_direction = DIR_DOWN;
		}
	}
skipCheckStanding:;
}

void updataKonglongHorizonPos()
{
	//  更新小恐龙水平位移
	switch (konglongBo.horizon_direction)
	{
	case DIR_LEFT: // 向左
		if (konglongBo.x > KONGLONG_MIN_X)
		{
			--konglongBo.x;
			// 移动后需要检查是否站在小板块上面
			onCheckStanding = 1;
		}
		break;
	case DIR_RIGHT: // 向右
		if (konglongBo.x < 111)
		{
			++konglongBo.x;
			// 移动后需要检查是否站在小板块上面
			onCheckStanding = 1;
		}
		break;
	default:
		break;
	}
}

void updataKonglongVerticalPos()
{
	// 更新小恐龙垂直位移
	switch (konglongBo.vertical_direction)
	{
	case DIR_UP: // 向上
		if (konglongBo.standingBlock != -1)
		{
			konglongBo.y = blocks[konglongBo.standingBlock].y - 16;
			// 如果恐龙的 y 比 1 小，说明我们输了
			if (konglongBo.y < BLOCK_MIN_Y)
			{
				gameStatus = STATUS_LOSE;
			}
		}
		break;
	case DIR_DOWN: // 向下
		if (konglongBo.y < KONGLONG_MAX_Y)
		{
			++konglongBo.y;
			// 移动后需要检查是否站在小板块上面
			onCheckStanding = 1;
		}
		else
			// 如果恐龙的 y 比 规定的y最大值 大，说明我们输了
			gameStatus = STATUS_LOSE;
		break;
	default:
		break;
	}
}

void updateBlockPos()
{
	data uint8 i;
	for (i = 0; i < BLOCK_NUMBER; i++)
	{
		if (blocks[i].y > BLOCK_MIN_Y)
			--blocks[i].y;
		// 回收小板块
		else
		{
			clearObject(blocks[i].x, blocks[i].y);
			onRedrawRoof = 1;
			// 重置小板块的 y
			blocks[i].x = blockHorizonPos[(currentHorizonPos++) % BLOCK_HORIZON_POS_MAX];
			blocks[i].y = BLCOK_MAX_Y + 53;
		}
	}
	// 移动后需要检查是否站在小板块上面
	onCheckStanding = 1;
}