#include "common.h"

typedef unsigned short u16;

typedef struct Obj_800F96B4 Obj_800F96B4;

typedef struct {
    s32 kind;
} Msg_800F96B4;

extern s32 func_800F27A4(Obj_800F96B4 *obj, Msg_800F96B4 *msg);
extern s32 func_801F258C(s32 id, s32 arg);
extern u16 D_8014767C;

s32 func_80108330(Obj_800F96B4 *self, Msg_800F96B4 *msg)
{
    if (msg->kind == 10 && func_801F258C(0x54, 0)) {
        D_8014767C |= 0x40;
    }
    return func_800F27A4(self, msg);
}
