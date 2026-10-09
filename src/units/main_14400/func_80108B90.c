#include "common.h"
typedef unsigned char u8;
extern void *func_800A38FC(s32);
extern void *func_80108BD0(void *, u8);
void *func_80108B90(u8 value, void *storage) {
    void *result;
    if (!storage) result = func_80108BD0(func_800A38FC(0xA0), value);
    else result = func_80108BD0(storage, value);
    return result;
}
