#ifndef STEG_LSB_ALG_HANDLERS_H
#define STEG_LSB_ALG_HANDLERS_H

#include <stdint.h>

#include "util/common.h"
#include "util/bmp.h"
#include "util/bitStream.h"
#include "stego_lsb.h"

typedef struct {
    BMPImage img;
    BitStream bs;
    Message msg;
    Options opts;
} StegoContext;

typedef ERRStatus (*steg_foo)(const StegoContext *ctx);

typedef struct {
    const char *name;
    steg_foo handler;
} Algorithm;

ERRStatus lsb1_handler(const StegoContext *ctx);
ERRStatus lsb2_handler(const StegoContext *ctx);
ERRStatus lsb3_handler(const StegoContext *ctx);
ERRStatus lsb_rand_interval_handler(const StegoContext *ctx);

Algorithm handlers[];

#endif //STEG_LSB_ALG_HANDLERS_H
