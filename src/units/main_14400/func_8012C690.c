#include "common.h"

typedef unsigned char u8;

typedef struct {
    u32 flags;
    u8 pad4[0x44 - 0x4];
    s32 id;
    u8 pad48[0x13C - 0x48];
} Entry8012C690;

extern s32 D_801CA6D4;
extern Entry8012C690 *D_801CA6DC;

void func_8012C690(s32 id, u32 mask, u32 bits) {
    Entry8012C690 *entry = D_801CA6DC;
    s32 i;

    for (i = 0; i < D_801CA6D4; i++, entry++) {
        if (entry->id == id) {
            entry->flags = (entry->flags & mask) | bits;
        }
    }
}
