#include "common.h"
void *func_800AC5B4(s32 size, s32 b);
void *func_8011B930(void *p);
void *func_8011B970(void) {
    return func_8011B930(func_800AC5B4(0x18, 0));
}
