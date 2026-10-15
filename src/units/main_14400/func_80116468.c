#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef struct { s32 id; } Msg;
s32 func_800AF28C(Item *, Msg *);
void func_801159CC(Item *, s32, Msg *);
s32 func_80116468(Item *obj, Msg *msg) {
    s32 id = msg->id;
    switch (id) {
    case 0x1A:
        obj->flags02 &= ~0x10;
        break;
    case 0x15:
    case 0x16:
    case 0x17:
        func_801159CC(obj, id, msg);
        return 1;
    case 0x1F:
        obj->payload.trap.flags0C &= ~4;
        break;
    default:
        return func_800AF28C(obj, msg);
    }
    return 1;
}
