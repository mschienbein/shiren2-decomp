#include "common.h"

extern void func_800F479C(void *, s32);
extern void func_800A3918(void *);

void func_800F5298(void *state, s32 flags) {
    func_800F479C(state, 0);
    if (flags & 1) {
        func_800A3918(state);
    }
}
