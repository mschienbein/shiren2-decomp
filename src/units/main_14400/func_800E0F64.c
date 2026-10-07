#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { s32 x; s32 y; } Pos800E0F64;
typedef struct { u32 high : 20; u32 audible : 1; u32 low : 11; } Flags800E0F64;
typedef struct {
    u8 pad0[0x78];
    s16 this_offset;
    u8 pad7A[2];
    void (*react)(void *self, s16 kind);
} VTable800E0F64;
typedef struct {
    s32 x;
    s32 y;
    u8 pad8[0x18];
    Flags800E0F64 flags;
    VTable800E0F64 *vtable;
} Unit800E0F64;
void func_800E20F0(Unit800E0F64 *unit);
s32 func_800E0F40(Unit800E0F64 *unit);
s32 func_80049CB4(s32 id, ...);
void func_800497F0(s32 id, ...);
static inline s32 flags_audible(Flags800E0F64 *flags) {
    return flags->audible;
}

s32 func_800E0F64(Unit800E0F64 *unit, s16 kind, s32 play_extra) {
    Pos800E0F64 pos;
    Flags800E0F64 flags;
    s32 before;
    func_800E20F0(unit);
    before = (u8)func_800E0F40(unit);
    if (kind < 0 && (flags = unit->flags, flags_audible(&flags))) {
        Pos800E0F64 *p;
        s32 handle;
        pos.x = unit->x;
        p = &pos;
        p->y = unit->y;
        handle = func_80049CB4(0xDA, p);
        if (play_extra != 0) {
            func_800497F0(0x224, handle);
        }
    } else {
        unit->vtable->react((u8 *)unit + unit->vtable->this_offset, kind);
    }
    return before != (u8)func_800E0F40(unit);
}
