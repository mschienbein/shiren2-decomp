#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 words[0x27]; } Block;
typedef struct { u8 pad0[0x54]; Block block; u8 padF0[0xF0 - 0x54 - 0x9C]; s32 field_F0; } Obj;
u8 D_801425C4[4] = { 0x0C, 0x01, 0x1E, 0x18 };
s32 D_801425C8[4] = { 0x18, 0x1E, 3, 5 };
u8 D_801425D8[4] = { 0x03, 0x01, 0x1E, 0x03 };
s32 D_801425DC[4] = { 6, 0x1E, 0xC, 5 };
void func_8009D910(Obj *obj, s32 *a, u8 *b);
void func_8009E5A0(Obj *obj, Block *src, s32 flag) {
    s32 *a;
    u8 *b;

    obj->block = *src;
    obj->field_F0 = flag;
    if (flag != 0) {
        b = D_801425C4;
        a = D_801425C8;
    } else {
        b = D_801425D8;
        a = D_801425DC;
    }
    func_8009D910(obj, a, b);
}
