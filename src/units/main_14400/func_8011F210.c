#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef struct { u8 pad[0x90]; s16 delta; s16 idx; s32 (*fn)(void *, s32, s32, u8, s32); } VTable;
typedef struct { u8 pad0[0x1E]; u8 flags; u8 pad1F[5]; VTable *vt; } Obj;
extern u32 D_8013960C;
/* Trap apply slot +0x54 supplies five pointers; self, actor, direction and
 * attacker are unused by this override. */
void func_8011F210(void *self, void *actor, void *target, void *direction, void *attacker) {
    Obj *o = target;
    if (o->flags & 0x7C) {
        D_8013960C <<= 1;
        o->vt->fn((u8 *)o + o->vt->delta, 0, 5, 0xFE, 0);
        D_8013960C >>= 1;
    }
}
