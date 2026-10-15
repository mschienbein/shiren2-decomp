#include "func_801E8D0C_13AC2C/types.h"
extern void *func_800A8CB0(s32 cell);
extern s32 func_800A8974(void *obj);
s32 func_801E8D0C_13AC2C(u16 id, u16 selector) {
    s32 index;
    s32 kind, filter;
    if (!id) return 0;
    index = 0;
    kind = id;
    filter = selector;
    for (; ; index++) {
        S06Creature *obj;
        if (index >= 30) break;
        obj = func_800A8CB0((u8)index);
        if (func_800A8974(obj)) continue;
        if (kind != obj->id0A) continue;
        if (!filter) return index;
        if ((obj->flags1E & 0x7C) && obj->field32 == filter) return index;
    }
    return 0;
}
