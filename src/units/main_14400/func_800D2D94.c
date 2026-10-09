#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef struct { s32 x, y; } Pair;
typedef struct { Pair first, last; } Rect;
typedef struct { Pair current, first, last; } Iter;
typedef struct Ent { u8 kind, id; } Ent;
/* Content-list iterator (func_800CEB20/func_800CEBA0/func_800CEC68): index, list, direction, current entry. */
typedef struct { s32 index; void *owner; s32 reverse; Ent *current; } ListIter;
typedef Ent S;
typedef struct { s32 field_0; Rect *field_4; s32 field_8, field_C, field_10; } Obj;
/* Whole 16-byte map rectangle (first corner +0, last corner +8). */
extern Rect D_801429C0;
extern Pair *func_800A3610(Pair *out, Iter *it);
extern void *func_800B4D80(Pair *pos);
extern s32 func_800AE9AC(Ent *e, s32 a1, s32 a2);
extern s32 func_800D2BD4(Obj *obj);
extern void func_800AE974(S *s, s8 value);
extern void *func_8011422C(u8 *obj);
extern ListIter *func_800CEB20(ListIter *s, void *owner);
extern s32 func_800CEBA0(ListIter *it);
extern Ent *func_800CEC68(ListIter *it);
static inline void copy_rect(Rect *dst, Rect *src) {
    dst->first = src->first;
    dst->last = src->last;
}
void func_800D2D94(Obj *obj) {
    Rect bounds;
    Iter iter;
    Pair pos;
    ListIter contents;
    Pair *position;
    Rect *region = obj->field_4;
    obj->field_10 = 0;
    if (!region) {
        copy_rect(&bounds, &D_801429C0);
    } else {
        copy_rect(&bounds, region);
    }
    position = &pos;
    pos.x = bounds.first.x;
    pos.y = bounds.first.y;
    iter.first = pos;
    iter.current = iter.first;
    pos.x = bounds.last.x;
    pos.y = bounds.last.y;
    iter.last = pos;
    while (1) {
        Ent *item;
        u8 kind, id;
        s32 valid = iter.current.x <= iter.last.x;
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
