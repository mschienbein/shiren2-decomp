#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad0[0x90];
    short adjust90;
    short pad92;
    /* D_80158C98 and D_80159320 slot 0x94 target func_800E115C. */
    s32 (*field94)(void *, s32, s32, u8, s32);
} VTable;
typedef struct { u8 pad0[0x24]; VTable *field24; } Object;

s32 func_800E2DCC(Object *self)
{
    return self->field24->field94((u8 *)self + self->field24->adjust90,
                                 0, 8, 0xFE, 0);
}
