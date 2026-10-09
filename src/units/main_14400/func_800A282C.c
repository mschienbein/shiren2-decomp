#include "common.h"
typedef struct { s32 x, y; } Position;
extern Position *func_800A256C(Position *out, Position *a, Position *b);
extern s32 func_800A24A8(Position *position);
s32 func_800A282C(Position *from, Position *to) {
    Position difference;
    func_800A256C(&difference, to, from);
    return func_800A24A8(&difference);
}
