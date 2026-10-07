#include "common.h"

typedef unsigned char u8;

extern void *func_800A38FC(s32 id);
extern void *func_800F86B0(void *obj, u8 value);

void *func_800F8720(u8 value) {
    return func_800F86B0(func_800A38FC(0x80), value);
}
