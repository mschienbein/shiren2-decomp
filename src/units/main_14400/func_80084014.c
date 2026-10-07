#include "common.h"

typedef float f32;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;
void func_800593F8(Vec3f *);
s32 func_80084014(s32 x, s32 z) {
    Vec3f pos;
    s32 cx;
    s32 cz;

    func_800593F8(&pos);
    cx = (s32)pos.x >> 5;
    cz = (s32)pos.z >> 5;
    if (x >= cx - 5 && x <= cx + 5 && z >= cz - 5 && z <= cz + 4) {
        return 1;
    }
    return 0;
}
