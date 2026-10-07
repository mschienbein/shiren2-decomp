#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Vec2i;
s32 func_800B5900(void *, s32, s32, void **);
void func_800AD7A4(u8 *obj, Vec2i *pos, void **arg2) {
    Vec2i tmp;
    Vec2i *p = &tmp;
    tmp.x = pos->x;
    p->y = pos->y;
    func_800B5900(p, obj[3], obj[3], arg2);
}
