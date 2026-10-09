#include "common.h"
typedef struct { s32 field_00; const void *field_04; } Object;
extern const unsigned char D_80157FA8[];
extern void func_800D8FE8(void *object);
void func_800DA4F8(Object *object, s32 flags) {
    object->field_04 = D_80157FA8;
    if (flags & 1) func_800D8FE8(object);
}
