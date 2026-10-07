#include "common.h"
typedef unsigned char u8;
typedef struct { s32 key; s32 group; } Key80042338;
typedef struct { u8 type; char pad1[0xB]; u8 flagsC; } Entry80042338;
Entry80042338 *func_800B4D80(Key80042338 *);
s32 func_801158EC(Entry80042338 *);
s32 func_80042338(s32 group, s32 key) {
    Key80042338 k;
    Entry80042338 *entry;
    s32 result = 1;
    k.group = group;
    k.key = key;
    entry = func_800B4D80(&k);
    if (entry != 0) {
        if (entry->type != 0x10) {
            return result;
        }
        switch (func_801158EC(entry)) {
        case 0:
            result = 1;
            break;
        case 1:
            result = 2;
            break;
        case 2:
            result = 4;
            break;
        }
        if ((entry->flagsC >> 2) & 1) {
            result |= 0x80;
        }
    }
    return result;
}
