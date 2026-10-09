#include "common.h"
typedef struct Flags32 { u32 bits; } Flags32;
typedef struct Flags16 { unsigned short bits; } Flags16;
typedef struct Unit { unsigned char pad_00[0x20]; Flags32 flags_20; } Unit;
extern Unit *func_800C5F60(void);
extern Flags16 D_8014767C;
/* Whole 16-byte room state; +0xC is the active flag. */
typedef struct { void *room; s32 value4, value8, active; } RoomState;
extern RoomState D_80143434;
static __inline__ Flags32 *unit_flags(Flags32 *out, Unit *unit) {
    *out = unit->flags_20;
    return out;
}
s32 func_800C965C(void) {
    Flags32 flags;
    s32 result = 0;
    u32 bits;
    unit_flags(&flags, func_800C5F60());
    bits = flags.bits;
    bits >>= 7;
    bits &= 1;
    if (bits || ((D_8014767C.bits >> 5) & 1) || D_80143434.active) result = 1;
    return result;
}
