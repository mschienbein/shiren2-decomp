#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Vec2i;
void *func_800A8CB0(s32 cell);
void func_8004194C(s32 id, s32 *outY, s32 *outX) {
    Vec2i *src = func_800A8CB0((u8)id);
    Vec2i v;
    v.x = src->x;
    v.y = src->y;
    *outY = v.y;
    *outX = v.x;
}
