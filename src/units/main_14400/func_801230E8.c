#include "common.h"
typedef struct Obj Obj;
extern void func_8011414C(Obj *self, s32 flags);
extern void func_800AC68C(void *a);
void func_801230E8(Obj *object, s32 flags) {
    func_8011414C(object, 0);
    if (flags & 1) func_800AC68C(object);
}
