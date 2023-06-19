#ifndef __DS1302_H__
#define __DS1302_H__

typedef struct Time
{
    char sec;
    char min;
    char hour;
    char day;
    char mon;
    char week;
    char year;
} Time;

#define DS1302_W_ADDR 0xBE
#define DS1302_R_ADDR 0xBF

void set_time(void);
void read_time(unsigned char *timedata);

#endif // __DS1302_H__