#include "common.h"
typedef unsigned char u8;
extern void *func_800A38FC(s32 size);
extern void *func_80107800(void *obj, u8 arg);
void *func_801077C0(u8 kind, void *memory) {
    if (memory != 0) return func_80107800(memory, kind);
    else return func_80107800(func_800A38FC(0xA0), kind);
}
