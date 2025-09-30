#ifndef STEG_LSB_BMP_H
#define STEG_LSB_BMP_H

#include <stdint.h>

#include "utils.h"

#pragma pack(push, 1)
typedef struct{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BMPHeader;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} DIBHeader;

#pragma pack(pop)

typedef struct {
    BMPHeader bheader;
    DIBHeader dheader;
    uint8_t *pixels;

    int32_t row_size;
    int32_t data_size;
} BMPImage;

ERRStatus bmp_load(const char *filename, BMPImage *img);
ERRStatus bmp_save(const char *filename, BMPImage *img);

ERRStatus bmp_free(BMPImage *img);

ERRStatus bmp_get_pixels(const BMPImage *img, int x, int y, uint8_t *r, uint8_t *g, uint8_t *b);
ERRStatus bmp_set_pixels(const BMPImage *img, int x, int y, uint8_t *r, uint8_t *g, uint8_t *b);

#endif //STEG_LSB_BMP_H
