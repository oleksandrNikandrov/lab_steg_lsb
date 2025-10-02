#include "../include/cli/alg_handlers.h"
#include "../include/stego_lsb.h"


#include <string.h>
#include <stdio.h>

Algorithm handlers[] ={
        {"lsb1", lsb1_handler},
        {"lsb2", lsb2_handler},
        {"lsb3", lsb3_handler},
        {"lsb_rand_interval", lsb_rand_interval_handler},
        {NULL, NULL}
};

ERRStatus lsb1_handler(const StegoContext *ctx){
    if (strcmp(ctx->opts.mode, "encode") == 0){
       encode_classic(ctx);

    }

}