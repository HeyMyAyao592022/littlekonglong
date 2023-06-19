#include "main.h"
#include "konglong.h"
#include "lcd12864.h"
#include "imgarr.h"
#include "eeprom.h"
#include "ds1302.h"
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

/**
 * @brief 更新小刺
 *
 */
void generateThurn();

/**
 * @brief 更新小刺的纵坐标
 *
 */
void updateThurnPos();

/**
 * @brief 检测是否碰到小刺
 *
 */
void checkStandingThurn();

/**
 * @brief 开始界面
 *
 */
void startScreen();

/**
 * @brief 结束界面
 *
 */
void endScreen();

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
	{25, BLCOK_MAX_Y + 73},
	{65, BLCOK_MAX_Y + 117},
	{88, BLCOK_MAX_Y + 151}};

/// @brief 代表本次是第几次游戏
data uint8 gamePlayTimes = 0;
data uint8 preScores = 0;

/// @brief 定时器计数
data uint8 counter1 = 1;
data uint8 counter2 = 1;

/// @brief 定时器发出的信号，在主函数中相应
data bit onUpdateKonglong1 = 0;
data bit onUpdateKonglong2 = 0;
data bit onUpdateBlock = 0;
data bit onRedrawRoof = 1;
data bit onCheckStanding = 1;
data bit onPanel = 1;
data bit onScore = 1;
data bit onClear = 0;

/// @brief 小刺出现的信号
data bit onThurn = 0;
/// @brief 小刺是否出现的标志
data bit thurnShow = 0;
/// @brief 小刺
data Block thurnBlock = {0, 0};

/// @brief 给小板块生成新的 x 坐标
xdata uint8 blockHorizonPos[] = {23, 4, 45, 65, 84, 75};
data uint8 currentHorizonPos = 0;
#define BLOCK_HORIZON_POS_MAX 6

void main(void)
{
	// 遍历
	data uint8 i;

	// 初始化 eeprom
	init_eeprom();
	// 读取数据
	read_eeprom(&gameScores, &preScores);

	// 初始化屏幕
	LCD_init();
	delay(5);

	// 启动界面
	startScreen();

	// 初始化时钟
	set_time();

	// 初始化定时器
	initializeTimer();

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

		// 绘制小刺
		if (thurnShow)
		{
			if (thurnBlock.y <= BLCOK_MAX_Y)
				drawThurn(thurnBlock.x, thurnBlock.y);
		}

		// 绘制表盘
		if (onPanel)
		{
			onPanel = 0;
			drawPanel();
		}

		// 绘制边界
		if (onRedrawRoof)
		{
			onRedrawRoof = 0;
			drawRoof();
		}

		// 更新分数
		if (onScore)
		{
			onScore = 0;
			if (gameScores <= 99)
				updateScores(gameScores);
		}

		// 扫码按键
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

		// 检查小恐龙是否站在小板块上面
		if (onCheckStanding)
		{
			onCheckStanding = 0;
			checkStandingBlock();

			// 如果小刺出现，那也检测小刺
			if (thurnShow)
			{
				checkStandingThurn();
			}
		}

		// 更新小恐龙 x 坐标
		if (onUpdateKonglong1)
		{
			onUpdateKonglong1 = 0;
			updataKonglongHorizonPos();
		}

		// 更新小恐龙 y 坐标
		if (onUpdateKonglong2)
		{
			onUpdateKonglong2 = 0;
			updataKonglongVerticalPos();
		}

		// 更新小板块坐标
		if (onUpdateBlock)
		{
			onUpdateBlock = 0;
			updateBlockPos();

			// 如果小刺出现，那也更新小刺的坐标
			if (thurnShow)
			{
				updateThurnPos();
			}
		}

		// 更新小刺
		if (onThurn)
		{
			onThurn = 0;
			generateThurn();
		}

		// 清屏
		if (onClear)
		{
			onClear = 0;
			clear12864();
			onPanel = 1;
			onScore = 1;
		}
	}

	// 结束界面
	endScreen();
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
	if (!(counter1 % 5))
	{
		onUpdateKonglong1 = 1;
	}

	// 更新小恐龙垂直位移
	if (!(counter1 % 10))
	{
		onUpdateKonglong2 = 1;
	}

	// 更新小板块的坐标
	if (!(counter1 % 10))
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
	++counter2;

	if (!(counter2 % 60))
	{
		onRedrawRoof = 1;
	}

	if (!(counter2 % 20))
	{
		onPanel = 1;
		onScore = 1;
	}

	if (!(counter2 % 100) && !thurnShow)
	{
		onThurn = 1;
	}
}

