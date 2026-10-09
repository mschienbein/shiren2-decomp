#include "common.h"
typedef struct { unsigned short field_00, field_02; unsigned char field_04[12]; unsigned short field_10; } State;
extern unsigned short D_801A70F2;
/* D_8013D3F0 supplies both input records; this handler uses only the first. */
s32 func_8006E1E0(State *state, void *other)
{
    u32 flags = state->field_02;
    if (flags & 0x1000) return 0;
    if (flags & 0x8000) return 1;
    if (flags & 0x4000) return 2;
    if ((flags & 0xA00) == 0xA00) return 0x17;
    if ((flags & 0x900) == 0x900) return 0x18;
    if ((flags & 0x600) == 0x600) return 0x19;
    if ((flags & 0x500) == 0x500) return 0x1A;
    if (!(state->field_00 & 0x10)) {
        if (flags & 0x200) return 0x15;
        if (flags & 0x100) return 0x16;
        if (flags & 0x800) return 0x13;
        if (state->field_02 & 0x400) return 0x14;
    }
    if (state->field_02 || (state->field_10 && !D_801A70F2)) return 3;
    return -1;
}
