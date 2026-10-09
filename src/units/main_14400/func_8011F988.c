#include "common.h"

typedef struct { s32 field_0; s32 field_4; void *vtable_8; } Obj;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Obj *func_8011F950(Obj *obj);

Obj *func_8011F988(void)
{
    return func_8011F950(func_800AC5B4(0x10, 0));
}
