#include "common.h"

void func_800F479C(void *obj, s32 flags);
void func_800A3918(void *obj);

void func_801362FC(void *obj, s32 flags) {
    func_800F479C(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
