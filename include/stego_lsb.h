#ifndef STEG_LSB_STEGO_LSB_H
#define STEG_LSB_STEGO_LSB_H

#include <stdint.h>

#include "utils.h"
#include "bitStream.h"

ERRStatus encode_classic(BitStream *bs, const uint8_t *data, uint32_t data_size);
ERRStatus decode_classic(BitStream *bs, uint8_t *data);

#endif //STEG_LSB_STEGO_LSB_H
