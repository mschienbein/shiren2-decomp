#include "common.h"

typedef float f32;

typedef struct {
    f32 x, y, z;
} Vec3;

extern Vec3 D_80165300;
extern Vec3 D_8016530C;
extern Vec3 D_8016534C;
extern Vec3 D_80165358;

/* Mode 0: reset origin to pos with zero offset; modes 1/2: offset of pos from a stored anchor. */
void func_800594C0(Vec3 *pos, s32 mode) {
    Vec3 *anchor;
    f32 dz;

    switch ((u32)mode) {
    case 0:
        D_80165300 = *pos;
        D_8016530C.x = 0.0f;
        D_8016530C.y = 0.0f;
        D_8016530C.z = 0.0f;
        return;
    case 1:
        anchor = &D_8016534C;
        break;
    case 2:
        anchor = &D_80165358;
        break;
    default:
        return;
    }
    D_8016530C.x = pos->x - anchor->x;
    D_8016530C.y = pos->y - anchor->y;
    dz = pos->z - anchor->z;
    D_80165300 = *anchor;
    D_8016530C.z = dz;
}
