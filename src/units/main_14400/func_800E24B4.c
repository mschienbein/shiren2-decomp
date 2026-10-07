#include "common.h"

typedef unsigned char u8;

/*
 * Base actor prefix view: +0x58 holds the target actor. The actor update
 * loads it (0x800E48BC) and passes it to func_800A6420 as the target object,
 * which dereferences its position and vtable. Shared with the getter
 * func_800E24BC and the setter func_800E24C4.
 */
typedef struct Actor {
    u8 pad0[0x58];
    struct Actor *target;
} Actor;

void func_800E24B4(Actor *actor) {
    actor->target = 0;
}
