#include "common.h"

typedef struct Obj Obj;

void *func_800AC5B4(s32 size, s32 alternate);
Obj *func_8011F1B0(Obj *obj);

/* Factory entry of table D_801579FC: allocate a 0x10-byte object and construct it. */
Obj *func_8011F1E8(void) {
    return func_8011F1B0(func_800AC5B4(0x10, 0));
}
