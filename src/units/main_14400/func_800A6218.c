#include "common.h"
typedef struct { s32 x, y; } Point;
typedef struct { short offset; short field_2; void (*method)(void *); } Method;
typedef struct { Point position; unsigned char field_8, field_9; unsigned char field_A[0x1A]; Method *field_24; } Object;
extern s32 func_800A5D2C(Object *, Point *, s32);
extern s32 func_800A251C(Point *, Point *);
extern s32 func_80049CB4(s32, ...);
extern void func_800A58FC(Object *, Point *);
extern u32 func_800B1C6C(Point *);
extern s32 func_800A60D8(Object *, Point *);
extern s32 func_800A5168(Object *, Point *, s32);
static inline Point *copy_point(Point *dest, Point *src) { dest->x = src->x; dest->y = src->y; return dest; }
s32 func_800A6218(Object *self, Point *position, s32 notify) {
    Point local;
    Point *point = copy_point(&local, position);
    if (func_800A5D2C(self, point, 1)) {
        s32 old_state, new_state;
        if (notify) { s32 different = func_800A251C(point, position) ^ 1; if (different) func_80049CB4(0x8C, self, position, point); }
        old_state = self->field_9 & 15;
        func_800A58FC(self, &local);
        new_state = self->field_9 & 15;
        if (old_state != new_state && new_state == 1) {
            Point *updated = &local;
            if (func_800B1C6C(updated) & 0x2000) func_80049CB4(0x10B, updated);
        }
    } else {
        if (!func_800A60D8(self, point)) {
            self->field_24[3].method((unsigned char *)self + self->field_24[3].offset);
            return 0;
        }
        self->position = *position;
        func_800A5168(self, point, 0);
    }
    return 1;
}
