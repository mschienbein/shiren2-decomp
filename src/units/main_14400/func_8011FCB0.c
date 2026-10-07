#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x1E];
    u8 flags1E;
    u8 pad1F[0x7C - 0x1F];
    u16 flags7C;
} Unit8011FCB0;

extern u16 D_8014767C;
s32 func_800EBAC4(Unit8011FCB0 *attacker, Unit8011FCB0 *target);

/* Trap apply slot +0x54 supplies five pointers; self, actor and direction
 * are unused by this override. */
void func_8011FCB0(void *self, void *actor, void *target_arg, void *direction, void *attacker_arg) {
    Unit8011FCB0 *target = target_arg;
    Unit8011FCB0 *attacker = attacker_arg;
    if ((attacker->flags1E >> 2) & 1) {
        s32 ok = ((target->flags1E >> 4) & 1) && !((target->flags7C >> 9) & 1);
        if (ok && func_800EBAC4(attacker, target)) {
            D_8014767C |= 0x40;
        }
    }
}
