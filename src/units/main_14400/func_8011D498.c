#include "common.h"
extern void *func_800AC5B4(s32 size, s32 flags);
extern void *func_8011D460(void *object);
void *func_8011D498(void) {
    return func_8011D460(func_800AC5B4(0x18, 0));
}
