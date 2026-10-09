#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct Entry { u16 base_00; u8 count_02; u8 field_03; } Entry;
extern Entry *func_80045134(u8 id);
s32 func_800F4044(s32 id, u8 b) {
    Entry *entry = func_80045134(id);
    if (entry && entry->count_02 >= b) {
        s32 base = entry->base_00;
        s32 offset = b + 0x36C2;
        return base + offset;
    }
    return 0;
}