/**
 * @brief 碰撞检测
 *
 */
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

/**
 * @brief 检测是否碰到小刺
 *
 */
void checkStandingThurn()
{
	uint8 diff;
	if ((konglongBo.y + 16) > thurnBlock.y)
		diff = (konglongBo.y + 16) - thurnBlock.y;
	else
		diff = thurnBlock.y - (konglongBo.y + 16);
	if (diff <= 3 &&
		(konglongBo.x + 10 >= thurnBlock.x && konglongBo.x <= (thurnBlock.x + 15)))
	{
		gameStatus = STATUS_LOSE;
	}
}

/**
 * @brief 更新小恐龙横坐标
 *
 */
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

/**
 * @brief 更新小恐龙纵坐标
 *
 */
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

/**
 * @brief 更新小板块
 *
 */
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
			currentHorizonPos = (currentHorizonPos + 1) % BLOCK_HORIZON_POS_MAX;
			blocks[i].x = blockHorizonPos[currentHorizonPos];
			blocks[i].y = BLCOK_MAX_Y + 53;

			// 每 5 个小板块加 1 分
			if (i == 4)
			{
				++gameScores;
				onScore = 1;
			}
		}
	}
	// 移动后需要检查是否站在小板块上面
	onCheckStanding = 1;
}

/**
 * @brief 刷新小刺
 *
 */
void generateThurn()
{
	currentHorizonPos = (currentHorizonPos + 1) % BLOCK_HORIZON_POS_MAX;
	thurnBlock.x = blockHorizonPos[currentHorizonPos];
	thurnBlock.y = BLCOK_MAX_Y + 71;
	thurnShow = 1;
}

/**
 * @brief 更新小刺坐标
 *
 */
void updateThurnPos()
{
	if (thurnBlock.y > BLOCK_MIN_Y)
	{
		--thurnBlock.y;
	}
	else
	{
		clearObject(thurnBlock.x, thurnBlock.y);
		onRedrawRoof = 1;
		// 小刺消失
		thurnShow = 0;
	}
}

/**
 * @brief 开始界面
 *
 */
void startScreen()
{
	// 用于遍历
	// 绘制小恐龙封面
	play16(46, 3, img_flags);
	// 绘制“开始”
	drawDark(53, 6, 0xff);
	drawDark(74, 6, 0xff);
	play8(54, 6, img_kai);
	play8(65, 6, img_shi);
	// // 绘制游戏次数
	playNums(65, 3, gamePlayTimes);

	// 按下 Key3 则加载游戏
	while (KeyIn3)
	{
		// 按下 key1 切换
		if (!KeyIn3)
		{
			delay(500);
			if (KeyIn3)
				break;
			;
		}
	}
	clear12864();
	clear12864();
	delay(1000);
}

/**
 * @brief 结束界面
 *
 */
void endScreen()
{
	// 接收时钟
	Time time1;

	clear12864();
	clear12864();
	delay(1000);

	// 结束
	play16(30, 2, img_konglong);
	playNums(65, 2, gameScores);

	// 更新分数
	if (preScores < gameScores)
		write_eeprom(gameScores, gamePlayTimes + 1);

	// 读取时钟
	read_time((uint8 *)&time1);
	// 显示数据
	playNums(28, 5, time1.min);
	drawDark(65, 5, 0x18);
	playNums(66, 5, time1.sec);

	// 延迟 3 ms
	delay(3000);

	while (1)
	{
	}
}
