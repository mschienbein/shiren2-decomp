#include "common.h"
typedef struct Obj Obj;
void *func_800AC5B4(s32 size, s32 alternate);
Obj *func_8011F090(Obj *self);
Obj *func_8011F0C8(void) { return func_8011F090(func_800AC5B4(0x10, 0)); }
