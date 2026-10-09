#include "common.h"

typedef unsigned char u8;
typedef struct Entry { u8 bytes[4]; } Entry;
extern u8 D_80143114[40];
extern u8 D_8014313C[5]; /* Forty occupancy bits; save/load transfer five bytes. */
extern Entry D_80143144[40];
extern u8 D_8015488C[8];
extern s32 func_800B101C(u8 *entry);
extern void func_800B0EF4(u8 id);

u8 func_800B09A0(void *source)
{
    Entry *entry = source;
    s32 remaining = 40;
    if (entry->bytes[0]) {
        for (;;) {
            u8 id;
            s32 index;
            u8 *bits;
            if (remaining-- <= 0)
                break;
            id = D_80143114[remaining];
            index = id - 1;
            bits = &D_8014313C[index >> 3];
            if (!(*bits & D_8015488C[index & 7])) {
                s32 reorder;
                D_80143144[index] = *entry;
                *bits |= D_8015488C[index & 7];
                reorder = func_800B101C(entry->bytes) != 1;
                if (reorder)
                    func_800B0EF4(id);
                return id;
            }
        }
    }
    return 0;
}
