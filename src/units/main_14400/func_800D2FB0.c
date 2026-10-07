#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { Pos cur; Pos start; Pos end; } RectIter;
typedef struct { u8 pad[0x18]; } ListIter;
typedef struct { u8 pad[0x98]; s16 delta; s16 idx; void *(*getItem)(void *); } VTable;
typedef struct { u8 pad0[0x24]; VTable *vt; u8 pad28[0x64]; void *item; u8 pad90[0x70]; u8 *held; } Entity;
typedef struct { u8 pad0[4]; void *map; u8 pad8[4]; s32 base; } Query;
extern Entity *D_801476B8;
typedef struct { Pos start; Pos end; } Rect;
extern Rect D_801429C0;
s32 func_800D2BDC(void *query);
s32 func_800CDBC0(void *item, s32 mode);
s32 func_800A8FC8(s32 *index, s32 kind);
Entity *func_800A910C(s32 *index);
void *func_800A3610(void *out, void *it);
s32 func_800A31C8(void *map, Pos *pos);
void *func_800B4D80(Pos *pos);
s32 func_800AE9AC(void *obj, s32 mode, s32 discount);
void *func_8011422C(void *obj);
void *func_800CEB20(ListIter *it, void *list);
s32 func_800CEBA0(ListIter *it);
void *func_800CEC68(ListIter *it);
static inline void *entityItem(Entity *e) { return e->vt->getItem((u8 *)e + e->vt->delta); }
static inline s32 rectIterValid(RectIter *it) { return it->cur.x <= it->end.x; }
static inline void pos_set(Pos *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
}
s32 func_800D2FB0(Query *q) {
    s32 total = q->base;
    s32 mode = func_800D2BDC(q);
    Entity *e;
    void *item;
    s32 index;
    u8 *held;

    total += func_800CDBC0(entityItem(D_801476B8), mode);
    index = 0;
    while (func_800A8FC8(&index, 8)) {
        item = entityItem(func_800A910C(&index));
        if (item) total += func_800CDBC0(item, mode);
    }
    index = 0;
    while (func_800A8FC8(&index, 0x10)) {
        e = func_800A910C(&index);
        if (e->item) total += func_800CDBC0(e->item, mode);
    }
    if (q->map) {
        RectIter it;
        Pos pos;
        pos_set(&pos, D_801429C0.start.x, D_801429C0.start.y);
        it.start = pos;
        it.cur = it.start;
        pos_set(&pos, D_801429C0.end.x, D_801429C0.end.y);
        it.end = pos;
        while (rectIterValid(&it)) {
            func_800A3610(&pos, &it);
            if (func_800A31C8(q->map, &pos)) continue;
            item = func_800B4D80(&pos);
            if (item) total += func_800AE9AC(item, mode, 0);
        }
    }
    held = D_801476B8->held;
    if (held && *held == 9) {
        ListIter list;
        func_800CEB20(&list, func_8011422C(held));
        while (func_800CEBA0(&list)) {
            total += func_800AE9AC(func_800CEC68(&list), mode, 0);
        }
    }
    return total < 0 ? 0 : total;
}