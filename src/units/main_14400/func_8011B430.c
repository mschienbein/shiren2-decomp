#include "common.h"

extern void *func_800AC5B4(s32, s32);
extern void *func_8011B3F0(void *);

void *func_8011B430(void) {
    return func_8011B3F0(func_800AC5B4(0x10, 0));
}
