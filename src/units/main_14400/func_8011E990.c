#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x1E];
    u8 flags;
} Obj;

extern void func_800E35A8(Obj *obj, void *source, u8 percent, s32 kind, s32 keepAll);

/* Trap apply slot +0x54 supplies five pointers; self, direction and attacker
 * are unused by this override. */
void func_8011E990(void *self, void *b, void *target, void *direction, void *attacker) {
    Obj *obj = target;
    if (obj->flags & 0x7C) {
        func_800E35A8(obj, b, 0x4B, 0x1F, 0);
    }
}
