#include "common.h"
typedef unsigned char u8;
typedef struct { u8 value; } Dir;
typedef struct { s32 x, y; } Point;
typedef struct { Point field_0; } Object;
extern unsigned char D_80142F20;
extern unsigned char func_800A6420(Object *, Object *);
extern s32 func_800A692C(Object *, s32), func_800A4520(Object *, void *), func_800E20CC(Object *);
extern void *func_800A6CF0(Object *);
extern s32 func_800A23E8(Point *, Point *);
extern Dir *func_800A22B8(Dir *out, Point *from, Point *to);
extern void func_800A665C(Object *, const Dir *);
static inline void position(Point *out, Object *arg) { out->x = arg->field_0.x; out->y = arg->field_0.y; }
static inline s32 is_special_floor(void) { return D_80142F20 == 0x4F; }
s32 func_800F1244(Object *arg, Object *target, s32 distance, s32 force)
{
    unsigned char relation = func_800A6420(arg, target);
    s32 flag;
    if (!force && func_800A692C(arg, 0x12)) {
        flag = 0;
        if (!relation || func_800A4520(arg, func_800A6CF0(arg)) || func_800E20CC(arg)) flag = 1;
        if (flag) return 1;
        return 2;
    }
    flag = 0;
    if (func_800E20CC(arg) || is_special_floor()) flag = 1;
    if (!flag) {
        if (relation == 3) return 2;
        {
            Point a, b, copy;
            position(&a, arg);
            position(&b, target);
            copy.x = b.x;
            copy.y = b.y;
            if (func_800A23E8(&a, &copy) > distance) {
                Dir delta;
                func_800A22B8(&delta, &a, &b);
                func_800A665C(arg, &delta);
                return 2;
            }
        }
    }
    return 0;
}
