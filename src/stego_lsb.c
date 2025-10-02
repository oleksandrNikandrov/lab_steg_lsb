#include <stdio.h>
#include <stdlib.h>

#include "../include/stego_lsb.h"

// stegocontext *ctx передаем сtx во все случаи
ERRStatus encode_classic(StegoContext *ctx){
    if (NULL == &(ctx->bs)){
        LOG_ERROR("bit stream is uninitialized");
        return MEMORY_ERR;
    }else if (NULL == ctx->bs.pixels){
        LOG_ERROR("pixels array is uninitialized");
        return MEMORY_ERR;
    }

    bs_reset(&(ctx->bs));

    for(int i = 31; i >= 0; i--){
        bs_put_bit(&(ctx->bs), ((ctx->msg.data_size >> i) & 1));
    }

    for(uint32_t i = 0; i < ctx->msg.data_size; i++){
        for(int8_t j = 7; j >= 0; j--){
            bs_put_bit(bs, (*(ctx->msg.data + i) >> j) & 1);
        }
    }

    return OK_STATUS;
}

ERRStatus decode_classic(StegoContext *ctx){
    if (NULL == &(ctx->bs)){
        LOG_ERROR("bit stream is uninitialized");
        return MEMORY_ERR;
    }
    else if (NULL == ctx->bs.pixels){
        LOG_ERROR("pixels array is uninitialized");
        return MEMORY_ERR;
    }

    bs_reset(&(ctx->bs));

    for(int8_t i = 31; i >= 0; i--){
        ctx->msg.data_size |= (bs_get_bit(bs) << i);
    }

    ctx->msg.data = malloc(sizeof(ctx->msg->data_size + 1)); // \0
    if(!ctx->msg.data) {
        LOG_ERROR("occured while allocating memory. ");
        return MEMORY_ERR;
    }

    for(uint32_t i = 0; i < ctx->msg.data_size; i++){
        uint8_t byte = 0;
        for(int8_t j = 7; j >= 0; j--){
            byte |= (bs_get_bit(&(ctx->bs)) << j);
        }
        *(ctx->msg.data + i) = byte;
    }

    ctx->msg.data[ctx->msg.data_size] = '\0';

    return OK_STATUS;
}
