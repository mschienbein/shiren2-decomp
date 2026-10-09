#include "common.h"

typedef unsigned char u8;
extern void *func_800A38FC(s32 size);
extern void *func_80107000(void *object, u8 mode);

void *func_80106FC0(u8 kind, void *storage)
{
    if (storage != 0) {
        return func_80107000(storage, kind);
    } else {
        return func_80107000(func_800A38FC(0xAC), kind);
    }
}
