#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Pair800C481C;
typedef struct { u8 pad0[0x44]; s32 unk44; s32 unk48; } Src800C481C;
Pair800C481C *func_800C481C(Pair800C481C *out, Src800C481C *src) {
    out->x = src->unk44;
    out->y = src->unk48;
    return out;
}
