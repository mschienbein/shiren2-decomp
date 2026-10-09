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
    Pair pos;
    u8 dir;
} Placement;
typedef struct {
    s32 type;
    u8 pad4[8];
    u8 dir;
    u8 padD[3];
    Pair pos;
    u8 pad18[8];
} Message;
u8 *func_800B4D80(Placement *);
s32 func_800A7A1C(Placement *place) {
    u8 *target = func_800B4D80(place);
    Message msg;
    VtblEntry *vt;
    Message *pmsg;

    if (target == 0) {
        return 0;
    }
    if (target[0] != 0x10) {
        return 0;
    }
    msg.type = 0x15;
    msg.pos = place->pos;
    pmsg = &msg;
    pmsg->dir = place->dir;
    vt = *(VtblEntry **)(target + 8);
    vt[7].func(target + vt[7].delta, pmsg);
    return 1;
}
