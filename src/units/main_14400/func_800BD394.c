#include "common.h"
#include "map_rect.h"


void func_800B1B58(MapPos *pos, unsigned short flags);
void func_800B1BE0(MapPos *pos, s32 flags);

void func_800BD394(void *self, MapRect *rect) {
    MapPos pos;

    for (pos.x = rect->min.x + 1;; pos.x += 2) {
        s32 xEnd = rect->max.x - 1;

        if (xEnd < pos.x) {
            break;
        }
        for (pos.y = rect->min.y + 1;; pos.y += 2) {
            s32 yEnd = rect->max.y - 1;

            if (yEnd < pos.y) {
                break;
            }
            func_800B1B58(&pos, 0x4000);
            func_800B1BE0(&pos, 0x200);
        }
    }
}
