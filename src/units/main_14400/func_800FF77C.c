#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad00[0x8A];
    u8 field_8A;
} Actor;

typedef struct Item Item;

extern char D_80147620[];
extern void *func_800E8A68(Actor *obj, u8 kind);
extern s32 func_800C587C(void *rng, u8 limit);

static inline Item *equipped(Actor *actor, u8 slot) {
    return func_800E8A68(actor, slot);
}

/* Pick one of the target's items in slots 4 and 3; when both exist, roll against self->field_8A. */
Item *func_800FF77C(Actor *self, Actor *target) {
    Item *first = equipped(target, 4);
    Item *second = equipped(target, 3);
    Item *result;

    if (first != 0) {
        if (second == 0) {
            return first;
        }
        if (func_800C587C(D_80147620, self->field_8A) != 0) {
            return first;
        }
        return second;
    }
    result = second;
    if (result == 0) {
        return 0;
    }
    return result;
}
