#include "common.h"
typedef signed char s8;
typedef struct { s32 x, y; } Vec2;
typedef struct { s8 d; } Dir;
extern Vec2 *func_800A25D8(Vec2 *out, Vec2 *p, Dir dir, s32 scale);
void *func_800A39C0(void *self, void *position, unsigned char *direction, s32 distance) {
    Dir dir;
    dir.d = *direction;
    func_800A25D8(self, position, dir, distance);
    return self;
}
