#include "common.h"

typedef unsigned char u8;
void *func_800A38FC(s32);
void *func_800F6CB0(void *, u8);

void *func_800F6F60(u8 kind)
{
    return func_800F6CB0(func_800A38FC(0xB0), kind);
}
