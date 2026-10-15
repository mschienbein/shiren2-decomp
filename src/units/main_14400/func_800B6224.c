#include "common.h"
#include "map_position_iterator.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct Obj800B6224 Obj800B6224;
typedef struct { u8 pad0[0x18]; s16 delta; u8 pad1A[2]; void (*notify)(void *self, s32 cmd, void *data); } VTable800B6224;
struct Obj800B6224 { u8 pad0[0x18]; VTable800B6224 *vtable; };
/* 16-byte map bounds rectangle at 0x801429D0: min then max corner. */
extern MapRect D_801429D0;
extern u8 D_80145460[][0x4C];
extern u8 D_80153B2C[];
void *func_800B4D80(MapPos *pos);
void func_800CA4A4(Obj800B6224 *obj, void *data);

static inline s32 iter_valid(MapPositionIterator *it) {
    return it->cur.x <= it->end.x;
}

static inline void first_point(MapPos *point, MapRect *bounds) {
    point->x = bounds->min.x;
    point->y = bounds->min.y;
}

static inline void last_point(MapPos *point, MapRect *bounds) {
    point->x = bounds->max.x;
    point->y = bounds->max.y;
}

void func_800B6224(Obj800B6224 *obj) {
    MapPositionIterator it;
    MapPos pos;
    u16 count;

    count = 0;
    first_point(&pos, &D_801429D0);
    it.start = pos;
    it.cur = it.start;
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
    it.start = pos;
    it.cur = it.start;
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
