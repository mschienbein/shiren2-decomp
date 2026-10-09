#include "common.h"

typedef unsigned char u8;

void func_800F479C(u8 *self, s32 flags);
void func_800A3918(u8 *obj);

/* Destructor: run the base destructor, release the pool block when bit 0 is set. */
void func_800F5E74(u8 *self, s32 flags)
{
    func_800F479C(self, 0);
    if (flags & 1) {
        func_800A3918(self);
    }
}
