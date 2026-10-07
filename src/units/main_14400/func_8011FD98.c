#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x1E]; u8 flags_1E; } Obj;
s32 func_80049CB4(s32 id, ...);
/* Trap apply slot +0x54 supplies five pointers; self, actor, direction and
 * attacker are unused by this override. */
void func_8011FD98(void *self, void *actor, void *target, void *direction, void *attacker) {
    Obj *obj = target;
    s32 notset = ((obj->flags_1E >> 2) & 1) ^ 1;
    if (notset) {
        func_80049CB4(0x90, obj);
    }
}
