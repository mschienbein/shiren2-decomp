#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0xC]; u8 value_0C; } Object80111608;
extern u8 D_801576E8[];
extern u8 D_80147620[];
u8 func_800AE98C(u8 *object);
s32 func_800C5844(void *rng, u8 base, u8 top);
void func_80111608(Object80111608 *object)
{
    u8 bounds = D_801576E8[func_800AE98C((u8 *)object)];
    object->value_0C = func_800C5844(D_80147620, bounds & 0xF, bounds >> 4);
}
