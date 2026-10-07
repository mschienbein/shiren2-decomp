#include "common.h"

typedef unsigned char u8;
extern void *func_800CF058(void *target, u8 arg1);

void *func_800CFB08(void *target) {
    return func_800CF058(target, 4);
}
