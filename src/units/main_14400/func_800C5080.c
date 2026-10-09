#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct {
    s16 delta;
    s16 pad2;
    s32 (*func)(void *, void *);
} VtblEntry;
typedef struct {
    s32 x;
    s32 y;
} Pair;
typedef struct {
    s32 type;
    u8 pad4[8];
    u8 dir;
    u8 padD[3];
    Pair pos;
    u8 pad18[8];
} Message;
u32 func_800B1C6C(Pair *);
s32 func_80049CB4(s32, ...);
u8 *func_800B4D80(Pair *);
/* The +0x14 contract supplies self and actor even though this override ignores them. */
void func_800C5080(void *self, void *actor, u8 *dir, Pair *pos) {
    u8 *target;
    Message msg;
    Message *pmsg;
    VtblEntry *vt;

    if (func_800B1C6C(pos) & 0x2000) {
        func_80049CB4(0x10B, pos);
        return;
    }
    if (func_800B1C6C(pos) & 0x100) {
        return;
    }
    func_80049CB4(0x114, pos);
    target = func_800B4D80(pos);
    if (target == 0) {
        return;
    }
    if (target[0] != 0x10) {
        return;
    }
    msg.type = 0x15;
    msg.pos = *pos;
    pmsg = &msg;
    pmsg->dir = *dir;
    vt = *(VtblEntry **)(target + 8);
    vt[7].func(target + vt[7].delta, pmsg);
}
