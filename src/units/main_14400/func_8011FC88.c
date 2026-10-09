#include "common.h"

void *func_800AC5B4(s32 size, s32 alternate);
/* Constructor: initializes the 0x10-byte object and returns it. */
void *func_8011FC50(void *obj);

/* Factory slot (D_801579FC table): allocate and construct one object. */
void *func_8011FC88(void)
{
    return func_8011FC50(func_800AC5B4(0x10, 0));
}
