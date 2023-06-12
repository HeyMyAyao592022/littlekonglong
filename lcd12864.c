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

void drawPicture16(uint8 x, uint8 y, uint8 *addr)
{
	uint8 i;										// 遍历
	uint8 offset;									// 如果图像横跨左右屏幕，可以用于辅助计算
	uint8 *img_ptr = NULL;							// 如果图像横跨左右屏幕，可以用 img_ptr 保存 addr 原始地址
	uint8 crossScreen = (x > 47 && x < 63) ? 1 : 0; // 判断图像是否横跨左右屏幕

	// 横坐标超过 63，就显示到右半屏幕
	if (x < 63)
		choose12864(0);
	else if (x < 112)
	{
		choose12864(1);
		x = x - 64;
	}
	else
		return;

	// 定位到上半部
	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | y);

	// 写入上半部分
	// if 判断是否跨越左右半屏
	if (!crossScreen)
		for (i = 0; i < 16; i++)
			dat_w12864(*addr++);
	else
	{
		// 跨屏幕的话，就先写左屏幕部分
		offset = 63 - x;
		// 用 img_ptr 保存 addr 原始地址
		img_ptr = addr;
		for (i = 0; i < offset; i++)
			dat_w12864(*addr++);
		for (i = 0; i < 8 - offset; i++)
			dat_w12864(0x00);
	}

	// 定位到下半部分
	cmd_w12864(0x40 | x);
	cmd_w12864(0xb8 | (y + 1));

	// 写入下半部分
	// if 判断是否跨越左右半屏
	if (!crossScreen)
		for (i = 0; i < 16; i++)
			dat_w12864(*addr++);
	else
	{
		// 跨屏幕的话，就先写左屏幕部分
		addr = img_ptr + 8;
		for (i = 0; i < offset; i++)
			dat_w12864(*addr++);
		for (i = 0; i < 8 - offset; i++)
			dat_w12864(0x00);
	}

	// 如果跨左右屏幕，那就执行下面的程序
	if (!crossScreen)
		return;

	// // 写右屏幕的部分
	// choose12864(1);
	// x = 0;

	// // 定位到上半部
	// cmd_w12864(0x40);
	// cmd_w12864(0xb8 | y);

	// addr = img_ptr + offset;
	// // 写入上半部分
	// for (i = offset; i < 16; i++)
	// 	dat_w12864(*addr++);

	// // 定位到下半部分
	// cmd_w12864(0x40 | x);
	// cmd_w12864(0xb8 | (y + 1));

	// addr = img_ptr + offset + 8;
	// // 写入下半部分
	// for (i = offset; i < 16; i++)
	// 	dat_w12864(*addr++);
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
