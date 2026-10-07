#include "common.h"
typedef unsigned char u8;
typedef struct { char pad[0x1E]; u8 flags; } Obj;
Obj *func_800C5F60(void);
s32 func_800A6FD0(Obj *self) {
    s32 r = 0;
    if ((self->flags >> 2) & 1 || func_800C5F60() == self) r = 1;
    return r;
}
