#include "common.h"
#include "collection_iterator.h"

typedef unsigned char u8;
typedef short s16;

typedef struct Container Container;


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

void func_80112E7C(Ent *object);

s32 func_800E91E4(Unit *unit)
{
    CollectionIterator it;
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
