#include "common.h"

typedef unsigned char u8;
void *func_800A38FC(s32);
void *func_8010B220(void *, u8);
void *func_8010B34C(u8 arg0) {
    return func_8010B220(func_800A38FC(0xE4), arg0);
}
