#include "lcd12864.h"

/**
 * @brief 星星
 *
 */
xdata const uint8 img_star[8] =
	{0x1E, 0x3E, 0x7F, 0x7E,
	 0x7E, 0x7F, 0x3E, 0x1E};

/**
 * @brief 数字的图像 4*8大小
 *
 */
xdata const uint8 img_nums[10][4] = {
	{0x1C, 0x24, 0x22, 0x3C}, /*"0",0*/
	{0x00, 0x04, 0x3E, 0x00}, /*"1",0*/
	{0x20, 0x32, 0x32, 0x2C}, /*"2",0*/
	{0x00, 0x2A, 0x2A, 0x34}, /*"3",0*/
	{0x10, 0x18, 0x14, 0x3E}, /*"4",0*/
	{0x00, 0x2E, 0x2A, 0x32}, /*"5",0*/
	{0x18, 0x34, 0x2A, 0x3A}, /*"6",0*/
	{0x02, 0x22, 0x1A, 0x06}, /*"7",0*/
	{0x14, 0x2C, 0x2A, 0x34}, /*"8",0*/
	{0x0C, 0x34, 0x32, 0x3C}};

/**
 * @brief 辅助显示板块
 *
 */
void auxDrawBlock(uint8 x, uint8 screen_y, uint8 offset_y);

/**
 * @brief 辅助显示恐龙
 *
 * @param konglong
 */

/**
 * @brief 辅助显示跨越左右屏幕的恐龙
 *
 */
void auxDrawKonglongX(KongLong *KongLong, uint8 screen_y, uint8 offset_y);

/**
 * @brief 清除小恐龙下降的残影
 *
 * @param x
 * @param screen_y
 */
void clearGhost(uint8 x, uint8 screen_y);

/**
 * @brief 小板块的二进制代码
 *
 */
#define blockDot 0x09

/**
 * @brief 小刺的二进制代码
 *
 */
#define blockDotThorn1 0x0f
#define blockDotThorn2 0x0e

/// @brief 辅助显示图片
data uint8 segment[16];

/// @brief
data int8 screen_y;
data int8 offset_y;

void chekbusy12864(void)
{
	data uint8 dat;

	RS = 0;
	RW = 1;
	do
	{
		P2 = 0;
		E = 1;
		_nop_();
		dat = P2 & 0x80;
		E = 0;
	} while (dat != 0);
}

void choose12864(uint8 i)
{
	switch (i)
	{
	case 0:
		CS1 = 0;
		CS2 = 1;
		break;
	case 1:
		CS1 = 1;
		CS2 = 0;
		break;

	default:
		break;
	}
}

void cmd_w12864(uint8 cmd)
{
	chekbusy12864();

	RS = 0;
	RW = 0;
	_nop_();
	E = 1;
	_nop_();
	P2 = cmd;
	_nop_();
	E = 0;
}

void dat_w12864(uint8 dat)
{
	chekbusy12864();

	RS = 1;
	RW = 0;
	_nop_();
	E = 1;
	_nop_();
	P2 = dat;
	_nop_();
	E = 0;
}

/**
 * @brief 读取屏幕数据
 *
 * @return uint8
 */
uint8 dat_r12864(void)
{
	data uint8 lcdData;
	chekbusy12864();
	P2 = 0xff;
	E = 0;
	RS = 1;
	RW = 1;
	E = 1;
	_nop_();
	_nop_();
	_nop_();
	lcdData = P2;
	_nop_();
	_nop_();
	_nop_();
	E = 0;
	return lcdData;
}

void clear12864(void)
{
	data uint8 page, row;

	// 选择左屏
	choose12864(0);
	// 遍历 8 页
	for (page = 0; page < 8; page++)
	{
		cmd_w12864(0xb8 + page);
		cmd_w12864(0x40);
		for (row = 0; row < 64; row++)
		{

			dat_w12864(0x00);
		}
	}

	choose12864(1);
	for (page = 0; page < 8; page++)
	{
		cmd_w12864(0xb8 + page);
		cmd_w12864(0x40);
		for (row = 0; row < 64; row++)
		{
			dat_w12864(0x00);
		}
	}
}

void LCD_init(void)
{
	chekbusy12864();
	cmd_w12864(0xc0);
	cmd_w12864(0x3f);
	clear12864();
}

