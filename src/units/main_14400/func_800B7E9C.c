#include "map_rect.h"

void func_800A3180(MapRect *rect);
void func_800A3500(MapRect *rect);
void func_800B7A4C(void *arg0, MapRect *rect, s32 flip);
void func_800B7C88(void *arg0, const MapRect *arg1, const MapRect *arg2);
void func_800B7E9C(void *arg0, const MapRect *arg1, const MapRect *arg2) {
    MapRect rect;
    MapRect *r;
    s32 flags;
    s32 ordered;
    func_800A324C(&rect, arg1, arg2);
    r = &rect;
    flags = r->min.y > r->max.y;
    ordered = rect.min.x <= r->max.x;
    if (!ordered) {
        flags |= 2;
    }
    func_800A3180(r);
    switch (flags) {
    /* ODD_C: the regions overlap on both axes, so no connecting corridor is
     * needed. This label shapes the compare tree: removing it changes 35
     * overlapping words and grows text from 240 to 248 bytes in this profile. */
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
