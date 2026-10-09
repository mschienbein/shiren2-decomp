#include "common.h"
typedef unsigned char u8;
extern void *func_800A38FC(s32 size);
extern void *func_80106CB0(void *object, u8 value);
void *func_80106C70(u8 value, void *storage) {
    if (storage != 0) return func_80106CB0(storage, value);
    else return func_80106CB0(func_800A38FC(0xA0), value);
}
