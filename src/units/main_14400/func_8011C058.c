#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pair;
typedef struct { s32 type; char pad4[0xC]; Pair pos; } Msg;
typedef struct { char pad0[2]; u8 flags2; char pad3[9]; u8 flagsC; char padD[3]; Pair pos; } Self;
extern u8 D_801476BC;
extern void *func_800B31E8(Pair *pos, s32 team);
extern s32 func_801131F8(Self *, Msg *);
s32 func_8011C058(Self *self, Msg *m) {
    switch (m->type) {
    case 0x1F: {
        s32 hit = 0;
        if (!(self->flagsC & 1) && (self->flags2 & 0x20)) hit = !func_800B31E8(&self->pos, 10);
        if (hit) D_801476BC |= 1;
        return 1;
    }
    case 0x1A:
        self->pos = m->pos;
        break;
    }
    return func_801131F8(self, m);
}
