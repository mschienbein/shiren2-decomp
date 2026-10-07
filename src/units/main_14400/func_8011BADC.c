#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { s32 a, b; } Pair;
typedef struct { u8 pad[2]; u8 flags2; u8 pad3[9]; u8 flagsC; u8 padD[3]; Pair pos; } S;
typedef struct { s32 type; u8 pad[0xC]; Pair pos; } Msg;
extern u8 D_801476BC;
void *func_800B31E8(Pair *pos, s32 team);
s32 func_801131F8(S *, Msg *);
s32 func_8011BADC(S *s, Msg *msg) {
    s32 ok;
    switch (msg->type) {
    case 0x1F:
        ok = 0;
        if ((s->flags2 & 0x20) && !(s->flagsC & 1)) {
            ok = func_800B31E8(&s->pos, 10) == 0;
        }
        if (ok) {
            D_801476BC |= 2;
        }
        return 1;
    case 0x1A:
        s->pos = msg->pos;
        break;
    }
    return func_801131F8(s, msg);
}
