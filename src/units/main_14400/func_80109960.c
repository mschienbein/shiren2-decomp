#include "common.h"

typedef unsigned char u8;

extern void *func_800A38FC(s32 size);
extern void *func_80109870(void *obj, u8 arg1);

void *func_80109960(u8 arg0) {
    return func_80109870(func_800A38FC(0xC4), arg0);
}
