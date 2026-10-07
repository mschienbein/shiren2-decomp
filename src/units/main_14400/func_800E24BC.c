#include "common.h"

typedef unsigned char u8;

/*
 * Base actor prefix view: +0x58 holds the target actor (passed to
 * func_800A6420 as its target object by the actor update at 0x800E48BC).
 * Shared with the clear func_800E24B4 and the setter func_800E24C4.
 */
typedef struct Actor {
    u8 pad0[0x58];
    struct Actor *target;
} Actor;

Actor *func_800E24BC(Actor *actor) {
    return actor->target;
}
