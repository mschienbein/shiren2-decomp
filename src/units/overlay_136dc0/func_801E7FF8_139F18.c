#include "func_801E7FF8_139F18/types.h"

extern Obj801F6540 D_801F6540_148460;
extern u32 func_8002A9B0(void);
extern void func_800595DC(f32 *v);

void func_801E7FF8_139F18(void) {
    Obj801F6540 *o = &D_801F6540_148460;
    s32 x = 0;
    s32 z = 0;
    s32 y = 0;
    f32 v[3];

    if (o->x1 != 0) {
        if (o->x0 != 0) {
            o->x0--;
            return;
        }
        if (o->x4 != 0) {
            x = func_8002A9B0() % (o->x4 * 2);
        }
        x -= o->x4;
        if (o->x3 != 0) {
            z = func_8002A9B0() % (o->x3 * 2);
        }
        z -= o->x3;
        if (o->x5 != 0) {
            y = func_8002A9B0() % (o->x5 * 2);
        }
        y -= o->x5;
        v[0] = (f32)x * 0.5;
        v[1] = (f32)y * 0.5;
        v[2] = (f32)z * 0.5;
        func_800595DC(v);
        o->x1--;
    }
}
