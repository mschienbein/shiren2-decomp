#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Point800B6224;
typedef struct { Point800B6224 cur; Point800B6224 begin; Point800B6224 end; } Iter800B6224;
typedef struct Obj800B6224 Obj800B6224;
typedef struct { u8 pad0[0x18]; s16 delta; u8 pad1A[2]; void (*notify)(void *self, s32 cmd, void *data); } VTable800B6224;
struct Obj800B6224 { u8 pad0[0x18]; VTable800B6224 *vtable; };
/* 16-byte map bounds rectangle at 0x801429D0: first then last corner. */
typedef struct { Point800B6224 first; Point800B6224 last; } Bounds800B6224;
extern Bounds800B6224 D_801429D0;
extern u8 D_80145460[][0x4C];
extern u8 D_80153B2C[];
void *func_800A3610(void *out, void *it);
void *func_800B4D80(void *pos);
void func_800CA4A4(Obj800B6224 *obj, void *data);

static inline s32 iter_valid(Iter800B6224 *it) {
    return it->cur.x <= it->end.x;
}

static inline void first_point(Point800B6224 *point, Bounds800B6224 *bounds) {
    point->x = bounds->first.x;
    point->y = bounds->first.y;
}

static inline void last_point(Point800B6224 *point, Bounds800B6224 *bounds) {
    point->x = bounds->last.x;
    point->y = bounds->last.y;
}

void func_800B6224(Obj800B6224 *obj) {
    Iter800B6224 it;
    Point800B6224 pos;
    u16 count;

    count = 0;
    first_point(&pos, &D_801429D0);
    it.begin = pos;
    it.cur = it.begin;
    last_point(&pos, &D_801429D0);
    it.end = pos;
    while (iter_valid(&it)) {
        func_800A3610(&pos, &it);
        if (func_800B4D80(&pos)) {
            count++;
        }
    }
    func_800CA4A4(obj, D_80153B2C);
    obj->vtable->notify((u8 *)obj + obj->vtable->delta, 2, &count);
    first_point(&pos, &D_801429D0);
    it.begin = pos;
    it.cur = it.begin;
    last_point(&pos, &D_801429D0);
    it.end = pos;
    while (iter_valid(&it)) {
        func_800A3610(&pos, &it);
        if (func_800B4D80(&pos)) {
            obj->vtable->notify((u8 *)obj + obj->vtable->delta, 8, &pos);
            obj->vtable->notify((u8 *)obj + obj->vtable->delta, 1, &D_80145460[pos.x][pos.y]);
        }
    }
}
