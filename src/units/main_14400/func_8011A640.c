#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct Obj800E8A68 { Position position; u8 pad_08[0x16]; u8 field_1E; } Obj800E8A68;
typedef struct Item Item;
extern void *func_800E8A68(Obj800E8A68 *obj, u8 arg1);
extern char *func_800AE674(void *obj);
extern s32 func_8010BAE8(Item *, s32);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 id, ...);

static __inline__ Position *copy_position(Position *out, Position *source) {
    out->x = source->x;
    out->y = source->y;
    return out;
}

/* Vtable slot +0x44 (self, unit, item; dispatched by func_801131F8 at 0x801132A8-0x801132B8):
 * `self` and the supplied item argument are unused here. */
void func_8011A640(void *self, Obj800E8A68 *unit, void *arg2) {
    Item *item;
    if ((unit->field_1E >> 2) & 1) {
        item = func_800E8A68(unit, 4);
    } else {
        item = 0;
    }
    if (item) {
        char *before = func_800AE674(item);
        if (func_8010BAE8(item, 1)) {
            func_80049CB4(0x2B, unit);
            func_800498E4(0xCF, before, func_800AE674(item));
        } else {
            Position position;
            func_80049CB4(0x11D, copy_position(&position, &unit->position));
            func_800498E4(0xD0, before);
        }
    } else {
        func_80049CB4(0x132);
        func_800498E4(0x222);
    }
}
