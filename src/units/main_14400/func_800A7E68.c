#include "common.h"

typedef struct { s32 x, y; } Point;
extern void *func_800A6538(void *out_direction, void *object, void *target_position);
extern void func_800A6690(void *, Point *, s32);

void func_800A7E68(void *object, void *target_position, s32 mode) {
    Point point;
    Point *temporary = &point;
    func_800A6538(temporary, object, target_position);
    func_800A6690(object, temporary, mode);
}
