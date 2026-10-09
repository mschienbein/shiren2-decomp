#include "common.h"
extern void *func_800A38FC(s32 size);
extern void *func_80107DB0(void *object, unsigned char index);
void *func_80107D70(unsigned char index, void *storage) {
    if (storage != 0) return func_80107DB0(storage, index);
    return func_80107DB0(func_800A38FC(0xC8), index);
}
