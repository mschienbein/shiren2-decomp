#include "common.h"

typedef struct S8011FFB0 S8011FFB0;

extern void *func_800AC5B4(s32 size, s32 alternate);
extern S8011FFB0 *func_8011FFB0(S8011FFB0 *s);

/* Allocate a 12-byte object and construct it. */
S8011FFB0 *func_8011FFE8(void) {
    return func_8011FFB0(func_800AC5B4(0xC, 0));
}
