#include "common.h"
typedef short s16;
typedef struct { s16 delta; s16 index; void *fn; } VtblEntry;
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad[0x1E]; u8 flags; char pad1F[5]; VtblEntry *vtbl; } Obj8011DEF8;
extern u16 D_801569F8;
/* Trap apply slot +0x54 supplies five pointers; self, actor, direction and
 * attacker are unused by this override. */
void func_8011DEF8(void *self, void *actor, void *obj, void *direction, void *attacker) {
    u8 flags = ((Obj8011DEF8 *)obj)->flags;
    if ((flags >> 2) & 1) {
        VtblEntry *e = &((Obj8011DEF8 *)obj)->vtbl[16];
        ((void (*)(void *, s32))e->fn)((char *)obj + e->delta, D_801569F8);
    } else if (flags & 0x7C) {
        VtblEntry *e = &((Obj8011DEF8 *)obj)->vtbl[15];
        ((void (*)(void *, s16))e->fn)((char *)obj + e->delta, 1);
    }
}
