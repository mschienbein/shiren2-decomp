#include "common.h"

extern void func_8011414C(void *self, s32 flags);
extern void func_800AC68C(void *a);

void func_801212CC(void *self, s32 flags) {
    func_8011414C(self, 0);
    if (flags & 1) {
        func_800AC68C(self);
    }
}
