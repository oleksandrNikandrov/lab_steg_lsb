#include <stdio.h>
#include <stdlib.h>

#include "../include/stego_lsb.h"
#include "./cli/alg_handlers.h"

ERRStatus encode_classic(BitStream *bs, Message *msg){
    if (NULL == bs){
        LOG_ERROR("bit stream is uninitialized");
        return MEMORY_ERR;
    }else if (NULL == bs->pixels){
        LOG_ERROR("pixels array is uninitialized");
        return MEMORY_ERR;
    }

    bs_reset(bs);

    for(int i = 31; i >= 0; i--){
        bs_put_bit(bs, ((msg->data_size >> i) & 1));
    }

    for(uint32_t i = 0; i < msg->data_size; i++){
        for(int8_t j = 7; j >= 0; j--){
            bs_put_bit(bs, (*(msg->data + i) >> j) & 1);
        }
    }

    return OK_STATUS;
}

ERRStatus decode_classic(BitStream *bs, Message *msg){
    if (NULL == bs){
        LOG_ERROR("bit stream is uninitialized");
        return MEMORY_ERR;
    }
    else if (NULL == bs->pixels){
        LOG_ERROR("pixels array is uninitialized");
        return MEMORY_ERR;
    }

    bs_reset(bs);

    for(int8_t i = 31; i >= 0; i--){
        msg->data_size |= (bs_get_bit(bs) << i);
    }

    msg->data = malloc(sizeof(msg->data_size + 1)); // \0
    if(!msg->data) {
        LOG_ERROR("occured while allocating memory. ");
        return MEMORY_ERR;
    }

    for(uint32_t i = 0; i < msg->data_size; i++){
        uint8_t byte = 0;
        for(int8_t j = 7; j >= 0; j--){
            byte |= (bs_get_bit(bs) << j);
        }
        *(msg->data + i) = byte;
    }

    msg->data[msg->data_size] = '\0';

    return OK_STATUS;
}
