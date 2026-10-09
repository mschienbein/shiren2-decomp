#include "common.h"
typedef struct { unsigned char pad00[4]; const void *vtable; } Object;
extern const unsigned char D_80157FA8[];
extern void func_800D8FE8(void *object);
void func_800DB4B4(Object *object, s32 flags) {
    object->vtable = D_80157FA8;
    if (flags & 1) func_800D8FE8(object);
}
