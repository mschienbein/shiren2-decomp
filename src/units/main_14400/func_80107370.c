#include "common.h"

typedef unsigned char u8;
typedef struct Obj801073B0 Obj801073B0;
extern void *func_800A38FC(s32 size);
extern Obj801073B0 *func_801073B0(Obj801073B0 *object, u8 variant);

Obj801073B0 *func_80107370(u8 variant, Obj801073B0 *storage)
{
    if (storage)
        return func_801073B0(storage, variant);
    return func_801073B0(func_800A38FC(0xAC), variant);
}
