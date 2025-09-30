#include <stdio.h>
#include <stdlib.h>

#include "../include/bmp.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemoryLeak"

ERRStatus bmp_load(const char *filename, BMPImage **img){
    FILE *f = fopen(filename, "rb");
    if( !f) return ERR_FILE_NOT_FOUND;

    (*img) = malloc(sizeof(BMPImage));
    if(!(*img)) return MEMORY_ERR;
    // читаем заголовкоув
    if(fread(&(*img)->bheader, sizeof(BMPHeader), 1, f) != 1){
        if(feof(f)) {
            LOG_ERROR("Unexpected end of file");
            return ERR_INVALID_FORMAT;
        }
        else if(ferror(f)) LOG_ERROR("Reading file failure");
        fclose(f);
        return IO_ERROR;
    }
    if(fread(&(*img)->dheader, sizeof(DIBHeader), 1, f) != 1){
        if(feof(f)) LOG_ERROR("Unexpected end of file");
        else if(ferror(f)) LOG_ERROR("Reading file failure");
        fclose(f);
        return IO_ERROR;
    }

    if((*img)->dheader.biBitCount != 24) return ERR_INVALID_FORMAT;
    if((*img)->dheader.biCompression != 0) return ERR_INVALID_FORMAT;

    //вычисления размера массива пикселей с учетом пагинации до 4 байт ( DWORD )
    (*img)->row_size = (((*img)->dheader.biBitCount * (*img)->dheader.biWidth + 31) / 32 ) * 4;
    (*img)->data_size = (*img)->row_size * abs((*img)->dheader.biHeight);
    (*img)->pixels = malloc((*img)->data_size);
    if(!(*img)->pixels) return MEMORY_ERR;

    fseek(f, (*img)->bheader.bfOffBits, SEEK_SET);
    if(fread((*img)->pixels,1, (*img)->data_size, f) != (*img)->data_size){
        if(feof(f)) LOG_ERROR("Unexpected end of file");
        else if(ferror(f)) LOG_ERROR("Reading file failure");
        fclose(f);
        return IO_ERROR;
    }

    fclose(f);
    return OK_STATUS;
}
#pragma clang diagnostic pop

ERRStatus bmp_save(const char *filename, BMPImage **img){
    if(!(*img)) {
        LOG_ERROR("img pointer is NULL");
        return MEMORY_ERR;
    }

    FILE *f = fopen(filename, "wb");
    if(!f) {
        LOG_ERROR("such file not found");
        return ERR_FILE_NOT_FOUND;
    }

    if (fwrite(&(*img)->bheader, sizeof(BMPHeader), 1, f) != 1){
        LOG_ERROR("writing file failure");
        fclose(f);
        return IO_ERROR;
    }

    if (fwrite(&(*img)->dheader, sizeof(DIBHeader), 1, f) != 1){
        LOG_ERROR("writing file failure");
        fclose(f);
        return IO_ERROR;
    }

    uint32_t wrote_bytes = fwrite((*img)->pixels, 1, (*img)->data_size, f);

    if ( wrote_bytes != (*img)->data_size ){
        LOG_ERROR("writing file failure");
        fclose(f);
        return IO_ERROR;
    }

    if(fclose(f) != 0){
        LOG_ERROR("failure occurs while quitting file");
        return IO_ERROR;
    }

    fclose(f);
    return OK_STATUS;
}

ERRStatus bmp_get_pixel(const BMPImage *img, int x, int y, uint8_t *r, uint8_t *g, uint8_t *b){
    int32_t bytes_per_pixel = img->dheader.biBitCount / 8;

    if (x < 0 || x > img->dheader.biWidth || y < 0 || y > abs(img->dheader.biHeight)){
        LOG_ERROR("invalid coordinates for getting pixel");
        return ERR_INVALID_FORMAT;
    }

    int32_t row = abs(img->dheader.biHeight) - 1 - y;
    int32_t idx = row * img->row_size + x * 3;

    *b = img->pixels[idx];
    *g = img->pixels[idx+1];
    *r = img->pixels[idx+2];

    return OK_STATUS;
}

ERRStatus bmp_set_pixel(const BMPImage *img, int x, int y, const uint8_t r, const uint8_t g, const uint8_t b){
    int32_t bytes_per_pixel = img->dheader.biBitCount / 8;

    if (x < 0 || x > img->dheader.biWidth || y < 0 || y > abs(img->dheader.biHeight)){
        LOG_ERROR("invalid coordinates for getting pixel");
        return ERR_INVALID_FORMAT;
    }

    int32_t row = abs(img->dheader.biHeight) - 1 - y;
    int32_t idx = row * img->row_size + x * 3;

    img->pixels[idx] = b;
    img->pixels[idx + 1] = g;
    img->pixels[idx + 2] = r;

    return OK_STATUS;
}

ERRStatus bmp_free(BMPImage *img){
    if(!img) return OK_STATUS;
    free(img->pixels);
    free(img);
}