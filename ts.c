#include <stdio.h>
#include <math.h>

typedef unsigned char uint8;

int main()
{
    uint8 a = 255;
    int b = -127;
    printf(" -> %hx \n", a == b);
    return 0;
}