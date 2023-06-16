#ifndef __IMGARR_H__
#define __IMGARR_H__

// xdata unsigned char img_konglong[] =
//     {0x00, 0x80, 0x00, 0x00,
//      0x00, 0x80, 0xC0, 0xC0,
//      0xFE, 0xFF, 0xFF, 0xBF,
//      0xAF, 0x2F, 0x0F, 0x00, // 上半部分
//      0x00, 0x07, 0x07, 0x0E, // 下半部分
//      0xFF, 0xFF, 0x3F, 0x3F,
//      0xFF, 0x07, 0x07, 0x00,
//      0x00, 0x00, 0x00, 0x00};

/**
 * @brief 小恐龙的图像
 *
 */
xdata unsigned char img_konglong[] = {0x00, 0x00, 0x00, 0x00,
                                      0x7C, 0x86, 0x02, 0x01,
                                      0x01, 0x01, 0x19, 0x01,
                                      0x01, 0x19, 0x82, 0xFC,
                                      0x00, 0x20, 0x30, 0x2C,
                                      0xE2, 0x81, 0xE0, 0xA8,
                                      0x28, 0x2A, 0xEE, 0x80,
                                      0xFA, 0x8B, 0x0F, 0x00};

// unsigned char rec_frameless[] =
//     {0xff, 0x01, 0x01, 0x01,
//      0x01, 0x01, 0x01, 0x01,
//      0x01, 0x01, 0x01, 0x01,
//      0x01, 0x01, 0x01, 0xff, // 上半部分
//      0xff, 0x80, 0x80, 0x80, // 下半部分
//      0x80, 0x80, 0x80, 0x80,
//      0x80, 0x80, 0x80, 0x80,
//      0x80, 0x80, 0x80, 0xff};

// code unsigned char rec_frame[] =
//     {0xff, 0xff, 0xff, 0xff,
//      0xff, 0xff, 0xff, 0xff,
//      0xff, 0xff, 0xff, 0xff,
//      0xff, 0xff, 0xff, 0xff, // 上半部分
//      0xff, 0xff, 0xff, 0xff, // 下半部分
//      0xff, 0xff, 0xff, 0xff,
//      0xff, 0xff, 0xff, 0xff,
//      0xff, 0xff, 0xff, 0xff};

#endif // __IMGARR_H__