#include <stdio.h>

typedef unsigned char uint8;

unsigned char addr[] =
    {0xff, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, // 上半部分
     0xff, 0x00, 0x00, 0x00, // 下半部分
     0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00};

int main()
{
    // uint8 offset_y = 2;

    // // 先取上面第1段
    // uint8 seg1[16] = {0x00};
    // uint8 seg2[16] = {0x00};
    // uint8 seg3[16] = {0x00};

    // // 构造这3段新数据
    // for (uint8 i = 0; i < 16; i++)
    // {
    //     seg1[i] = seg1[i] | (addr[i] << (8 - offset_y));
    //     seg2[i] = (addr[i] >> offset_y) | (addr[i + 16] << (8 - offset_y));
    //     seg3[i] = addr[i + 16] >> offset_y;
    // }

    // for (uint8 i = 0; i < 16; i++)
    // {
    //     printf("%hx\t", seg1[i]);
    // }
    // printf("\n");

    // for (uint8 i = 0; i < 16; i++)
    // {
    //     printf("%hx\t", seg2[i]);
    // }
    // printf("\n");

    // for (uint8 i = 0; i < 16; i++)
    // {
    //     printf("%hx\t", seg3[i]);
    // }
    // printf("\n");

    unsigned char sender = 0x01;
    unsigned char receiver = 0x00;

    receiver = receiver | (sender << 6);

    printf("%hx \n", receiver);

    return 0;
}