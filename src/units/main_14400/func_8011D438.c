#include "common.h"
extern void *func_800AC5B4(s32 size, s32 flags);
extern void *func_8011D400(void *obj);
void *func_8011D438(void)
{
    return func_8011D400(func_800AC5B4(0x18, 0));
}
