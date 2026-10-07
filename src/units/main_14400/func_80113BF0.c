#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { u8 pad[0xD]; u8 unkD; } S;
typedef struct { s32 type; u8 pad[0x14]; s32 unk18; } Msg;
s32 func_800AF28C(S *, Msg *);
void func_80113C68(S *, Msg *);
s32 func_80113BF0(S *arg0, Msg *msg) {
    switch (msg->type) {
    case 15:
        func_80113C68(arg0, msg);
        return 1;
    case 25:
        if (msg->unk18 != 0) {
            arg0->unkD |= 1;
        } else {
            arg0->unkD &= ~1;
        }
        return 1;
    default:
        return func_800AF28C(arg0, msg);
    }
}
