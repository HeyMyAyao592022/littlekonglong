/*
 * LCD128*64
 */

#include "lcd12864.h"

void chekbusy12864(void)
{
	uint8 dat;

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

void clear12864(void)
{
	uint8 page, row;

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
	cmd_w12864(0xb8 | (y++));

	// 确保 y 第八位为 0
	if ((y & 0x80) == 0)
		// 写入 8 个 8 位的 16 进制数据
		for (i = 0; i < 8; i++)
			dat_w12864(*addr++);
	else
		for (i = 0; i < 8; i++)
			dat_w12864(0xFF - *addr++);

	// 写入 x, y + 1
	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | y);

	if ((y & 0x80) == 0)
		for (i = 0; i < 8; i++)
			dat_w12864(*addr++);
	else
		for (i = 0; i < 8; i++)
			dat_w12864(0xFF - *addr++);
}

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
	cmd_w12864(0xb8 | (y++));

	if ((y & 0x80) == 0)
		for (i = 0; i < 16; i++)
			dat_w12864(*addr++);
	else
		for (i = 0; i < 16; i++)
			dat_w12864(0xFF - *addr++);

	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | y);

	if ((y & 0x80) == 0)
		for (i = 0; i < 16; i++)
			dat_w12864(*addr++);
	else
		for (i = 0; i < 16; i++)
			dat_w12864(0xFF - *addr++);
}

void drawPicture8(uint8 x, uint8 y, uint8 *addr)
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
	cmd_w12864(0xb8 | (y++));

	// 确保 y 第八位为 0
	if ((y & 0x80) == 0)
		// 写入 8 个 8 位的 16 进制数据
		for (i = 0; i < 8; i++)
			dat_w12864(*addr++);
	else
		for (i = 0; i < 8; i++)
			dat_w12864(0xFF - *addr++);

	// 写入 x, y + 1
	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | y);

	if ((y & 0x80) == 0)
		for (i = 0; i < 8; i++)
			dat_w12864(*addr++);
	else
		for (i = 0; i < 8; i++)
			dat_w12864(0xFF - *addr++);
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
 * @param x 0~127
 * @param y 0~63
 * @param addr
 * @param dir_horizon
 * @param dir_vertical
 */
void drawPicture16(uint8 x, uint8 y, uint8 *addr, int8 dir_horizon, int8 dir_vertical)
{

	int8 screen_y = y / 8;		 // 算出 y 对应屏幕的 y 坐标
	int8 offset_y = 8 - (y % 8); // 算出 y 对 screen_y 的偏移

	uint8 i;
	uint8 crossingScreen = 0;

	if (x > 47 && x < 63)
	{
		switch (dir_horizon)
		{
		case DIR_LEFT:
			crossingScreen = 1;
			break;
		case DIR_RIGHT:
			crossingScreen = 2;
			break;
		default:
			break;
		}
	}

	if (x > 63)
	{
		choose12864(1);
		x = x - 64;
	}
	else if (x == 63)
	{
		choose12864(1);
		x = 0;
	}
	else
		choose12864(0);

	// offset 为 0 就正常显示图片
	if (!offset_y)
	{

		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | screen_y);

		switch (crossingScreen)
		{
		case 0:
			for (i = 0; i < 16; i++)
				dat_w12864(addr[i]);
			break;
		case 1:
			break;
		case 2:
			for (i = 0; i < 63 - x; i++)
				dat_w12864(addr[i]);
			break;
		default:
			break;
		}

		// 下半部分
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | (screen_y + 1));

		switch (crossingScreen)
		{
		case 0:
			for (i = 0; i < 16; i++)
				dat_w12864(addr[i + 16]);
			break;
		case 1:
			break;
		case 2:
			for (i = 0; i < 63 - x; i++)
				dat_w12864(addr[i + 16]);
			break;
		default:
			break;
		}
	}
	// 否则，一张图片要拆分成3段
	else
	{
		// 先取上面第1段
		uint8 seg1[16] = {0x00};
		uint8 seg2[16] = {0x00};
		uint8 seg3[16] = {0x00};

		// 构造这3段新数据
		for (i = 0; i < 16; i++)
		{
			seg1[i] = seg1[i] | (addr[i] << (8 - offset_y));
			seg2[i] = (addr[i] >> offset_y) | (addr[i + 16] << (8 - offset_y));
			seg3[i] = addr[i + 16] >> offset_y;
		}

		// 先画第一段
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | (screen_y));
		for (i = 0; i < 16; i++)
			dat_w12864(seg1[i]);

		// 再画第二段
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | (screen_y + 1));
		for (i = 0; i < 16; i++)
			dat_w12864(seg2[i]);

		// 再画第三段
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | (screen_y + 2));
		for (i = 0; i < 16; i++)
			dat_w12864(seg3[i]);
	}
}

void clearObject(uint8 x, uint8 y)
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
	cmd_w12864(0xb8 | (y++));

	for (i = 0; i < 16; i++)
		dat_w12864(0x00);

	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | y);

	for (i = 0; i < 16; i++)
		dat_w12864(0x00);
}
