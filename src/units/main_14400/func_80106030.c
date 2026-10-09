#include "common.h"
typedef unsigned char u8;
extern void *func_800A38FC(s32 size);
extern void *func_80106070(void *self, u8 kind);
void *func_80106030(u8 kind, void *memory) {
    if (memory != 0) return func_80106070(memory, kind);
    return func_80106070(func_800A38FC(0xA0), kind);
}
