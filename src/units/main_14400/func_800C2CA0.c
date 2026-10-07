#include "common.h"

typedef unsigned char u8;

extern u8 D_80149E40[];
void func_800C2CDC(u8 *self, void *position, void *direction, s32 limit, s32 mode);
void *func_800C2CA0(u8 *self, void *position, void *direction, s32 limit, s32 mode) {
    *(void **)(self + 0x1C) = D_80149E40;
    func_800C2CDC(self, position, direction, limit, mode);
    return self;
}
