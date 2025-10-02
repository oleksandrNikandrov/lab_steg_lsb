#ifndef STEG_LSB_STEGO_LSB_H
#define STEG_LSB_STEGO_LSB_H

#include <stdint.h>

#include "util/common.h"
#include "./cli/alg_handlers.h"


typedef struct {
    uint8_t *data;
    uint32_t data_size;
} Message;

ERRStatus encode_classic(StegoContext *ctx);
ERRStatus decode_classic(StegoContext *ctx);

#endif //STEG_LSB_STEGO_LSB_H
