#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u32 pad_hi : 8;
    u32 flag23 : 1;
    u32 pad_lo : 23;
} Flags800EC630;

typedef struct {
    u8 pad00[0x20];
    Flags800EC630 flags20;
    u8 pad24[0xE4 - 0x24];
    u16 flagsE4;
} Unit80096140;

typedef struct {
    u8 kind;
    u8 pad01;
    u8 flags02;
} Info80096140;

/* Accessor on a local copy of the flag word (method-style call: the copy is addressable). */
static inline s32 flags_test23(const Flags800EC630 *flags)
{
    return flags->flag23;
}

s32 func_800EC630(Unit80096140 *unit, Info80096140 *info)
{
    s32 blocked = 0;
    s32 bit;

    if (info->kind == 0x10) {
        Flags800EC630 flags = unit->flags20;

        blocked = flags_test23(&flags) || ((unit->flagsE4 >> 3) & 1);
    }
    if (blocked) {
        return 1;
    }
    bit = info->flags02 & 0x10;
    return bit == 0;
}
