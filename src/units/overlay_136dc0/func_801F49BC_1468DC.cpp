#include "func_801F49BC_1468DC/types.h"

extern "C" double func_801E7DE0_139D00(double x, double y);

extern "C" s32 func_801F49BC_1468DC(OvlBUnitHead *unit, OvlBUnitHead *target) {
    OvlBPos from = unit->pos;
    OvlBPos to = target->pos;
    s32 x = from.x - to.x;
    s32 y = to.y - from.y;
    double dx = x;
    s32 angle = (s32)func_801E7DE0_139D00(dx, y);

    if (dx >= 0.0) {
        angle += 22;
    } else {
        angle -= 22;
    }
    angle /= 45;
    angle %= 8;
    if (angle < 0) {
        angle += 8;
    }
    return angle;
}
