#include "common.h"

typedef unsigned char u8;
typedef struct { float x, y, z; } Vec3;
typedef struct {
    u8 pad00[0xC];
    signed short x:14; unsigned short flags0C:2;
    unsigned short field0E;
    signed short z:14; unsigned short flags10:2;
} Unit8004D170;
extern Unit8004D170 *func_8007946C(s32 side, s32 slot);
extern void func_80059AA8(float *v);
extern s32 D_8016539C, D_80165448;
extern Vec3 D_80165300;

void func_8005C4AC(Vec3 *position, Vec3 *copy) {
    s32 slot = D_8016539C;
    Unit8004D170 *unit;
    if (slot >= 0 && (unit = func_8007946C(0, slot)) != 0) {
        position->x = unit->x;
        position->z = unit->z;
        position->y = 0;
        *copy = *position;
        if (D_80165448 != 0) func_80059AA8(&position->x);
    } else {
        position->x = D_80165300.x;
        position->z = D_80165300.z;
        position->y = 0;
        *copy = *position;
    }
}
