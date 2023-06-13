#ifndef _MAIN_H_
#define _MAIN_H_

// #include <AT89X55.H>
// #include <REGX52.H>

#include <reg52.H>
#include <intrins.h>
#include <math.h>
#include <stddef.h>

#define DIR_LEFT 0
#define DIR_RIGHT 1
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_STILL -1

#define BLCOK_MAX_Y 55
#define KONGLONG_MAX_X 111
#define KONGLONG_MAX_Y 48

typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;
typedef char int8;
typedef int int16;

sbit RS = P1 ^ 0;
sbit RW = P1 ^ 1;
sbit E = P1 ^ 2;
sbit CS1 = P1 ^ 3;
sbit CS2 = P1 ^ 4;

sbit SCK = P1 ^ 5;
sbit SDA = P1 ^ 6;
sbit RST = P1 ^ 7; // DS1302

sbit KeyIn1 = P3 ^ 4;
sbit KeyIn2 = P3 ^ 5;
sbit KeyIn3 = P3 ^ 6;

sbit BEEP = P3 ^ 3; //

void delay(uint16 n); //

#endif
