#ifndef STEG_LSB_BITSTREAM_H
#define STEG_LSB_BITSTREAM_H

#include <stdint.h>

typedef struct {
    uint8_t *pixels;
    uint32_t total_bits;
    uint32_t current_bit;
}BitStream;

void bs_init (BitStream *bs, uint8_t *pixels, uint32_t total_bits);
void bs_put_bit(BitStream *bs, uint8_t bit);
uint8_t bs_get_bit(BitStream *bs);
void bs_reset (BitStream *bs);

#endif //STEG_LSB_BITSTREAM_H
