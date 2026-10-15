#include "common.h"
#include "map_rect.h"
typedef struct Obj Obj;
extern MapPos *func_800A33DC(MapPos *out, MapRect *bounds);
extern s32 func_800A4314(Obj *obj, MapPos *pos);

s32 func_800A5FCC(Obj *obj, MapPos *out) {
    MapRect bounds;
    MapPos pos;
    s32 count;
    func_800B1F58(&bounds);
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
