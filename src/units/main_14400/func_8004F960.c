#include "common.h"

typedef struct { s32 x, y; } Point;
typedef struct Object Object;

extern s32 D_8013968C;
Object *func_80050EB4(s32 arg0, Point *point, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

/* Spawns effect 0xA3 beside the target, turned one step away from the
 * attacker's heading (directions are 0..7). */
void func_8004F960(Point *target, unsigned char *from, unsigned char *to) {
    s32 facing, previous, direction, sign, dx, dy;

    if (D_8013968C == 0xD5) {
        facing = *to;
        previous = facing;
        if (((*from + 2) & 7) == facing) {
            direction = (facing + 1) & 7;
            sign = 1;
            if (direction < 4) sign = -1;
        } else {
            direction = (previous - 1) & 7;
            sign = -1;
            if (direction < 4) sign = 1;
        }
        dx = 0;
        if (direction & 3) dx = sign * 16;
        dy = 0;
        if (!(direction & 3)) dy = sign * 16;
        func_80050EB4(0xA3, target, direction, 0, dx, dy, 0);
    }
}
