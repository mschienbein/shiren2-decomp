#include "common.h"
typedef unsigned char u8;
typedef struct { void *pool_0; void *field_4; } Object;
extern u8 D_80154300[];
extern void func_800D8FA8(void *object);
void func_800CFEBC(Object *object, s32 flags)
{
    object->field_4 = D_80154300;
    if (flags & 1) {
        func_800D8FA8(object);
    }
}
