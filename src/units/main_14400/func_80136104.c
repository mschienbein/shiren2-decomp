#include "common.h"

typedef unsigned char u8;
extern void func_800F479C(u8 *self, s32 flags);
extern void func_800A3918(void *object);

void func_80136104(u8 *object, s32 flags)
{
    func_800F479C(object, 0);
    if (flags & 1) {
        func_800A3918(object);
    }
}
