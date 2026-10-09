#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef struct { s32 x; s32 y; } Point;
typedef struct { Point begin; Point end; } Rect;
typedef struct { Point current; Point begin; Point end; } RectIter;
typedef struct { s32 index; } EntityIter;
typedef struct { u8 pad_0[0x98]; short delta_98; short pad_9A; void *(*field_9C)(void *); } Methods;
typedef struct { u8 pad_0[0x24]; Methods *methods; u8 pad_28[0x64]; void *field_8C; } Entity;
typedef struct { u8 kind; u8 pad_1[4]; s8 owner; } Item;
typedef struct { s32 field_0; Rect *field_4; s32 field_8; s32 field_C; } Obj;
extern Entity *D_801476B8;
extern Rect D_801429C0;
extern s32 func_800D2BD4(Obj *);
extern void func_800CDCC8(void *, s8);
extern s32 func_800A8FC8(EntityIter *, s32);
extern void *func_800A910C(EntityIter *);
extern Point *func_800A3610(Point *, RectIter *);
extern s32 func_800A31C8(Rect *, Point *);
extern void *func_800B4D80(Point *);
extern void *func_8011422C(u8 *);
static inline void *inventory(Entity *entity) {
    return entity->methods->field_9C((u8 *)entity + entity->methods->delta_98);
}
static inline Point *copy_point(Point *out, Point *in) {
    out->x = in->x;
    out->y = in->y;
    return out;
}
static inline void init_rect(RectIter *it, Rect *bounds, Point *point) {
    it->begin = *copy_point(point, &bounds->begin);
    it->current = it->begin;
    it->end = *copy_point(point, &bounds->end);
}
static inline s32 has_next(RectIter *it) {
    return it->current.x <= it->end.x;
}
void func_800D31EC(Obj *obj) {
    EntityIter entities;
    s32 owner;
    obj->field_C = 0;
    owner = func_800D2BD4(obj);
    func_800CDCC8(inventory(D_801476B8), (s8)owner);
    entities.index = 0;
    while (func_800A8FC8(&entities, 8)) {
        void *list = inventory(func_800A910C(&entities));
        if (list) func_800CDCC8(list, (s8)owner);
    }
    entities.index = 0;
    while (func_800A8FC8(&entities, 16)) {
        Entity *entity = func_800A910C(&entities);
        if (entity->field_8C) func_800CDCC8(entity->field_8C, (s8)owner);
    }
    if (obj->field_4) {
        RectIter it;
        Point point;
        init_rect(&it, &D_801429C0, &point);
        while (has_next(&it)) {
            Item *item;
            func_800A3610(&point, &it);
            if (func_800A31C8(obj->field_4, &point)) continue;
            item = func_800B4D80(&point);
            if (!item) continue;
            if (item->owner == (s8)owner) item->owner = -1;
            if (item->kind == 9) func_800CDCC8(func_8011422C((u8 *)item), (s8)owner);
        }
    }
}
