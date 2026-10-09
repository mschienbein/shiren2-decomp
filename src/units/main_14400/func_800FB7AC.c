#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* g++ 2.x vtable slot; slot 18 targets (func_800F212C, func_800F426C) take
 * (self, s32, s32, u8, s32) and return s32. */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(void *self, s32 a, s32 b, u8 c, s32 d);
} VtblEntry;

typedef struct Obj800E3884 {
    u8 pad0[0x24];
    VtblEntry *vtbl24;
} Obj800E3884;

s32 func_80049CB4(s32 id, ...);
void func_800E3678(Obj800E3884 *obj, Obj800E3884 *attacker);
s32 func_800A08D8(s32 mode, s32 key, s32 sel);

s32 func_800FB7AC(Obj800E3884 *attacker, Obj800E3884 *obj) {
    VtblEntry *entry;
    s32 text;

    entry = &obj->vtbl24[18];
    if ((entry->fn((u8 *)obj + entry->delta, 2, 9, 0, 0) ^ 1) != 0) {
        text = func_80049CB4(0x55, attacker);
        entry = &obj->vtbl24[18];
        entry->fn((u8 *)obj + entry->delta, 0, 9, 0xFE, 0);
        func_800E3678(obj, attacker);
        func_800A08D8(1, text, 0);
        return 1;
    }
    return 0;
}
