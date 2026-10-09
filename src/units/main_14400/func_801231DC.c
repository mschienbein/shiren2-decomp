#include "common.h"
extern void func_8011414C(void *object, s32 flags);
extern void func_800AC68C(void *object);
void func_801231DC(void *object, s32 flags) {
    func_8011414C(object, 0);
    if (flags & 1) func_800AC68C(object);
}
