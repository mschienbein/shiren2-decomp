#include "common.h"
#include "map_rect.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { MapRect geometry; u8 counts[4]; } Obj800B6C14;
extern u8 D_80147620[];
s32 func_800B68B0(Obj800B6C14 *obj);
u16 func_800C58DC(void *rng, u16 range);
MapPos *func_800B6BA0(MapPos *out, Obj800B6C14 *obj);
MapPos *func_800B6A98(MapPos *out, Obj800B6C14 *obj, s32 value);
MapPos *func_800B6C14(MapPos *out, Obj800B6C14 *obj, s32 index) {
    s32 value;
    s32 sum;
    s32 i;

    value = func_800B68B0(obj);
    if (value == 0 || value == obj->counts[index]) {
        func_800B6BA0(out, obj);
        return out;
    }
    value = func_800C58DC(D_80147620, value - obj->counts[index] - 1);
    sum = 0;
    for (i = 0; i < index; i++) {
        sum += obj->counts[i];
    }
    if (value >= sum) {
        value += obj->counts[index];
    }
    func_800B6A98(out, obj, value);
    return out;
}
