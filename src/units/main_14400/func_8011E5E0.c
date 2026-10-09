#include "common.h"

typedef struct { unsigned char unknown00[0x1e]; unsigned char field1e; } Object;
extern unsigned short D_801569F6;
extern void func_800A7204(Object *, void *, void *, s32, s32, s32, s32, s32);
/* Slot +0x54 supplies self and attacker as pointers; this target ignores both. */
void func_8011E5E0(void *unused, void *value, Object *object, void *other, void *attacker) {
    s32 mode = 0;
    unsigned short color = 3;
    if (!((object->field1e >> 1) & 1)) { color = D_801569F6; mode = 5; }
    func_800A7204(object, value, other, color, mode, 6, 0, 1);
}
