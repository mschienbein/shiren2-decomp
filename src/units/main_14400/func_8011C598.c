#include "common.h"

void *func_800AC5B4(s32 size, s32 arg1);
void *func_8011C560(void *ptr);
void *func_8011C598(void) {
    return func_8011C560(func_800AC5B4(0x10, 0));
}
