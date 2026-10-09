#include "common.h"

typedef unsigned char u8;

void func_800F479C(u8 *self, s32 flags);
void func_800A3918(void *obj);

void func_800F59E8(u8 *self, s32 flags)
{
    func_800F479C(self, 0);
    if (flags & 1) {
        func_800A3918(self);
    }
}
