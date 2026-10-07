#include "common.h"
typedef unsigned char u8;
/* D_801C36EC: 29 unit slots of 0xE4 bytes plus the fallback slot at index 29 (0x801C50C0);
 * the occupancy bitmap D_801C51A4 follows it. Slot i is object id i + 1; the player object
 * D_801C35E0 is id 0. */
typedef struct { unsigned char b[0xE4]; } Slot;
extern unsigned char D_801C35E0[];
extern Slot D_801C36EC[30];
static inline s32 object_index(unsigned char *p) {
    if (p == D_801C35E0) return 0;
    {
        u32 index = (u32)(p - (unsigned char *)D_801C36EC) / 228;
        if (index < 0x1D) return (index + 1) & 0xFF;
        return 0xFF;
    }
}
u8 func_800A8C00(void *actor) {
    unsigned char *p = actor;
    if (!p || p == (unsigned char *)&D_801C36EC[29]) return 0xFF;
    return (u8)object_index(p);
}
