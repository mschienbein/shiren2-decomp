#include "common.h"
typedef struct { s32 x, y; } Position;
extern Position *D_801476B8;
void func_800418CC(s32 *y, s32 *x) {
    Position position;
    Position *source = D_801476B8;
    position.x = source->x;
    position.y = source->y;
    *y = position.y;
    *x = position.x;
}
