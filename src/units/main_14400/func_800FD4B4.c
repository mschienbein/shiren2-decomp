#include "map_unit_iterator.h"
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
typedef struct Unit { Pos pos; } Unit;
typedef struct {
    Pos pos;
    u8 pad08[4];
    MapRect area;
    u8 pad1C[0x6E];
    u8 unk8A;
    u8 pad8B[0x15];
    Unit *targetA0;
} Actor;
extern s32 func_800F069C(void *arg);
extern Unit *func_800C5F60(void);
extern u16 func_800E08F0(void *ptr);
extern u16 func_800E08B0(Unit *unit);
extern u32 func_800B1C6C(Pos *pos);
extern s32 func_800A5D2C(void *object, Pos *output, s32 flags);
extern s32 func_800E1CC4(Unit *obj, s32 kind);
extern s32 func_800A6E90(void *p);
extern s32 func_800A44F4(void *self, void *target);

static inline u32 is_wall(Pos *pos) {
    return func_800B1C6C(pos) & 0x4000;
}

Unit *func_800FD4B4(Actor *self) {
    u16 best;
    Unit *target;
    MapRect area;

    if (func_800F069C(self)) {
        {
            MapRect r;
            func_800A2FD0(&r, func_800C5F60(), self->unk8A);
            area = r;
        }
    } else {
        area = self->area;
    }
    best = 0;
    target = 0;
    {
        MapUnitIterator it;
        func_800A9204(&it, &area, &self->pos);
        while (func_800A9284(&it, 0x7C)) {
            Unit *u = func_800A942C(&it);
            u16 gap = func_800E08F0(u) - func_800E08B0(u);
            Pos pos;
            s32 ok;
            pos.x = u->pos.x;
            pos.y = u->pos.y;
            ok = 0;
            if (best < gap) {
                if (!is_wall(&pos) && func_800A5D2C(self, &pos, 1) && !func_800E1CC4(u, 1)
                    && !func_800A6E90(u) && func_800E08B0(u) != 0) {
                    ok = func_800A44F4(self, u) == 1;
                }
            }
            if (ok) {
                best = gap;
                target = u;
            }
        }
    }
    self->targetA0 = target;
    return target;
}
