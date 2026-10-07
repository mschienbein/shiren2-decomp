#include "common.h"

typedef unsigned char u8;

typedef short s16;
typedef unsigned short u16;
typedef struct {
    s16 delta;
    s16 index;
    void *func;
} VTableEntry;

typedef struct {
    s32 kind;
    u8 pad4[0xC];
    s32 value10;
    u8 pad14[4];
} Msg800C8C18;

typedef struct {
    s32 cursor;
    s32 pad4;
} Iter800C8C18;

typedef struct {
    u8 pad0[0x1E];
    u8 flags1E;
    u8 pad1F[5];
    VTableEntry *vtable24;
    u8 pad28[0x2A];
    u8 busy52;
} Actor800C8C18;

extern u16 D_8014767C;
extern s32 func_80046240(void);
extern s32 func_800A8FC8(Iter800C8C18 *iter, s32 kind);
extern Actor800C8C18 *func_800A910C(Iter800C8C18 *iter);
extern u16 func_800E08B0(Actor800C8C18 *actor);
extern s32 func_800E20CC(Actor800C8C18 *actor);

void func_800C8C18(u16 turns) {
    Msg800C8C18 msg;
    Iter800C8C18 iter;
    Actor800C8C18 *actor;
    VTableEntry *entry;
    s32 skip;
    s32 count;
    Msg800C8C18 *m;

    skip = func_80046240() || ((D_8014767C >> 6) & 1);
    if (skip) {
        return;
    }
    iter.cursor = 0;
    while (func_800A8FC8(&iter, 0x7C)) {
        actor = func_800A910C(&iter);
        skip = func_800E08B0(actor) == 0 || actor->busy52 != 0 || ((actor->flags1E >> 2) & 1) || func_800E20CC(actor);
        if (skip) {
            continue;
        }
        msg.kind = 2;
        count = turns;
        for (;;) {
            VTableEntry *e;

            if (--count == -1) {
                break;
            }
            e = &actor->vtable24[11];
            ((s32 (*)(void *, Msg800C8C18 *))e->func)((u8 *)actor + e->delta, &msg);
            func_800E08B0(actor);
        }
        m = &msg;
        m->kind = 3;
        m->value10 = (u8)turns;
        entry = &actor->vtable24[11];
        ((s32 (*)(void *, Msg800C8C18 *))entry->func)((u8 *)actor + entry->delta, m);
    }
}
