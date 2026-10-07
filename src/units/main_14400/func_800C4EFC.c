#include "common.h"

/* Partial views shared with sibling func_800C4F18: IDs at +4/+8, two-word positions. */
typedef struct { s32 unk0; s32 unk4; s32 unk8; } S;
typedef struct { s32 x; s32 y; } Pos;
s32 func_80049CB4(s32 id, ...);

s32 func_800C4EFC(S *self, Pos *from, Pos *to)
{
    return func_80049CB4(self->unk4, from, to);
}
