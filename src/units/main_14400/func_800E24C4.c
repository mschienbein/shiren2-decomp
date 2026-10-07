#include "common.h"

typedef unsigned char u8;

/*
 * Base actor prefix view: +0x58 holds the target actor (passed to
 * func_800A6420 as its target object by the actor update at 0x800E48BC).
 * Shared with the clear func_800E24B4 and the getter func_800E24BC.
 */
typedef struct Actor {
    u8 pad0[0x58];
    struct Actor *target;
} Actor;

void func_800E24C4(Actor *actor, Actor *target) {
    actor->target = target;
}
