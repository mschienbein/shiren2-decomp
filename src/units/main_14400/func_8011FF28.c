#include "common.h"

typedef struct Obj Obj;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Obj *func_8011FEF0(Obj *o);

Obj *func_8011FF28(void) {
    return func_8011FEF0(func_800AC5B4(12, 0));
}
