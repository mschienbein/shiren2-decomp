#include "common.h"
#include "collection_iterator.h"
#include "map_rect.h"
#include "map_position_iterator.h"
typedef unsigned char u8;
typedef signed char s8;
typedef struct Ent { u8 kind, id; } Ent;
/* Content-list iterator (func_800CEB20/func_800CEBA0/func_800CEC68): index, list, direction, current entry. */
typedef Ent S;
typedef struct { s32 field_0; MapRect *field_4; s32 field_8, field_C, field_10; } Obj;
/* Whole 16-byte map rectangle (first corner +0, last corner +8). */
extern MapRect D_801429C0;
extern void *func_800B4D80(MapPos *pos);
extern s32 func_800AE9AC(Ent *e, s32 a1, s32 a2);
extern s32 func_800D2BD4(Obj *obj);
extern void func_800AE974(S *s, s8 value);
extern void *func_8011422C(u8 *obj);
static inline void copy_rect(MapRect *dst, MapRect *src) {
    dst->min = src->min;
    dst->max = src->max;
}
void func_800D2D94(Obj *obj) {
    MapRect bounds;
    MapPositionIterator iter;
    MapPos pos;
    CollectionIterator contents;
    MapPos *position;
    MapRect *region = obj->field_4;
    obj->field_10 = 0;
    if (!region) {
        copy_rect(&bounds, &D_801429C0);
    } else {
        copy_rect(&bounds, region);
    }
    position = &pos;
    pos.x = bounds.min.x;
    pos.y = bounds.min.y;
    iter.start = pos;
    iter.cur = iter.start;
    pos.x = bounds.max.x;
    pos.y = bounds.max.y;
    iter.end = pos;
    while (1) {
        Ent *item;
        u8 kind, id;
        s32 valid = iter.cur.x <= iter.end.x;
        if (!valid) break;
        func_800A3610(position, &iter);
        item = func_800B4D80(position);
        if (!item) continue;
        id = item->id;
        if (id == 0xF2) continue;
        kind = item->kind;
        if (kind != 0x10 && func_800AE9AC(item, 0, 0)) func_800AE974(item, (s8)func_800D2BD4(obj));
        if (kind != 9 || id == 0xAB) continue;
        func_800CEB20(&contents, func_8011422C((u8 *)item));
        while (func_800CEBA0(&contents)) {
            s32 eligible;
            item = func_800CEC68(&contents);
            eligible = item->kind != 0x10 && func_800AE9AC(item, 0, 0);
            if (eligible) func_800AE974(item, (s8)func_800D2BD4(obj));
        }
    }
}
