#include "common.h"

typedef struct { s32 field00; s32 field04; } Position;
extern u32 func_800B1C6C(Position *);
s32 func_800414B8(s32 x, s32 y) {
    Position position;
    position.field04 = x;
    position.field00 = y;
    return (s32)(func_800B1C6C(&position) & 0x2000) > 0;
}
