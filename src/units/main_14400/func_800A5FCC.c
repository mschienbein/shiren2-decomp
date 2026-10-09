#include "common.h"
typedef struct { s32 x, y; } Pair;
typedef struct { Pair min, max; } Rect;
typedef struct Obj Obj;
extern s32 *func_800B1F58(s32 *out);
extern Pair *func_800A33DC(Pair *out, Rect *bounds);
extern s32 func_800A4314(Obj *obj, Pair *pos);

s32 func_800A5FCC(Obj *obj, Pair *out) {
    Rect bounds;
    Pair pos;
    s32 count;
    func_800B1F58((s32 *)&bounds);
    count = bounds.max.y - bounds.min.y + 1;
    count *= bounds.max.x - bounds.min.x + 1;
    func_800A33DC(&pos, &bounds);
    while (--count != -1) {
        if (func_800A4314(obj, &pos)) {
            *out = pos;
            return 1;
        }
        if (++pos.y > bounds.max.y) {
            pos.y = bounds.min.y;
            pos.x++;
        }
        if (pos.x > bounds.max.x) {
            pos.y = bounds.min.y;
            pos.x = bounds.min.x;
        }
    }
    return 0;
}
