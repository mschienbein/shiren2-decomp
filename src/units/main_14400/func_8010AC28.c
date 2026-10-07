#include "common.h"

typedef unsigned char u8;

void *func_800A38FC(s32 id);
void *func_8010A7D0(void *arg0, u8 arg1);

void *func_8010AC28(u8 arg0) {
    return func_8010A7D0(func_800A38FC(0xD4), arg0);
}
