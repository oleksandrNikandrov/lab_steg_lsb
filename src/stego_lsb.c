#include <stdio.h>

#include "../include/stego_lsb.h"
#include "../include/bitStream.h"

ERRStatus encode_classic(BitStream *bs, const uint8_t *data, uint32_t data_size){
    if (NULL == bs){
        LOG_ERROR("bit stream is uninitialized");
        return MEMORY_ERR;
    }else if (NULL == bs->pixels){
        LOG_ERROR("pixels array is uninitialized");
        return MEMORY_ERR;
    }

    bs_reset(bs);

    for(int i = 31; i >= 0; i--){
        bs_put_bit(bs, ((data_size >> i) & 1));
    }

    for(uint32_t i = 0; i < data_size; i++){
        for(int8_t j = 7; j >= 0; j--){
            bs_put_bit(bs, (*(data + i) >> j) & 1);
        }
    }

    return OK_STATUS;
}

ERRStatus decode_classic(BitStream *bs, uint8_t *data){
    if (NULL == bs){
        LOG_ERROR("bit stream is uninitialized");
        return MEMORY_ERR;
    }
    else if (NULL == bs->pixels){
        LOG_ERROR("pixels array is uninitialized");
        return MEMORY_ERR;
    }

    bs_reset(bs);

    uint32_t data_size = 0;
    for(int8_t i = 31; i >= 0; i--){
        data_size |= (bs_get_bit(bs) << i);
    }

    for(uint32_t i = 0; i < data_size; i++){
        uint8_t byte = 0;
        for(int8_t j = 7; j >= 0; j--){
            byte |= (bs_get_bit(bs) << j);
        }
        *(data + i) = byte;
    }

    return OK_STATUS;
}
