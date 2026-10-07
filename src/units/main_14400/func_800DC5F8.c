#include "common.h"

typedef unsigned char u8;
typedef struct { u8 unk0; u8 unk1; char pad2[6]; s32 unk8[2]; s32 unk10[2]; } Src;
void func_800DA9DC(u8 *, s32 *);
s32 func_800DC5F8(Src *src, u8 *dst) {
    *dst++ = src->unk1;
    func_800DA9DC(dst++, src->unk8);
    func_800DA9DC(dst, src->unk10);
    return 3;
}
