/*
 * DS1302 时钟芯片
 */
#include "main.h"
#include "DS1302.h" //包含头文件

/*
 * 写一个字节
 */
void write_ds1302_byte(uint8 dat)
{
    uint8 i;
    for (i = 0; i < 8; i++) // 循环8次
    {
        SDA = dat & 0x01;
        SCK = 1; // SCK端口置1
        dat >>= 1;
        SCK = 0; // SCK端口置0
    }
}

/*
 * 读一个字节
 */
uint8 read_ds1302_byte(void)
{
    uint8 i, dat = 0;

    for (i = 0; i < 8; i++) // 循环8次
    {
        dat >>= 1;
        if (SDA)
            dat |= 0x80;
        SCK = 1; // SCK端口置1
        SCK = 0; // SCK端口置0
    }

    return dat;
}

void reset_ds1302(void)
{
    RST = 0; // RST端口置0
    SCK = 0; // SCK端口置0
    RST = 1; // RST端口置1
}

/*
 * 清除写保护
 */
void clear_ds1302_WP(void)
{
    reset_ds1302();
    RST = 1; // RST端口置1
    write_ds1302_byte(0x8E);
    write_ds1302_byte(0);
    SDA = 0; // SDA端口置0
    RST = 0; // RST端口置0
}

/*
 * 设置写保护
 */
void set_ds1302_WP(void)
{
    reset_ds1302(); // 复位1302
    RST = 1;        // RST端口置1
    write_ds1302_byte(0x8E);
    write_ds1302_byte(0x80);
    SDA = 0; // SDA端口置0
    RST = 0; // RST端口置0
}

/*
 * 设定时钟数据 （秒分时日月周年）
 */
void set_time(void)
{
    uint8 i, tmp, tmps[7];

    for (i = 0; i < 7; i++) // 转化为BCD格式
    {
        tmp = 0;
        tmps[i] = 0;
        tmps[i] = 0;
    }

    clear_ds1302_WP(); // 取消写保护

    reset_ds1302(); // 复位芯片
    RST = 1;        // RST端口置1
    write_ds1302_byte(DS1302_W_ADDR);
    for (i = 0; i < 7; i++) // 循环7次
    {
        write_ds1302_byte(tmps[i]);
        delay(10); // 延时
    }
    write_ds1302_byte(0);
    SDA = 0; // SDA端口置0
    RST = 0; // RST端口置0

    set_ds1302_WP(); // 设置写保护
}

/*
 * 读时钟数据（秒分时日月周年）
 */
void read_time(uint8 *timedata)
{
    uint8 i, tmp;

    clear_ds1302_WP(); // 取消写保护

    reset_ds1302(); // 复位芯片
    RST = 1;        // RST端口置1
    write_ds1302_byte(DS1302_R_ADDR);
    for (i = 0; i < 7; i++) // 循环2次
    {
        timedata[i] = read_ds1302_byte();
        delay(10); // 延时
    }
    SDA = 0; // SDA端口置0
    RST = 0; // RST端口置0

    set_ds1302_WP(); // 设置写保护

    for (i = 0; i < 7; i++) // 循环2次
    {
        tmp = timedata[i];
        timedata[i] = (tmp / 16 % 10) * 10;
        timedata[i] += (tmp % 16);
    }
}
