#include "common.h"

typedef struct {
    unsigned high : 8;
    unsigned flag : 1;
    unsigned low : 23;
} Flags80041A0C;

typedef struct {
    char pad0[0x20];
    Flags80041A0C flags;
} Unit;

Unit *func_800C5F60(void);

static inline s32 flags_flag(Flags80041A0C *flags)
{
    return flags->flag;
}

s32 func_80041A0C(void)
{
    Flags80041A0C flags;

    flags = func_800C5F60()->flags;
    return flags_flag(&flags);
}
