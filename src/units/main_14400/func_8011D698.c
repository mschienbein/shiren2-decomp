#include "common.h"

extern void *func_800AC5B4(s32, s32);
extern void *func_8011D660(void *);

void *func_8011D698(void) {
    return func_8011D660(func_800AC5B4(0x18, 0));
}
