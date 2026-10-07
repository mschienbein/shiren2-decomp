#include "common.h"
typedef struct { s32 x; s32 y; } Vec2i;
extern unsigned char D_80143390;
extern Vec2i D_80143368[];
Vec2i *func_800B50D4(Vec2i *out, unsigned char dir) {
    s32 cur = D_80143390;
    s32 offset = dir - 4;
    unsigned char idx = (cur - offset) % 4;
    Vec2i *p = &D_80143368[idx];
    out->x = p->x;
    out->y = p->y;
    return out;
}
