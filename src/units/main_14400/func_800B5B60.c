#include "common.h"

/* One of the two 0x18-byte region records initialized by func_800B1080: owner byte +0
 * (func_800D1D90 clears it with sb zero,0(a0)) plus 3 padding bytes, area pointer +4,
 * remaining bytes not interpreted here. */
typedef struct {
    signed char owner_00;
    unsigned char pad_01[3];
    void *field_04;
    char reserved_08[0x10];
} Entry;
extern Entry D_80143330[2];
extern unsigned char D_80143391;
extern s32 D_80143444;
extern unsigned char D_80143448;

static inline s32 in_bounds(s32 index, s32 count) {
    return index < count;
}

s32 func_800B5B60(void *value) {
    s32 i;
    s32 count;
    Entry *entry;
    if (value == 0 || D_80143444 == 0) {
        return 0;
    }
    i = 0;
    count = D_80143448;
    entry = D_80143330;
    while (in_bounds(i, count)) {
        if (entry->field_04 == 0) {
            if (D_80143391 & 4) {
                return 1;
            }
            return 0;
        }
        if (entry->field_04 == value) {
            return 1;
        }
        entry++;
        i++;
    }
    return 0;
}
