#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { u8 v[1]; } Byte;
typedef struct {
    u8 pad0[8];
    u8 unk8;
    u8 pad9[0xA - 9];
    u8 unkA;
    u8 padB[0x1F - 0xB];
    u8 unk1F;
    u8 pad20[0xA4 - 0x20];
    s32 unkA4;
    s32 unkA8;
    u8 padAC[0x104 - 0xAC];
    s32 unk104;
} S;
extern S *D_801476B8;
s32 func_800E20CC(S *);
s32 func_800E1CC4(S *, s32);
s32 func_800F4498(S *);
u8 func_800A6420(S *, S *);
s32 func_800E2074(S *);
void func_800A665C(S *, Byte *);
s32 func_800F7970(S *, S *);

static __inline__ s32 isBusy(S *self) {
    s32 busy = 0;
    if (self->unkA4 != 0 || func_800E20CC(self) || func_800E1CC4(self, 0)) {
        busy = 1;
    }
    return busy;
}

static __inline__ s32 canFollow(S *self) {
    return self->unkA8 != 0 && !func_800A6420(self, D_801476B8) && func_800E2074(D_801476B8) &&
           D_801476B8->unk104 == 0 && D_801476B8->unk1F == D_801476B8->unkA;
}

s32 func_800F7664(S *self) {
    if (isBusy(self)) {
        return func_800F4498(self);
    }
    if (canFollow(self)) {
        if (func_800E1CC4(self, 4)) {
            Byte dir;
            dir.v[0] = (self->unk8 + 4) & 7;
            func_800A665C(self, &dir);
            return 0;
        }
        if (func_800F7970(self, D_801476B8)) {
            self->unkA8 = 0;
        }
        return 1;
    }
    return 0;
}