void drawBlock(uint8 x, uint8 y, bit standing)
{
	data uint8 i;
	//	data uint8 blockDot = 0x0f;
	screen_y = y / 8; // 算出 y 对应屏幕的 y 坐标
	offset_y = y % 8; // 算出 y 对 screen_y 的偏移

	// 选择屏幕
	if (x > 63)
	{
		choose12864(1);
		x = x - 64;
	}
	else
		choose12864(0);

	// offset 为 0 就正常显示图片
	if (!offset_y)
	{
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | screen_y);

		dat_w12864(0x0f);
		for (i = 1; i < 15; i++)
			dat_w12864(blockDot);
		dat_w12864(0x0f);
	}
	// 否则，一张图片要拆分成2段
	else
	{
		// 先画第一段
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | (screen_y));

		// 如果板块上面站在小恐龙，则使用复杂绘图法
		if (standing)
		{
			auxDrawBlock(x, screen_y, offset_y);
		}
		else
		{
			segment[0] = (0x0f << offset_y) | 0x00;
			dat_w12864(segment[0]);
			for (i = 1; i < 15; i++)
			{
				segment[0] = (blockDot << offset_y) | 0x00;
				dat_w12864(segment[0]);
			}
			segment[0] = (0x0f << offset_y) | 0x00;
			dat_w12864(segment[0]);
		}

		// 再画第二段
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | (screen_y + 1));

		segment[0] = (0x0f >> (8 - offset_y));
		dat_w12864(segment[0]);
		for (i = 1; i < 15; i++)
		{
			segment[0] = (blockDot >> (8 - offset_y));
			dat_w12864(segment[0]);
		}
		segment[0] = (0x0f >> (8 - offset_y));
		dat_w12864(segment[0]);

		segment[0] = 0x00;
	}
}

/**
 * @brief 绘制小刺
 *
 * @param x
 * @param y
 */
void drawThurn(uint8 x, uint8 y)
{
	data uint8 i;
	data uint8 index = 0;
	screen_y = y / 8; // 算出 y 对应屏幕的 y 坐标
	offset_y = y % 8; // 算出 y 对 screen_y 的偏移

	// 选择屏幕
	if (x > 63)
	{
		choose12864(1);
		x = x - 64;
	}
	else
		choose12864(0);

	if (!(y % 8))
	{
		clearLine(x, y + 1);
	}

	// offset 为 0 就正常显示图片
	if (!offset_y)
	{
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | screen_y);

		dat_w12864(0x0f);
		for (i = 1; i < 15; i++)
			dat_w12864(blockDot);
		dat_w12864(0x0f);
	}
	// 否则，一张图片要拆分成2段
	else
	{
		// 先画第一段
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | (screen_y));

		// 第一段
		for (i = 0; i < 16; i++)
		{
			if (index++ % 2)
				segment[0] = (blockDotThorn1 << offset_y) | 0x00;
			else
				segment[0] = (blockDotThorn2 << offset_y) | 0x00;
			dat_w12864(segment[0]);
		}

		// 再画第二段
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | (screen_y + 1));

		for (i = 0; i < 16; i++)
		{
			segment[0] = (0x0f >> (8 - offset_y));
			dat_w12864(segment[0]);
		}

		segment[0] = 0x00;
	}
}

/**
 * @brief
 * 图像的 y 坐标并不是屏幕的实际 y 坐标，屏幕纵方向被分为了 8 段，每段有 8 行，共 64 个像素。
 * 我们每次向屏幕写入一个 8位16进制数据，那这个数据就会在屏幕上形成 纵方向的8个小点。（刚好是 16x16 图像高度的一半）
 * 一般我们要在 （screen_x,screen_y) 的地方写入 16x16 的图片，
 * 都是先在 (screen_x,screen_y)写上半部分，写入 8 个8位16进制数据，构成图像的上半部分，
 * 然后再在 (screen_x,screen_y + 1)写入下半部分。
 * 假如图像的 y 坐标刚好是 8 的倍数，比如 16，那 16x16 的图像刚好可以被放在屏幕 y 坐标为 2 的位置。
 * 如果图像的 y 坐标不是 8 的倍数，比如 18，那我们就需要把图像分成 3 段，并通过负责的运算把图像放到屏幕上。
 *
 */
