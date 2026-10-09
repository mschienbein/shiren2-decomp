#include "common.h"
typedef struct { unsigned char pad0[4]; void *vtable4; } Object;
extern unsigned char D_80157FA8[];
extern void func_800D8FE8(void *object);
void func_800DA5EC(Object *object, s32 flags) {
    object->vtable4 = D_80157FA8;
    if (flags & 1) func_800D8FE8(object);
}
