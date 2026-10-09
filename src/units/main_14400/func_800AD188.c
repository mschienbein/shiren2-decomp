#include "common.h"
typedef unsigned char u8;
typedef struct { u8 data[11]; } Entry11;
extern Entry11 D_80156BA0[];
extern u8 D_8014303C;
void func_800AD188(u8 kind, u8 *chance, u8 *enabled) {
    Entry11 *entry;
    entry = &D_80156BA0[D_8014303C];
    *chance = 100;
    *enabled = 0;
    switch (kind) {
    case 1: *chance = entry->data[7]; break;
    case 2: *chance = entry->data[6]; break;
    case 3: *enabled = entry->data[0]; break;
    case 4: *enabled = entry->data[1]; break;
    case 6: *chance = entry->data[4]; *enabled = entry->data[5]; break;
    case 7: *chance = entry->data[2]; *enabled = entry->data[3]; break;
    case 9: *chance = entry->data[8]; break;
    case 10: *chance = entry->data[10]; break;
    }
}
