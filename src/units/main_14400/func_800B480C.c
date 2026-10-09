#include "common.h"
typedef struct { s32 field00; s32 field04; } Position;
extern unsigned short D_80143450[][76];
void func_800B480C(Position *position) {
    s32 outside = 0;
    if (position->field04 >= 76 || position->field00 >= 54 || position->field04 < 0 || position->field00 < 0) {
        outside = 1;
    }
    if (!outside) {
        D_80143450[position->field00][position->field04] = 0x2200;
    }
}
