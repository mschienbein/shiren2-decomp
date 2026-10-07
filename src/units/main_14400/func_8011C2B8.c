#include "common.h"
extern void *func_800AC5B4(s32 size, s32 flags);
extern void *func_8011C280(void *object);
void *func_8011C2B8(void) {
    return func_8011C280(func_800AC5B4(0x10, 0));
}
