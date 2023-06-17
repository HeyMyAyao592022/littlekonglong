#ifndef __IMGARR_H__
#define __IMGARR_H__

/**
 * @brief 小恐龙的图像
 *
 */
xdata const unsigned char img_konglong[] = {0x00, 0x00, 0x00, 0x00,
                                            0x7C, 0x86, 0x02, 0x01,
                                            0x01, 0x01, 0x19, 0x01,
                                            0x01, 0x19, 0x82, 0xFC,
                                            0x00, 0x20, 0x30, 0x2C,
                                            0xE2, 0x81, 0xE0, 0xA8,
                                            0x28, 0x2A, 0xEE, 0x80,
                                            0xFA, 0x8B, 0x0F, 0x00};

/**
 * @brief 超大恐龙 32*32
 *
 */
xdata const unsigned char img_konglong_large[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x60, 0x60, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                                  0x20, 0x60, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // line1
                                                  0x00, 0x00, 0x00, 0x00, 0x1F, 0x3B, 0x60, 0xC0, 0xC0, 0xC0, 0xC3, 0xC3, 0x00, 0x02, 0xC3, 0xC2,
                                                  0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // line2
                                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 0xFE, 0xFE, 0x00, 0x80, 0x83, 0x06,
                                                  0x86, 0x80, 0x00, 0x00, 0x80, 0x1F, 0x3F, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // line3
                                                  0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x07, 0x07, 0x06, 0x04, 0x07, 0x07,
                                                  0x07, 0x07, 0x06, 0x06, 0x07, 0x05, 0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00};

/**
 * @brief 字模 16*16
 *
 */
xdata const unsigned char img_kai[8] = {0x00, 0x48, 0x3A, 0x0E, 0x0A, 0x0A, 0x3E, 0x08}; // 开
xdata const unsigned char img_shi[8] = {0x00, 0x5E, 0x33, 0x4C, 0x76, 0x4D, 0x4E, 0x76}; // 始

xdata const unsigned char img_ji[8] = {0x00, 0x39, 0x20, 0x00, 0x79, 0x45, 0x45, 0x47}; // 记
xdata const unsigned char img_lu[8] = {0x00, 0x28, 0x36, 0x17, 0x7B, 0x17, 0x2F, 0x48}; // 录

xdata const unsigned char img_jie[32] =
    {0xC0, 0xB0, 0x88, 0x80, 0x40, 0x20, 0xA0, 0xA0, 0xA0, 0xF8, 0xA0, 0xA0, 0xA0, 0x20, 0x00, 0x00,
     0x12, 0x13, 0x0A, 0x0A, 0x0A, 0x00, 0x3E, 0x12, 0x12, 0x12, 0x12, 0x12, 0x3E, 0x00, 0x00, 0x00};
xdata const unsigned char img_shu[32] =
    {0x00, 0x20, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xF0, 0xA0, 0xA0, 0xA0, 0xA0, 0x20, 0x00, 0x00, 0x00,
     0x10, 0x10, 0x13, 0x0A, 0x0A, 0x06, 0x06, 0x1F, 0x06, 0x0A, 0x0A, 0x13, 0x10, 0x00, 0x00, 0x00};

xdata unsigned char img_chong[32] =
    {0x00, 0x20, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xF0, 0xF0, 0xB0, 0xB0, 0xB0, 0xB0, 0xA0, 0x20, 0x00,
     0x00, 0x20, 0x2F, 0x2F, 0x2E, 0x2E, 0x2E, 0x3F, 0x3F, 0x2E, 0x2E, 0x2E, 0x2F, 0x2F, 0x20, 0x00}; /*"重",0*/
xdata unsigned char img_lai[32] =
    {0x00, 0x00, 0x20, 0x60, 0xA0, 0xA0, 0x20, 0xF8, 0xF8, 0x20, 0xA0, 0xA0, 0x60, 0x20, 0x00, 0x00,
     0x00, 0x10, 0x12, 0x0A, 0x0A, 0x06, 0x06, 0x3F, 0x3F, 0x06, 0x06, 0x0A, 0x0A, 0x12, 0x10, 0x00}; /*"来",0*/

xdata unsigned char img_bao[32] =
    {0x00, 0x00, 0xC0, 0xF0, 0x00, 0x00, 0xF0, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xF0, 0x00, 0x00,
     0x00, 0x01, 0x00, 0x3F, 0x20, 0x12, 0x12, 0x0A, 0x06, 0x3F, 0x02, 0x06, 0x0A, 0x12, 0x12, 0x00};
xdata unsigned char img_cun[32] =
    {0x00, 0x00, 0x20, 0xA0, 0x60, 0x30, 0x18, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0x00, 0x00,
     0x00, 0x02, 0x01, 0x3F, 0x00, 0x00, 0x04, 0x24, 0x24, 0x24, 0x3E, 0x05, 0x04, 0x04, 0x00, 0x00};

// xdata unsigned char img_konglong[] =
//     {0x00, 0x80, 0x00, 0x00,
//      0x00, 0x80, 0xC0, 0xC0,
//      0xFE, 0xFF, 0xFF, 0xBF,
//      0xAF, 0x2F, 0x0F, 0x00, // 上半部分
//      0x00, 0x07, 0x07, 0x0E, // 下半部分
//      0xFF, 0xFF, 0x3F, 0x3F,
//      0xFF, 0x07, 0x07, 0x00,
//      0x00, 0x00, 0x00, 0x00};
#endif // __IMGARR_H__