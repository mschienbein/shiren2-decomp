#include "common.h"

typedef unsigned char u8;
typedef struct { void *selected; u8 index; u8 pad5[3]; } Entry;
extern Entry D_8013F3D0[];
extern Entry D_8013FE90[];
extern Entry D_8013FEB0[];
extern Entry D_8013FEC8[];
extern Entry D_8013FA90[];

s32 func_80075F44(s32 kind, s32 level, s32 sub) {
    Entry *table = D_8013FA90;
    Entry *entry;
    if (sub >= 127 || kind < 2 || kind >= 215) return -1;
    level = level > 0 ? level - 1 : 0;
    switch (kind) {
    case 0x37:
        if (level >= 4) level = 3;
        entry = &D_8013FE90[level];
        break;
    case 0x31:
        if (level >= 3) level = 2;
        entry = &D_8013FEB0[level];
        break;
    case 0x25:
        if (level >= 3) level = 2;
        entry = &D_8013FEC8[level];
        break;
    default:
        entry = &D_8013F3D0[kind];
        break;
    }
    {
        /* local-arithmetic-qualification: in-bounds table[index] emits the base first;
           retain the byte offset as the first operand of this local address sum. */
        Entry *byIndex = (Entry *)((u32)entry->index * sizeof(Entry) + (u32)table);
        Entry *bySub = &D_8013FA90[sub];
        if (sub == 0) entry->selected = byIndex;
        else entry->selected = bySub;
    }
    return 0;
}
