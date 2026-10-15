#include "common.h"
#include "map_position_iterator.h"
typedef unsigned char u8;
typedef signed char s8;
typedef struct { u8 field_00[0x18]; short field_18, field_1A; s32 (*field_1C)(void *, s32); } VTable;
typedef struct { u8 kind, subtype, flags, state; u8 field_04[4]; VTable *table; } Item;
extern void *D_801476B8, *D_80147FE0;
extern MapRect *D_80148080;
extern MapPos D_80148000;
extern u8 D_80148008[];
extern void *func_800B4D80(MapPos *position);
extern s32 func_800A41EC(void *object, void *position);
extern s32 func_800EC630(void *object, Item *item);
extern s32 func_800D6AF8(void *map, void *filter, MapPos *position);
extern s32 func_800D6D04(void *map, MapPos *position, MapPos *best);
static inline MapPos *copy_position(MapPos *out, MapPos *in) { out->x=in->x; out->y=in->y; return out; }
/* Kind 0x10 is constructed by func_801188D0 with D_8015E0A8; slot 0x1C targets func_8011702C. */
static inline s32 active(MapPositionIterator *iterator) {
    return (iterator->cur.x > iterator->end.x) ^ 1;
}
s32 func_800D68EC(MapPos *out, s8 *flag) {
    MapPositionIterator iterator;
    MapPos position;
    void *world = D_801476B8;
    out->x = 0;
    out->y = 0;
    iterator.start = *copy_position(&position, &D_80148080->min);
    iterator.cur = iterator.start;
    iterator.end = *copy_position(&position, &D_80148080->max);
    while (active(&iterator)) {
        Item *item;
        u8 kind, subtype;
        func_800A3610(&position, &iterator);
        item = func_800B4D80(&position);
        if (!item) continue;
        kind = item->kind;
        subtype = item->subtype;
        if ((func_800A41EC(D_80147FE0, &position) ^ 1) == 0 && item->state == 2 && subtype != 0xF4) {
            if ((func_800EC630(world, item) ^ 1) == 0) {
                if (kind == 0x10) {
                    VTable *table = item->table;
                    if (table->field_1C((u8 *)item + table->field_18, 0x23)) continue;
                }
                if (kind != 0xA && func_800D6AF8(&D_80148000, D_80148008, &position)) {
                    s32 selected = 0;
                    if ((out->y | out->x) == 0 || func_800D6D04(&D_80148000, &position, out)) selected = 1;
                    if (selected) *out = position;
                    *flag = 0;
                }
            }
        }
    }
    return (out->y | out->x) != 0;
}
