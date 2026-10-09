#include "common.h"

extern void *func_800AC5B4(s32, s32);
extern void *func_8011E250(void *);
void *func_8011E288(void) { return func_8011E250(func_800AC5B4(0x10, 0)); }
