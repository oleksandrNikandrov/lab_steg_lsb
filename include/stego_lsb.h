#ifndef STEG_LSB_STEGO_LSB_H
#define STEG_LSB_STEGO_LSB_H

#include <stdint.h>

#include "util/common.h"
#include "util/bitStream.h"

typedef struct {
    uint8_t *data;
    uint32_t data_size;
} Message;

ERRStatus encode_classic(BitStream *bs, Message *msg);
ERRStatus decode_classic(BitStream *bs, Message *msg);

#endif //STEG_LSB_STEGO_LSB_H
