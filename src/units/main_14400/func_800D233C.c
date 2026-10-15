#include "common.h"
#include "map_rect.h"
/* Region record (D_80143330): owner byte +0 plus 3 padding bytes, area pointer +4. */
typedef struct { signed char owner_0; unsigned char pad_1[3]; void *field_4; } Object;
extern s32 func_800B68B0(void *);
extern MapPos *func_800B6A98(MapPos *out, void *room, s32 index);
extern void func_800D2260(void *, MapPos *);
void func_800D233C(Object *arg) {
    MapPos item;
    s32 i = 0;
    s32 count = func_800B68B0(arg->field_4);
    for (; i < count; ++i) {
        func_800B6A98(&item, arg->field_4, i);
        func_800D2260(arg, &item);
    }
}
