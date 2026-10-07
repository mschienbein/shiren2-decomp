#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 kind;
} Item800E219C;

typedef struct {
    u8 pad0[0xA];
    u8 id;
    u8 padB[0x1C - 0xB];
    u16 flags;
    u8 state;
    u8 pad1F[0x104 - 0x1F];
    void *field_104; /* retained actor pointer (func_800EE290, func_800EBCD0) */
} Obj800E219C;

extern void *D_801476B8;
void *func_800B4D80(void *pos);
u32 func_8011575C(void *self);
s32 func_800A44F4(void *world, Obj800E219C *obj);
s32 func_800A692C(Obj800E219C *obj, s32 arg);

void *func_800E219C(Obj800E219C *obj, void *pos) {
    Item800E219C *item = func_800B4D80(pos);
    s32 allowed;
    s32 ok;

    if (item == 0) {
        return 0;
    }
    if (item->kind != 0x10) {
        return 0;
    }
    if (func_8011575C(item)) {
        if (obj->id == 0x5E) {
            return 0;
        }
        allowed = 0;
        if ((obj->state & 0xC) == 0) {
            allowed = func_800A44F4(D_801476B8, obj) != 1;
        }
        if (!allowed) {
            return 0;
        }
        ok = 0;
        if (!(obj->flags & 0x40) || func_800A692C(obj, 0x14)) {
            ok = 1;
        }
        if (ok) {
            return item;
        }
    } else {
        allowed = 0;
        if ((obj->state >> 2) & 1) {
            allowed = obj->field_104 == 0;
        }
        if (allowed) {
            return item;
        }
    }
    return 0;
}
