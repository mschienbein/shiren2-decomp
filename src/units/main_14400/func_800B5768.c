#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Partial view: only the level byte at +2 is known. */
typedef struct {
    u8 pad0[2];
    u8 level;
} Entry;

extern u16 D_801569FC;
extern u16 D_80156A00;
extern u16 D_80156A02;
extern u16 D_80156A04;
extern u16 D_80156A06;

extern Entry *func_800B51D4(void *object);

u16 func_800B5768(void *object)
{
    Entry *entry = func_800B51D4(object);
    u8 level;

    if (entry == 0) {
        return 0;
    }
    level = entry->level;
    if (level == 0) {
        return 0;
    }
    if (level <= D_80156A06) {
        return D_80156A04;
    }
    if (level <= D_80156A02) {
        return D_80156A00;
    }
    if (level <= 100) {
        return D_801569FC;
    }
    return 0;
}
