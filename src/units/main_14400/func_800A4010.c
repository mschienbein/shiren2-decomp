#include "common.h"
typedef unsigned short u16;
typedef struct { u32 pad_bits : 7; u32 locked : 1; unsigned char rest[3]; } Flags;
typedef struct { unsigned char pad_00[0x1C]; u16 flags_1C; unsigned char flags_1E; unsigned char field_1F; Flags flags_20; } Unit;
s32 func_800E1CD4(Unit *object, s32 kind);
s32 func_800A4140(s32 flags, s32 mode);
s32 func_800A416C(s32 flags, s32 mode);
s32 func_800A4198(s32 flags, s32 mode);
s32 func_800A41B4(s32 flags, s32 mode);
s32 func_800A4124(s32 flags, s32 mode);
s32 func_800A41D0(s32 flags, s32 mode);
static inline s32 Flags_locked(Flags *flags) { return flags->locked; }
s32 func_800A4010(Unit *unit, u16 flags, s32 mode)
{
    s32 special = 0;
    if ((unit->flags_1C & 4) || ((unit->flags_1E & 0x7C) && func_800E1CD4(unit, 15))) {
        special = 1;
    }
    if (special) {
        return func_800A4140(flags, mode);
    }
    if (unit->flags_1C & 8) {
        return func_800A416C(flags, mode);
    }
    if (unit->flags_1C & 0x10) {
        return func_800A4198(flags, mode);
    }
    if (unit->flags_1C & 0x20) {
        return func_800A41B4(flags, mode);
    }
    {
        Flags state = unit->flags_20;
        s32 result;
        if (!Flags_locked(&state)) {
            result = func_800A4124(flags, mode);
        } else {
            result = func_800A41D0(flags, mode);
        }
        return result;
    }
}
