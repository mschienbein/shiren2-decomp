#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    u8 dir_8;
} Obj_800A6690;

typedef struct {
    u8 dir;
    u8 diff;
} Turn_800A6690;

extern void func_800A2F80(unsigned char *self, s32 step);
extern void func_800A2F94(unsigned char *value, s32 delta);
extern s32 func_80049CB4(s32 command, ...);

static inline s32 turn_differs(Turn_800A6690 *turn, u8 *target) {
    return turn->dir != *target;
}

/* ODD_C: taking both direction bytes by address keeps the current-direction
 * register copy and re-reads *target, matching the original move/lbu pair. */
static inline s32 dir_delta(u8 *from, u8 *to) {
    return (*from - *to) & 7;
}

void func_800A6690(Obj_800A6690 *obj, u8 *target, s32 arg) {
    Turn_800A6690 turn;
    Turn_800A6690 *cur = &turn;

    turn.dir = obj->dir_8;
    while (turn_differs(cur, target)) {
        /* ODD_C: promoting the stored byte to s32 preserves the original
         * andi 0xFF followed by the signed slti comparison. */
        s32 delta;
        turn.diff = dir_delta(&cur->dir, target);
        delta = turn.diff;
        if (delta >= 5) {
            func_800A2F80(&cur->dir, 1);
        } else {
            func_800A2F94(&cur->dir, 1);
        }
        obj->dir_8 = turn.dir;
        func_80049CB4(0x8B, obj);
        func_80049CB4(0x129, arg);
    }
}
