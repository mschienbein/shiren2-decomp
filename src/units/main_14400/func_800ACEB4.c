#include "common.h"

typedef struct { unsigned char kind; unsigned char bit; unsigned char flags; } Item80051860;
extern s32 func_800AD468(u32 bit);

s32 func_800ACEB4(Item80051860 *item)
{
    s32 result;
    if (item->flags & 2) {
        result = 2;
    } else if (func_800AD468(item->bit)) {
        switch (item->kind) {
        case 3: case 4: case 6: case 7:
            result = 1;
            break;
        default:
            result = 2;
            break;
        }
    } else {
        result = 0;
    }
    return result;
}
