#include "common.h"
typedef struct { s32 x, y; } Position;
extern void *func_800A6CC0(void *out_position, void *object);
extern void *func_800B4D80(Position *position);
void *func_800A6D18(void *object) {
    Position position;
    func_800A6CC0(&position, object);
    return func_800B4D80(&position);
}
