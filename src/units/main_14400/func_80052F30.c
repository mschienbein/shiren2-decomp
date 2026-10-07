#include "common.h"

typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

/* Observed table stride is 12 bytes. Original field names are unknown. */
struct D_801397E0_entry {
    s16 unk0;
    u16 unk2;
    u32 unk4;
    u8 unk8;
    u8 unk9;
    u16 unkA;
};

/* Original prototypes are unknown. unk4 may be a handle or pointer bit pattern. */
extern struct D_801397E0_entry *func_80053034(s16 arg0);

u32 func_80052F30(s16 arg0)
{
    return func_80053034(arg0)->unk4;
}
