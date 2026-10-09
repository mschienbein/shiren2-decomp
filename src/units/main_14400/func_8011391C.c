#include "common.h"

typedef unsigned char u8;

typedef struct {
    short delta;
    short index;
    void (*fn)(void *self, s32 flags);
} DtorEntry;

typedef struct {
    u8 pad0[0x8];
    DtorEntry dtor;
} TimerVTable;

typedef struct {
    u8 field_0;
    u8 kind;
    u8 pad2[0x6];
    TimerVTable *vt;
    u8 turns;
} Timer;

typedef struct {
    u8 pad0[0x98];
    short delta;
    short index;
    void *(*fn)(void *self);
} UnitVTable;

typedef struct {
    u8 pad0[0x1E];
    u8 flags_1E;
    u8 pad1F[0x24 - 0x1F];
    UnitVTable *vt;
} Unit;

/* Bit stack pushed and popped around the messages below. */
extern u32 D_8013960C;

char *func_800AE674(void *obj);
char *func_800A3B20(Unit *u);
void func_800498E4(s32 id, ...);
s32 func_80049CB4(s32 id, ...);
void func_80049BF0(s32 mode);
s32 func_800CD2BC(void *object, void *element);
void func_800D3650(void *arg);

/* Report the timer, detach it from the unit's list (vtable slot 19) and delete it. */
void func_8011391C(Timer *t, Unit *unit)
{
    D_8013960C = (D_8013960C << 1) | 1;
    if ((unit->flags_1E >> 2) & 1) {
        func_800498E4(0xFF, func_800AE674(t));
    } else {
        char *name = func_800A3B20(unit);

        func_800498E4(0x100, name, func_800AE674(t));
    }
    func_80049CB4(0x128, 0x17);
    func_80049BF0(0);
    D_8013960C >>= 1;
    if (unit != 0) {
        void *list = unit->vt->fn((u8 *)unit + unit->vt->delta);

        if (list != 0) {
            func_800CD2BC(list, t);
        }
    }
    func_800D3650(t);
    if (t != 0) {
        t->vt->dtor.fn((u8 *)t + t->vt->dtor.delta, 3);
    }
}
