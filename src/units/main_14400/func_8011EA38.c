#include "common.h"

typedef struct Obj Obj;

void *func_800AC5B4(s32 size, s32 alternate);
Obj *func_8011EA00(Obj *obj);

Obj *func_8011EA38(void)
{
    return func_8011EA00(func_800AC5B4(0x10, 0));
}
