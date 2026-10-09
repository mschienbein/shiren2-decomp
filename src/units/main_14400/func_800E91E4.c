#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct Container Container;

typedef struct {
    s32 index;
    Container *container;
    s32 active;
    void *current;
} Iter;

typedef struct {
    u8 kind;
} Ent;

typedef struct {
    u8 pad0[0x98];
    s16 delta_98;
    s16 pad9A;
    Container *(*contents_9C)(void *self);
} VTable800E91E4;

typedef struct {
    u8 pad0[0x24];
    VTable800E91E4 *vtable_24;
} Unit;

Iter *func_800CEB20(Iter *it, void *a);
s32 func_800CEBA0(Iter *it);
Ent *func_800CEC68(Iter *it);
void func_80112E7C(Ent *object);

s32 func_800E91E4(Unit *unit)
{
    Iter it;
    s32 found = 0;
    Container *contents = unit->vtable_24->contents_9C((u8 *)unit + unit->vtable_24->delta_98);

    if (contents != 0) {
        func_800CEB20(&it, contents);
        while (func_800CEBA0(&it)) {
            Ent *ent = func_800CEC68(&it);

            if (ent->kind == 2) {
                found = 1;
                func_80112E7C(ent);
            }
        }
    }
    return found;
}
