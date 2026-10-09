#include "common.h"
void *func_800A38FC(s32 size);
void *func_800FCA90(void *self, unsigned char value);
void *func_800FCA50(unsigned char value, void *storage) {
    if (storage) return func_800FCA90(storage, value);
    return func_800FCA90(func_800A38FC(0xA0), value);
}
