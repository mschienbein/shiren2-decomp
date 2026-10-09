#include "common.h"
extern void func_800F479C(unsigned char *self, s32 flags);
extern void func_800A3918(void *self);
void func_8013614C(unsigned char *self, s32 flags) {
    func_800F479C(self, 0);
    if (flags & 1) {
        func_800A3918(self);
    }
}
