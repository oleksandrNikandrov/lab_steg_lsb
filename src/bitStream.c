#include "util/bitStream.h"

void bs_init(BitStream *bs, uint8_t *pixels, uint32_t total_bits){
    if(pixels == NULL) return;

    bs->pixels = pixels;
    bs->total_bits = total_bits;
    bs->current_bit = 0;
}

void bs_put_bit(BitStream *bs, uint8_t bit){
    if (bs->current_bit >= bs->total_bits) return;
    uint32_t byte_index = bs->current_bit;
    *(bs->pixels + byte_index) = (*(bs->pixels + byte_index) & 0xFE) | bit;
    bs->current_bit++;
}

uint8_t bs_get_bit(BitStream *bs){
    if (bs->current_bit >= bs->total_bits) return 0;
    uint32_t byte_index = bs->current_bit;
    uint8_t bit = *(bs->pixels + byte_index) & 1;
    bs->current_bit++;
    return bit;
}

void bs_reset(BitStream *bs){
    bs->current_bit = 0;
}