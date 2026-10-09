#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 field_0;
    u8 kind;
    u8 pad2[0xA];
    u8 turns;
} Timer;

typedef struct {
    u8 pad0[0x1E];
    u8 flags_1E;
} Unit;

/* Random-number state and per-effect tuning bytes. */
extern u8 D_80147620[];
extern u8 D_80156935;
extern u8 D_80156937;
extern u8 D_80156939;

s32 func_800C587C(void *rng, u8 limit);
char *func_800AE674(void *obj);
char *func_800A3B20(Unit *u);
void func_800498E4(s32 id, ...);
s32 func_80049CB4(s32 id, ...);
void func_80049BF0(s32 mode);
s32 func_800C5844(void *rng, u8 base, u8 top);

void func_801137CC(Timer *t, Unit *unit, u8 chance)
{
    if (t->turns != 0) {
        return;
    }
    if (t->kind == 0x8A) {
        chance = D_80156935;
    }
    if (!func_800C587C(D_80147620, chance)) {
        return;
    }
    if ((unit->flags_1E >> 2) & 1) {
        func_800498E4(0xFD, func_800AE674(t));
    } else {
        char *name = func_800A3B20(unit);

        func_800498E4(0xFE, name, func_800AE674(t));
    }
    func_80049CB4(0x128, 0x16);
    func_80049BF0(0);
    t->turns = func_800C5844(D_80147620, D_80156937, D_80156939);
}
