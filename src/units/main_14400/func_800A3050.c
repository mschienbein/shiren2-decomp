#include "common.h"
typedef struct { s32 x, y; } Pair;
/* Two-corner rectangle (16 bytes; the next rectangle starts at 0x801429C0). */
typedef struct { Pair first, second; } Rectangle;
extern Rectangle D_801429B0;
extern s32 func_800A251C(Pair *a, Pair *b);
s32 func_800A3050(Rectangle *object) {
    s32 equal = 0;
    if (func_800A251C(&object->first, &D_801429B0.first)) {
        equal = func_800A251C(&object->second, &D_801429B0.second) != 0;
    }
    return equal;
}
