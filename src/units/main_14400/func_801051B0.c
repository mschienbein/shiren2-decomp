#include "common.h"
extern void *func_800A38FC(s32 size);
extern void *func_801051F0(void *object, unsigned char kind);
void *func_801051B0(unsigned char kind, void *storage) {
    if (storage) return func_801051F0(storage, kind);
    return func_801051F0(func_800A38FC(0xA4), kind);
}
