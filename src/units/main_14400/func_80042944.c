#include "common.h"

typedef unsigned char u8;
typedef struct { char pad0[8]; u8 unk8; char pad9[0x15]; u8 unk1E; } Ent;
void *func_800A8CB0(s32 cell);
s32 func_800E47FC(Ent *);
s32 func_80042944(u8 id) {
    Ent *e = func_800A8CB0(id);
    s32 ret;
    if (e == 0) {
        return 6;
    }
    if (e->unk1E & 0x7C) {
        ret = func_800E47FC(e);
    } else {
        ret = e->unk8;
    }
    return ret;
}
