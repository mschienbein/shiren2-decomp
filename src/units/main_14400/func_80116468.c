#include "common.h"

typedef unsigned char u8;
typedef struct { char pad[2]; u8 flags2; char pad3[9]; u8 flagsC; } Obj;
typedef struct { s32 id; } Msg;
s32 func_800AF28C(Obj *, Msg *);
void func_801159CC(Obj *, s32, Msg *);
s32 func_80116468(Obj *obj, Msg *msg) {
    s32 id = msg->id;
    switch (id) {
    case 0x1A:
        obj->flags2 &= ~0x10;
        break;
    case 0x15:
    case 0x16:
    case 0x17:
        func_801159CC(obj, id, msg);
        return 1;
    case 0x1F:
        obj->flagsC &= ~4;
        break;
    default:
        return func_800AF28C(obj, msg);
    }
    return 1;
}