void drawKonglong(KongLong *konglong)
{
	data uint8 i;
	data int8 x = konglong->x;
	screen_y = konglong->y / 8; // 算出 y 对应屏幕的 y 坐标
	offset_y = konglong->y % 8; // 算出 y 对 screen_y 的偏移

	// 判断是否在跨越左右屏幕
	if (konglong->x > 47 && konglong->x < 64)
	{
		auxDrawKonglongX(konglong, screen_y, offset_y);
		return;
	}

	// 选择屏幕
	if (x > 63)
	{
		choose12864(1);
		x = x - 64;
	}
	else
		choose12864(0);

	// 清除残影
	if (!((konglong->y - 1) % 8))
		clearGhost(konglong->x, screen_y - 1);

	// offset 为 0 就正常显示图片
	if (!offset_y)
	{
		// 向右移动和向左移动不同
		if (konglong->towards == DIR_RIGHT)
		{
			// 上半部分
			cmd_w12864(0x40 | x);
			cmd_w12864(0xb8 | screen_y);
			for (i = 0; i < 16; i++)
				dat_w12864(konglong->image[i]);

			// 下半部分
			cmd_w12864(0x40 | x);
			cmd_w12864(0xb8 | (screen_y + 1));
			for (i = 0; i < 16; i++)
				dat_w12864(konglong->image[i + 16]);
		}
		else
		{
			cmd_w12864(0x40 | x);
			cmd_w12864(0xb8 | screen_y);
			for (i = 0; i < 16; i++)
				dat_w12864(konglong->image[15 - i]);

			// 下半部分
			// 处理跨屏
			cmd_w12864(0x40 | x);
			cmd_w12864(0xb8 | (screen_y + 1));
			// 向右移动和向左移动不同
			for (i = 0; i < 16; i++)
				dat_w12864(konglong->image[31 - i]);
		}
	}
	// 否则，一张图片要拆分成3段
	else
	{
		if (konglong->towards == DIR_RIGHT)
		{
			// 先画第一段
			cmd_w12864(0x40 | x);
			cmd_w12864(0xb8 | screen_y);

			for (i = 0; i < 16; i++)
			{
				segment[0] = 0x00 | (konglong->image[i] << offset_y);
				dat_w12864(segment[0]);
			}

			// 第二段
			cmd_w12864(0x40 | x);
			cmd_w12864(0xb8 | (screen_y + 1));

			for (i = 0; i < 16; i++)
			{
				segment[0] = (konglong->image[i] >> (8 - offset_y)) | (konglong->image[i + 16] << offset_y);
				dat_w12864(segment[0]);
			}

			// 第三段
			cmd_w12864(0x40 | x);
			cmd_w12864(0xb8 | (screen_y + 2));

			for (i = 0; i < 16; i++)
			{
				segment[0] = konglong->image[i + 16] >> (8 - offset_y);
				dat_w12864(segment[0]);
			}
		}
		else
		{
			// 先画第一段
			cmd_w12864(0x40 | x);
			cmd_w12864(0xb8 | screen_y);
			for (i = 0; i < 16; i++)
			{
				segment[0] = 0x00 | (konglong->image[15 - i] << offset_y);
				dat_w12864(segment[0]);
			}

			// 第二段
			cmd_w12864(0x40 | x);
			cmd_w12864(0xb8 | (screen_y + 1));
			for (i = 0; i < 16; i++)
			{
				segment[0] = (konglong->image[15 - i] >> (8 - offset_y)) |
							 (konglong->image[31 - i] << offset_y);
				dat_w12864(segment[0]);
			}

			// 第三段
			cmd_w12864(0x40 | x);
			cmd_w12864(0xb8 | (screen_y + 2));
			for (i = 0; i < 16; i++)
			{
				segment[0] = konglong->image[31 - i] >> (8 - offset_y);
				dat_w12864(segment[0]);
			}
		}

		// 清除 segment
		segment[0] = 0x00;
		if (konglong->standingBlock != -1)
		{
			for (i = 1; i < 16; i++)
			{
				segment[i] = 0x00;
			}
		}
	}
}

