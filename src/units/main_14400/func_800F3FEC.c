#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct Entry800F3FEC {
    u16 base_00;
    u8 count_02;
} Entry800F3FEC;

Entry800F3FEC *func_80045134(u8 id);

/* Message id for entry `id`, item `index` (0 when absent or out of range). */
s32 func_800F3FEC(u8 id, u8 index) {
    Entry800F3FEC *entry = func_80045134(id);

    if (entry != 0) {
        s32 offset;

        if (entry->count_02 < index) {
            return 0;
        }
        offset = index + 0x36B0;
        return entry->base_00 + offset;
    }
    return 0;
}
