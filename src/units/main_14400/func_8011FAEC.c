#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj;

void *func_800AC5B4(s32 size, s32 alternate);
Obj *func_8011FA20(Obj *self);

/* Factory-table entry: allocate a 0x10-byte object and construct it. */
Obj *func_8011FAEC(void)
{
    return func_8011FA20(func_800AC5B4(0x10, 0));
}
