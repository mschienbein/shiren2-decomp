#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(void *self, void *msg);
} VtEntry800C7A9C;

typedef struct {
    s32 kind;
    s32 args[5];
} Msg800C7A9C;

typedef struct {
    s32 field_0;
    s32 active;
} Sub800C7A9C;

typedef struct {
    u8 pad0[0x24];
    VtEntry800C7A9C *vtable;
    u8 pad28[0x84 - 0x28];
    Sub800C7A9C sub;
} Actor800C7A9C;

typedef struct {
    s32 index;
    s32 field_4;
} Iter800C7A9C;

extern u32 D_8013960C;
extern u8 D_801429C0[];
extern Actor800C7A9C *D_801476B8;
void func_800AA3C0(void);
s32 func_800B2A14(void *, s32);
void func_800EFE30(void);
void func_800B4B88(void);
s32 func_800A8FC8(Iter800C7A9C *it, s32 kind);
Actor800C7A9C *func_800A910C(Iter800C7A9C *it);
void func_800B512C(s32 arg);
s32 func_80049CB4(s32, ...);

void func_800C7A9C(void) {
    Msg800C7A9C msg;
    Iter800C7A9C it;
    Actor800C7A9C *world;
    Actor800C7A9C *actor;
    VtEntry800C7A9C *entry;

    D_8013960C *= 2;
    func_800AA3C0();
    func_800B2A14(D_801429C0, 1);
    func_800EFE30();
    func_800B4B88();
    msg.kind = 0x16;
    world = D_801476B8;
    entry = &world->vtable[11];
    entry->fn((char *)world + entry->delta, &msg);
    it.index = 0;
    while (func_800A8FC8(&it, 8)) {
        Sub800C7A9C *sub;

        Actor800C7A9C *found = func_800A910C(&it);
        /* The ROM keeps the dispatch copy (a2) apart from the null-tested
           call result (v0) that feeds the subobject conversion. */
        actor = found;
        sub = found != 0 ? &found->sub : 0;
        if (sub->active == 0) {
            continue;
        }
        entry = &actor->vtable[11];
        entry->fn((char *)actor + entry->delta, &msg);
    }
    func_800B512C(0);
    func_80049CB4(0xDB);
    func_80049CB4(2);
    D_8013960C /= 2;
}
