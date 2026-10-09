#include "common.h"

typedef struct Obj Obj;

extern void *func_800AC5B4(s32 size, s32 alternate);
extern Obj *func_8011F6C0(Obj *o);

Obj *func_8011F6F8(void)
{
    return func_8011F6C0(func_800AC5B4(0x10, 0));
}
