#include "common.h"
typedef struct Object Object;
void func_800E016C(Object *self, s32 flags);
void func_800A3918(Object *self);
void func_800F6B60(Object *self, s32 flags) {
    func_800E016C(self, 0);
    if (flags & 1) func_800A3918(self);
}
