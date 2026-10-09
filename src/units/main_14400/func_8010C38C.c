#include "common.h"
typedef struct { unsigned char field_00; unsigned char kind_01; } Item;
s32 func_8010C38C(Item *item) {
    switch (item->kind_01) {
    case 0x39:
    case 0x44:
    case 0x49:
    case 0x4B:
    case 0x4C:
    case 0x4D:
    case 0x58:
    case 0x6D:
        return 1;
    default:
        return 0;
    }
}
