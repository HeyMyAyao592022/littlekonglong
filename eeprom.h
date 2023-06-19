#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "main.h"

/**
 * @brief 把数据保存到单片机内部eeprom中
 *
 * @param scores
 * @param times
 */
void write_eeprom(uint8 scores, uint8 times);

/**
 * @brief 把数据从单片机内部eeprom中读出来
 *
 * @param scores
 * @param times
 */
void read_eeprom(uint8 *scores, uint8 *times);

/**
 * @brief 开机自检eeprom初始化
 *
 */
void init_eeprom();

#endif // __EEPROM_H__