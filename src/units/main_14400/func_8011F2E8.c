#include "common.h"
typedef struct Obj Obj;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Obj *func_8011F2B0(Obj *obj);
Obj *func_8011F2E8(void) {
    return func_8011F2B0(func_800AC5B4(20, 0));
}