void clearObject(uint8 x, uint8 y)
{
	data uint8 i;
	screen_y = y / 8;		// 算出 y 对应屏幕的 y 坐标
	offset_y = 8 - (y % 8); // 算出 y 对 screen_y 的偏移

	if (x > 63)
	{
		choose12864(1);
		x = x - 64;
	}
	else
		choose12864(0);

	// offset 为 0 就正常清除图片
	if (!offset_y)
	{
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | screen_y);
		for (i = 0; i < 16; i++)
			dat_w12864(0x00);

		// 下半部分
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | (screen_y + 1));
		for (i = 0; i < 16; i++)
			dat_w12864(0x00);
	}
	else
	{
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | (screen_y));
		for (i = 0; i < 16; i++)
			dat_w12864(0x00);

		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | (screen_y + 1));
		for (i = 0; i < 16; i++)
			dat_w12864(0x00);

		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | (screen_y + 2));
		for (i = 0; i < 16; i++)
			dat_w12864(0x00);
	}
}

void drawRoof()
{
	data uint8 i;
	choose12864(0);
	for (i = 0; i < 64; i++)
	{
		cmd_w12864(0x40 | i);
		cmd_w12864(0xb8);
		dat_w12864(0x01);
	}
	for (i = 0; i < 8; i++)
	{
		cmd_w12864(0x40);
		cmd_w12864(0xb8 | i);
		dat_w12864(0xff);
	}
	// 右
	choose12864(1);
	for (i = 0; i < 64; i++)
	{
		cmd_w12864(0x40 | i);
		cmd_w12864(0xb8);
		dat_w12864(0x01);
	}
	for (i = 0; i < 8; i++)
	{
		cmd_w12864(0x40 | 63);
		cmd_w12864(0xb8 | i);
		dat_w12864(0xff);
	}
}

/**
 * @brief 用复杂绘图法画第一段
 *
 * @param x
 * @param screen_y
 * @param offset_y
 */
void auxDrawBlock(uint8 x, uint8 screen_y, uint8 offset_y)
{
	uint8 i;

	// 指令已经在调用该函数前写入了
	// 画第一段
	// 如果小恐龙站在小板子上面，或者小板子达到了顶层，那就用复杂绘制法
	dat_r12864();
	segment[0] = (0x0f << offset_y) | dat_r12864();
	for (i = 1; i < 15; i++)
	{
		dat_r12864();
		segment[i] = (blockDot << offset_y) |
					 dat_r12864() & (0xff >> (8 - offset_y));
	}
	dat_r12864();
	segment[15] = (0x0f << offset_y) | dat_r12864();

	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | (screen_y));
	for (i = 0; i < 16; i++)
	{
		dat_w12864(segment[i]);
	}
}

/**
 * @brief 横跨左右屏幕的画法
 *
 * @param konglong
 * @param screen_y
 * @param offset_y
 */
