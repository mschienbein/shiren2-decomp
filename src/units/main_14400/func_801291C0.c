#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { u8 pad0[8]; s32 unk8; u8 pad1[0x34-0xC]; s32 unk34; s32 unk38; u8 pad2[0x44-0x3C]; s32 unk44; u8 pad3[0x74-0x48]; s32 unk74; s32 unk78; } S;
/* D_801487D0 handlers take a state and byte cursor, returning the next cursor. */
s8 *func_801291C0(S *arg0, s8 *cursor) {
    arg0->unk38 = 0;
    arg0->unk34 = 0;
    arg0->unk74 = 0;
    arg0->unk78 = 0;
    arg0->unk44 = 0;
    arg0->unk8 = 0;
    return 0;
}
