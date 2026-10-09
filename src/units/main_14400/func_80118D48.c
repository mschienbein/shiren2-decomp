#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad00[0x1E];
    u8 flags1E;
} Obj;

extern void func_800498E4(s32 id, ...);

/* Item vtable slot +0x4C pair action (self, source, target). `self` is the adjusted receiver
 * and `target` the target unit supplied by the dispatcher; only the source unit is tested here. */
void func_80118D48(void *self, Obj *o, void *target)
{
    if ((o->flags1E >> 2) & 1) {
        func_800498E4(0x223);
    }
}