void auxDrawKonglongX(KongLong *konglong, uint8 screen_y, uint8 offset_y)
{
	uint8 i;
	uint8 offset_x = 63 - konglong->x;
	// offset 为 0 就正常显示图片
	if (!offset_y)
	{
		/// @note 先画左边
		// 向右移动和向左移动不同
		if (konglong->towards == DIR_RIGHT)
		{
			// 上半部分
			cmd_w12864(0x40 | konglong->x);
			cmd_w12864(0xb8 | screen_y);
			for (i = 0; i < offset_x; i++)
				dat_w12864(konglong->image[i]);

			// 下半部分
			cmd_w12864(0x40 | konglong->x);
			cmd_w12864(0xb8 | (screen_y + 1));
			for (i = 0; i < offset_x; i++)
				dat_w12864(konglong->image[i + 16]);
		}
		else
		{
			cmd_w12864(0x40 | konglong->x);
			cmd_w12864(0xb8 | screen_y);
			for (i = 0; i < offset_x; i++)
				dat_w12864(konglong->image[15 - i]);

			// 下半部分
			// 处理跨屏
			cmd_w12864(0x40 | konglong->x);
			cmd_w12864(0xb8 | (screen_y + 1));
			// 向右移动和向左移动不同
			for (i = 0; i < offset_x; i++)
				dat_w12864(konglong->image[31 - i]);
		}

		/// @note 再画右边
		choose12864(1);

		// 向右移动和向左移动不同
		if (konglong->towards == DIR_RIGHT)
		{
			// 上半部分
			cmd_w12864(0x40);
			cmd_w12864(0xb8 | screen_y);
			for (i = offset_x; i < 16; i++)
				dat_w12864(konglong->image[i]);

			// 下半部分
			cmd_w12864(0x40);
			cmd_w12864(0xb8 | (screen_y + 1));
			for (i = offset_x; i < 16; i++)
				dat_w12864(konglong->image[i + 16]);
		}
		else
		{
			cmd_w12864(0x40);
			cmd_w12864(0xb8 | screen_y);
			for (i = offset_x; i < 16; i++)
				dat_w12864(konglong->image[15 - i]);

			// 下半部分
			// 处理跨屏
			cmd_w12864(0x40);
			cmd_w12864(0xb8 | (screen_y + 1));
			// 向右移动和向左移动不同
			for (i = offset_x; i < 16; i++)
				dat_w12864(konglong->image[31 - i]);
		}
	}
	// 否则，一张图片要拆分成3段
	else
	{
		/// @note 先画左边
		choose12864(0);

		if (konglong->towards == DIR_RIGHT)
		{
			// 先画第一段
			cmd_w12864(0x40 | konglong->x);
			cmd_w12864(0xb8 | screen_y);

			for (i = 0; i < offset_x; i++)
			{
				segment[0] = 0x00 | (konglong->image[i] << offset_y);
				dat_w12864(segment[0]);
			}

			// 第二段
			cmd_w12864(0x40 | konglong->x);
			cmd_w12864(0xb8 | (screen_y + 1));

			for (i = 0; i < offset_x; i++)
			{
				segment[0] = (konglong->image[i] >> (8 - offset_y)) | (konglong->image[i + 16] << offset_y);
				dat_w12864(segment[0]);
			}

			// 第三段
			cmd_w12864(0x40 | konglong->x);
			cmd_w12864(0xb8 | (screen_y + 2));

			for (i = 0; i < offset_x; i++)
			{
				segment[0] = konglong->image[i + 16] >> (8 - offset_y);
				dat_w12864(segment[0]);
			}
		}
		else
		{
			// 先画第一段
			cmd_w12864(0x40 | konglong->x);
			cmd_w12864(0xb8 | screen_y);
			for (i = 0; i < offset_x; i++)
			{
				segment[0] = 0x00 | (konglong->image[15 - i] << offset_y);
				dat_w12864(segment[0]);
			}

			// 第二段
			cmd_w12864(0x40 | konglong->x);
			cmd_w12864(0xb8 | (screen_y + 1));
			for (i = 0; i < offset_x; i++)
			{
				segment[0] = (konglong->image[15 - i] >> (8 - offset_y)) |
							 (konglong->image[31 - i] << offset_y);
				dat_w12864(segment[0]);
			}

			// 第三段
			cmd_w12864(0x40 | konglong->x);
			cmd_w12864(0xb8 | (screen_y + 2));
			for (i = 0; i < offset_x; i++)
			{
				segment[0] = konglong->image[31 - i] >> (8 - offset_y);
				dat_w12864(segment[0]);
			}
		}

		/// @brief 再画右边
		choose12864(1);

		if (konglong->towards == DIR_RIGHT)
		{
			// 先画第一段
			cmd_w12864(0x40);
			cmd_w12864(0xb8 | screen_y);

			for (i = offset_x; i < 16; i++)
			{
				segment[0] = 0x00 | (konglong->image[i] << offset_y);
				dat_w12864(segment[0]);
			}

			// 第二段
			cmd_w12864(0x40);
			cmd_w12864(0xb8 | (screen_y + 1));

			for (i = offset_x; i < 16; i++)
			{
				segment[0] = (konglong->image[i] >> (8 - offset_y)) | (konglong->image[i + 16] << offset_y);
				dat_w12864(segment[0]);
			}

			// 第三段
			cmd_w12864(0x40);
			cmd_w12864(0xb8 | (screen_y + 2));

			for (i = offset_x; i < 16; i++)
			{
				segment[0] = konglong->image[i + 16] >> (8 - offset_y);
				dat_w12864(segment[0]);
			}
		}
		else
		{
			// 先画第一段
			cmd_w12864(0x40);
			cmd_w12864(0xb8 | screen_y);
			for (i = offset_x; i < 16; i++)
			{
				segment[0] = 0x00 | (konglong->image[15 - i] << offset_y);
				dat_w12864(segment[0]);
			}

			// 第二段
			cmd_w12864(0x40);
			cmd_w12864(0xb8 | (screen_y + 1));
			for (i = offset_x; i < 16; i++)
			{
				segment[0] = (konglong->image[15 - i] >> (8 - offset_y)) |
							 (konglong->image[31 - i] << offset_y);
				dat_w12864(segment[0]);
			}

			// 第三段
			cmd_w12864(0x40);
			cmd_w12864(0xb8 | (screen_y + 2));
			for (i = offset_x; i < 16; i++)
			{
				segment[0] = konglong->image[31 - i] >> (8 - offset_y);
				dat_w12864(segment[0]);
			}
		}

		// 清除 segment
		segment[0] = 0x00;
		if (konglong->standingBlock != -1)
		{
			for (i = 1; i < 16; i++)
			{
				segment[i] = 0x00;
			}
		}
	}
}

