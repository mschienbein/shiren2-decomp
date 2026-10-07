#include "common.h"

typedef struct Ctx {
    unsigned char pad0[0x2CC];
    s32 f2CC;
    s32 f2D0;
    signed char f2D4;
    unsigned char pad2D5[0x2EC - 0x2D5];
    s32 (*callback)(void *entry);
    unsigned char pad2F0[0x2FC - 0x2F0];
    s32 f2FC;
} Ctx;
s32 func_80098E34(Ctx *c, s32 position);
void *func_800980F0(Ctx *c, s32 idx);
s32 func_800981E8(Ctx *c, s32 position) {
    s32 idx;
    void *entry;
    s32 accepted;
    if (c->f2FC == 8) {
        return 0x80000000;
    }
    if (c->f2FC == 0x400) {
        return 0x80000000;
    }
    idx = func_80098E34(c, position);
    if (idx < 0) {
        return 0x80000000;
    }
    entry = func_800980F0(c, idx);
    if (entry == 0) {
        return 0x80000000;
    }
    if (c->f2CC != 0 && c->f2D0 > 0) {
        return c->f2D4;
    }
    if (c->callback == 0) {
        return idx;
    }
    accepted = c->callback(entry) == 1;
    if (!accepted) {
        return 0x80000000;
    }
    return idx;
}
