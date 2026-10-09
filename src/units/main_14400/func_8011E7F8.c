#include "common.h"

typedef struct { unsigned char pad[8]; void *vtable; } Object;

void *func_800AC5B4(s32 size, s32 alternate);
Object *func_8011E7C0(Object *obj);

/* Factory entry of table D_801579FC: allocate a 0x10-byte object and construct it. */
Object *func_8011E7F8(void)
{
    return func_8011E7C0(func_800AC5B4(0x10, 0));
}
