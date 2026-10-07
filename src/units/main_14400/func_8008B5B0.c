#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad0[4]; u16 state; u8 pad6[0x1E]; s32 unk24; s32 unk28; u8 pad2C[0x30]; s32 unk5C; s32 unk60; s32 unk64; s32 unk68; s32 unk6C; s32 unk70; } Obj;
void func_80061D28(s32 a, s32 b, s32 c, s32 d);
void func_8007D824(s32 a, s32 b, s32 c, s32 d);
void func_8008B5B0(Obj *o) {
    func_80061D28(o->unk5C, o->unk68, o->unk60, o->unk6C);
    func_8007D824(o->unk64, o->unk70, o->unk24, o->unk28);
    o->state = 4;
}
