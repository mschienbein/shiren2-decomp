#include "common.h"
#include "item_records.h"
#include "map_rect.h"

typedef signed char s8;

/* Floor-item grid: a pool handle (consumed by func_800D4A28), then the busy flag at +8. */
typedef struct { Handle base; s8 busy; } Obj800D4D8C;
s32 func_800D4A10(Obj800D4D8C *obj);
MapPos *func_800D4B60(MapPos *pos, Obj800D4D8C *obj, s32 index);
void *func_800B4E18(MapPos *pos);
void func_800D4C34(Obj800D4D8C *obj);
void func_800D4D8C(Obj800D4D8C *obj) {
    s32 i;
    if (obj->busy == 0) {
        i = func_800D4A10(obj) - 1;
        while (1) {
            MapPos pos;
            if (i < 0) {
                break;
            }
            func_800D4B60(&pos, obj, i);
            func_800D4A28(&obj->base, i, func_800B4E18(&pos));
            i--;
        }
        func_800D4C34(obj);
    }
}
