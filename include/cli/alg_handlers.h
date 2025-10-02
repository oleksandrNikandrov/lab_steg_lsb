#ifndef STEG_LSB_ALG_HANDLERS_H
#define STEG_LSB_ALG_HANDLERS_H

#include <stdint.h>

#include "util/common.h"

typedef ERRStatus (*steg_foo)(int32_t argc, char **argv);

typedef struct {
    const char *name;
    steg_foo handler;
} Algorithm;

Algorithm handlers[];

#endif //STEG_LSB_ALG_HANDLERS_H
