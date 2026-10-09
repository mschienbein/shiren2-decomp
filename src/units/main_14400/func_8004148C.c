#include "common.h"

typedef struct {
    s32 field_00;
    s32 field_04;
} Coordinates;
extern u32 func_800B1C6C(Coordinates *);

s32 func_8004148C(s32 x, s32 y) {
    Coordinates point;
    point.field_04 = x;
    point.field_00 = y;
    if (func_800B1C6C(&point) & 0x1000) {
        return 1;
    }
    return 0;
}
