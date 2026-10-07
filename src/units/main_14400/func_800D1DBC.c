#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef struct {
    u8 pad[8];
    s16 delta;
    s16 padA;
    void (*fn)(void *, s32);
} VTable;
typedef struct {
    u8 pad[8];
    VTable *vtable;
} Entity;
typedef struct {
    s8 kind;
    u8 pad1[3];
    void *unk4;
    u8 unk8;
    u8 pad9[3];
    s32 unkC;
    s32 unk10;
    s32 unk14;
} Spawner;
extern u8 D_80154738[];
extern s32 D_80147F70;
extern s32 D_80147F74;
s32 func_800ABE50(void);
void *func_800AADF8(s8 index, u8 value);
void func_800D233C(Spawner *);
void func_800D23AC(Spawner *);
s32 func_800D248C(void *spawner);

void func_800D1DBC(Spawner *sp, s8 kind, void *arg)
{
    s32 tries;

    sp->kind = kind;
    sp->unk4 = arg;
    tries = 100;
    for (;;) {
        Entity *ent;
        s32 id;

        if (--tries == -1) {
            break;
        }
        id = D_80154738[func_800ABE50()];
        sp->unk8 = id;
        if (id == 0) {
            break;
        }
        ent = func_800AADF8(-1, sp->unk8);
        if (ent != 0) {
            ent->vtable->fn((u8 *)ent + ent->vtable->delta, 3);
            break;
        }
    }
    if (tries < 0) {
        sp->unk8 = 0;
    }
    sp->unkC = 0;
    sp->unk10 = 0;
    sp->unk14 = 0;
    D_80147F70 = 0;
    D_80147F74 = 0;
    func_800D233C(sp);
    func_800D23AC(sp);
    func_800D248C(sp);
}
