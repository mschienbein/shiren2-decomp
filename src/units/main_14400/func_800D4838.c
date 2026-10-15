#include "common.h"
#include "map_position_iterator.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad[0x1E]; u8 flags1E; } Entity;
typedef struct { char pad[0xE4]; u16 flagsE4; } Player;
typedef struct { MapRect *area; s32 kind; s32 unk8; s32 unkC; } Obj;
extern u32 D_8013960C;
extern Player *D_801476B8;
const u16 D_80154808[10] = {
    0x1BE, 0x1BF, 0x1C0, 0x1C1, 0x1C2, 0x1C3, 0x1C4, 0x1C5, 0x1C6, 0x1C7,
};
s32 func_80049CB4(s32 id, ...);
Entity *func_800B4928(MapPos *);
void func_800E20F0(Entity *);
void func_800498E4(s32 message_id, ...);
static inline void Pos_set(MapPos *dst, MapPos *src) {
    dst->x = src->x;
    dst->y = src->y;
}
static inline void Rect_set(MapRect *dst, MapRect *src) {
    Pos_set(&dst->min, &src->min);
    Pos_set(&dst->max, &src->max);
}
static inline void RectIter_init(MapPositionIterator *it, MapRect *rect) {
    MapPos tmp;
    Pos_set(&tmp, &rect->min);
    it->start = tmp;
    it->cur = it->start;
    Pos_set(&tmp, &rect->max);
    it->end = tmp;
}
static inline s32 RectIter_valid(MapPositionIterator *it) {
    return it->cur.x <= it->end.x;
}
void func_800D4838(Obj *obj) {
    s32 clear;
    func_80049CB4(0x126, obj->kind == 0 ? 0x22 : 0x23);
    D_8013960C <<= 1;
    clear = (D_801476B8->flagsE4 & 1) ^ 1;
    if (clear) {
        MapRect rect;
        MapPositionIterator it;
        Rect_set(&rect, obj->area);
        RectIter_init(&it, &rect);
        while (RectIter_valid(&it)) {
            MapPos at;
            Entity *entity;
            func_800A3610(&at, &it);
            entity = func_800B4928(&at);
            if (entity != 0 && !(entity->flags1E & 3)) {
                func_800E20F0(entity);
            }
        }
    }
    obj->unk8 = 1;
    obj->unkC = 1;
    D_8013960C >>= 1;
    func_80049CB4(0xDB);
    func_800498E4(D_80154808[obj->kind]);
    func_80049CB4(2);
}
