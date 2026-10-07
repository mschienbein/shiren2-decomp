#include "common.h"
typedef unsigned char u8;
typedef struct { short delta; short index; s32 (*fn)(void *, s32, s32, u8, s32); } VEntry;
typedef struct { char pad[0x90]; VEntry e; } VTable;
typedef struct { char pad[0x1E]; u8 flags; char pad1F[5]; VTable *vtbl; } Obj;
extern u32 D_8013960C;
/* Trap apply slot +0x54 supplies five pointers; self, actor, direction and
 * attacker are unused by this override. */
void func_8011E6F8(void *self, void *actor, void *target, void *direction, void *attacker) {
    Obj *p = target;
    if (p->flags & 0x7C) {
        VTable *vt = p->vtbl;
        D_8013960C <<= 1;
        vt->e.fn((char *)p + vt->e.delta, 0, 2, 0xFE, 0);
        D_8013960C >>= 1;
    }
}
