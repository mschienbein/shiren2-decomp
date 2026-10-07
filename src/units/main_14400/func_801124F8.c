#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef struct {
    s16 offset;
    u8 pad2[2];
    u32 (*func)(void *self); /* slot 13 (+0x68/+0x6C): u32 func_800E0E88(Obj *) */
} VtEntry;
typedef struct {
    u8 pad0[0x68];
    VtEntry entry_68;
} Vtable;
typedef struct {
    u8 pad0[0x1E];
    u8 flags;
    u8 pad1F[0x24 - 0x1F];
    Vtable *vtable;
} Unit;
typedef struct {
    u8 pad0[0x10];
    s32 field_10;
    s8 field_14;
    u8 field_15;
} Source;
typedef struct {
    void *actor;
    s32 field_4;
    s32 field_8;
    s16 value;
    u16 flags;
    u8 field_10;
} Result;
s32 func_800E8DC8(Unit *unit, s16 value);
void func_801124F8(Source *src, void *actor, Unit *unit, Result *out) {
    s32 value = src->field_14;
    s32 field_10 = src->field_10;

    out->actor = actor;
    out->field_4 = field_10;
    out->field_8 = 0;
    out->value = value;
    out->flags = 0;
    out->field_10 = 10;
    if (src->field_15 != 0) {
        out->flags |= 8;
    } else if (unit != 0) {
        if (unit->flags & 0xC) {
            out->value = (s16)func_800E8DC8(unit, out->value);
        } else if (unit->flags & 0x7C) {
            out->value += unit->vtable->entry_68.func((u8 *)unit + unit->vtable->entry_68.offset);
        }
    }
    if (out->value < 0) {
        out->value = 1;
    }
}
