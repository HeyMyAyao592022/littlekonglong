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

void drawPicture16(uint8 x, uint8 y, uint8 *addr, int8 dir_horizon, int8 dir_vertical)
{
	uint8 i;
	uint8 crossedScreen = x == 63 ? 1 : 0;
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
	else
		choose12864(0);

	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | y);

	if ((y & 0x80) == 0)
	{
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
	}
	else
		for (i = 0; i < 16; i++)
			dat_w12864(0xFF - addr[i]);

	// 下半部分
	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | (y + 1));

	if ((y & 0x80) == 0)
	{
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
	else
		for (i = 0; i < 16; i++)
			dat_w12864(0xFF - addr[i + 16]);

	// 横跨左右屏幕
	if (crossedScreen)
	{
		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | y);

		for (i = 0; i < 16; i++)
			dat_w12864(0x00);

		cmd_w12864(0x40 | x);
		cmd_w12864(0xb8 | (y + 1));

		for (i = 0; i < 16; i++)
			dat_w12864(0x00);
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