/**
 * @brief 清除小恐龙下降时产生的残影
 *
 */
void clearGhost(uint8 x, uint8 screen_y)
{
	uint8 i;
	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | (screen_y));
	for (i = 0; i < 16; i++)
	{
		dat_w12864(0x00);
	}
}

/**
 * @brief 清除一条痕迹
 *
 * @param x
 * @param y
 */
void clearLine(uint8 x, uint8 y)
{
	if (x > 63)
	{
		x -= 64;
		choose12864(1);
	}
	else
		choose12864(0);

	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | y / 8);
	dat_w12864(0x00);
}

/**
 * @brief 显示表盘
 *
 */
void drawPanel()
{
	uint8 i;
	choose12864(1);
	// 画星星
	cmd_w12864(0x40 | 38);
	cmd_w12864(0xb8 | 1);
	for (i = 0; i < 8; ++i)
	{
		dat_w12864(img_star[i]);
	}
}

/**
 * @brief 显示分数
 *
 * @param score
 */
void updateScores(uint8 score)
{
	uint8 digit;
	uint8 i;

	choose12864(1);

	digit = score % 10;
	cmd_w12864(0x40 | 54);
	cmd_w12864(0xb8 | 1);
	for (i = 0; i < 4; i++)
	{
		dat_w12864(img_nums[digit][i]);
	}

	digit = score / 10;
	cmd_w12864(0x40 | 46);
	cmd_w12864(0xb8 | 1);
	for (i = 0; i < 4; i++)
	{
		dat_w12864(img_nums[digit][i]);
	}
}

/**
 * @brief 8*16 大小字符
 *
 * @param x
 * @param y
 * @param addr
 */
void play8(uint8 x, uint8 y, uint8 *addr)
{
	uint8 i;

	// x>63，说明 x 在右边，选择右屏
	if (x > 63)
	{
		choose12864(1);
		x = x - 64;
	}
	else
		choose12864(0);

	// 写入 x,y 位置
	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | y);

	// 写入 8 个 8 位的 16 进制数据
	for (i = 0; i < 8; i++)
		dat_w12864(*addr++);
}

/**
 * @brief 16*16大小字符
 *
 * @param x
 * @param y
 * @param addr
 */
void play16(uint8 x, uint8 y, uint8 *addr)
{
	uint8 i;

	if (x > 63)
	{
		choose12864(1);
		x = x - 64;
	}
	else
		choose12864(0);

	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | y);
	for (i = 0; i < 16; i++)
		dat_w12864(*addr++);

	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | (y + 1));
	for (i = 0; i < 16; i++)
		dat_w12864(*addr++);
}

/**
 * @brief 展示 32*32 大小的图片
 *
 * @param x
 * @param y
 * @param addr
 */
void play32(uint8 x, uint8 y, uint8 *addr)
{
	uint8 i;

	if (x > 63)
	{
		choose12864(1);
		x = x - 64;
	}
	else
		choose12864(0);

	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | y);
	for (i = 0; i < 32; i++)
		dat_w12864(*addr++);

	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | (y + 1));
	for (i = 0; i < 32; i++)
		dat_w12864(*addr++);

	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | (y + 2));
	for (i = 0; i < 32; i++)
		dat_w12864(*addr++);

	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | (y + 3));
	for (i = 0; i < 32; i++)
		dat_w12864(*addr++);
}

/**
 * @brief 画黑色
 *
 * @param x
 * @param y
 */
void drawDark(uint8 x, uint8 y)
{
	uint8 i;

	if (x > 63)
	{
		choose12864(1);
		x = x - 64;
	}
	else
		choose12864(0);

	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | y);
	dat_w12864(0xff);
}
