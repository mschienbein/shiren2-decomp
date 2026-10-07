#include "common.h"

typedef struct { s32 x; s32 y; } Vec2i;
extern Vec2i D_801C9BC0;
extern Vec2i D_801C9BB0;
extern Vec2i D_801C9BB8;
static inline void set_vec(Vec2i *v, s32 x, s32 y) {
    v->x = x;
    v->y = y;
}
void func_800C0994(void) {
    Vec2i pos;
    Vec2i size;
    set_vec(&D_801C9BC0, 2, 2);
    set_vec(&size, 0x1E, 0x27);
    set_vec(&pos, D_801C9BC0.x + 0x16, D_801C9BC0.y + 0x22);
    D_801C9BB0 = pos;
    D_801C9BB8 = size;
}
