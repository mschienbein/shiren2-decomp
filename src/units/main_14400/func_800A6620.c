#include "common.h"

typedef struct { s32 x; s32 y; } Vec2;
extern void *func_800A27E8(void *, void *, Vec2 *);
void *func_800A6620(void *out, void *from, Vec2 *p) {
    Vec2 tmp;
    Vec2 *t = &tmp;
    t->x = p->x;
    t->y = p->y;
    func_800A27E8(out, from, t);
    return out;
}
