#include "common.h"

typedef struct { s32 x0; s32 y0; s32 x1; s32 y1; } Rect;
Rect *func_800A324C(Rect *rect, const Rect *arg1, const Rect *arg2);
void func_800A3180(Rect *rect);
void func_800A3500(Rect *rect);
void func_800B7A4C(s32 arg0, Rect *rect, s32 flip);
void func_800B7C88(s32 arg0, const Rect *arg1, const Rect *arg2);
void func_800B7E9C(s32 arg0, const Rect *arg1, const Rect *arg2) {
    Rect rect;
    Rect *r;
    s32 flags;
    s32 ordered;
    func_800A324C(&rect, arg1, arg2);
    r = &rect;
    flags = r->y0 > r->y1;
    ordered = rect.x0 <= r->x1;
    if (!ordered) {
        flags |= 2;
    }
    func_800A3180(r);
    switch (flags) {
    case 0:
        break;
    case 1:
        func_800B7A4C(arg0, r, 0);
        break;
    case 2:
        func_800A3500(r);
        func_800B7A4C(arg0, r, 1);
        break;
    case 3:
        func_800B7C88(arg0, arg1, arg2);
        break;
    }
}
