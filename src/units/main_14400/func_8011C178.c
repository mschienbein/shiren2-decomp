#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

void *func_800AC5B4(s32 size, s32 arg1);
void *func_8011C140(void *obj);

void *func_8011C178(void) {
    return func_8011C140(func_800AC5B4(0x10, 0));
}
