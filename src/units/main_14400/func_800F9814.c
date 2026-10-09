#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { s32 x, y; } Coord;
typedef struct Object Object;
typedef struct {
    u8 pad0[0x20]; s16 adjust20, pad22;
    s32 (*count)(void *);
} ListTable;
typedef struct { u8 pad0[4]; ListTable *table; } List;
struct Object {
    Coord position;
    u8 pad8[0x16]; u8 kind; u8 pad1F[0x39];
    Object *target; u8 pad5C[8];
    Coord goal; u8 pad6C[6]; u8 flags72; u8 pad73[0x19];
    List *items; u8 pad90[0xA]; u16 flags9A;
};
typedef struct { s32 index; Object *source; Coord position; s32 bounds[4]; } Iterator;
typedef union {
    Iterator scan;
    struct { Coord target, closest; } path;
} Work;
typedef struct { u8 kind; } Tile;
typedef struct { u8 value; } Dir;
extern s32 func_800E7794(void *);
extern void *func_800B4D80(Coord *);
extern u8 func_800A6420(Object *, Object *);
extern void func_800F06E4(Object *);
extern Dir *func_800A22B8(Dir *, Coord *, Coord *);
extern void func_800A665C(Object *, u8 *);
extern Iterator *func_800A915C(Iterator *, Object *);
extern s32 func_800A9284(Iterator *, s32);
extern Object *func_800A942C(Iterator *);
extern s32 func_800A4520(void *, Object *);
extern u32 func_800B1C6C(Coord *);
extern s32 func_800B4FF0(void *, s32);
extern void *func_800B36C4(void *, Coord *, u8, s32);
extern s32 func_800E7104(Object *);
extern s32 func_800E66EC(Object *);

static inline s32 needsSearch(Coord *c) { return !(c->y | c->x) || (!(func_800B1C6C(c) & 0x800) && !func_800B4FF0(c, 14)); }

/* Unit vtable slot (D_80159440 family): s32 (*)(void *self). */
s32 func_800F9814(void *self)
{
    Coord position;
    Work work;
    Dir direction;
    Object *target;
    Tile *tile;
    s32 hasItems, onTile, pursue, searchPoint;
    ((Object *)self)->flags72 &= ~2;
    hasItems = ((Object *)self)->items->table->count((u8 *)((Object *)self)->items +
        ((Object *)self)->items->table->adjust20) && !(((Object *)self)->flags9A & 0x40);
    if (hasItems) {
        ((Object *)self)->flags72 |= 2;
        return func_800E7794(self);
    }
    {
        Coord *where = &position;
        target = ((Object *)self)->target;
        position.x = ((Object *)self)->position.x;
        where->y = ((Object *)self)->position.y;
        tile = func_800B4D80(where);
    }
    onTile = tile && tile->kind == 14 && !(((Object *)self)->flags9A & 0x40);
    if (onTile) {
        switch (func_800A6420(self, target)) {
        case 0:
            func_800F06E4(self);
            break;
        case 1:
        case 2:
            {
                Dir *dir = &direction;
                func_800A22B8(dir, &position, &target->position);
                func_800A665C(self, &dir->value);
            }
            break;
        }
        return 0;
    }
    pursue = (((Object *)self)->flags9A & 0x40) && func_800A6420(self, target) != 3;
    if (pursue) {
        if (!(target->kind & 0x7C)) {
            pursue = 0;
        } else if (target->flags72 & 8) {
            pursue = 1;
        } else {
            pursue = 0;
        }
        if (pursue) {
            goto move;
        }
        ((Object *)self)->target = 0;
        {
            Iterator *iterator;
            func_800A915C(&work.scan, self);
            iterator = &work.scan;
            while (func_800A9284(iterator, 0x7C)) {
                Object *candidate = func_800A942C(iterator);
                Object *chosen = candidate;
                s32 suitable = (candidate->flags72 & 8) && func_800A4520(self, candidate);
                if (suitable) {
                    ((Object *)self)->target = chosen;
                }
            }
        }
        if (!((Object *)self)->target) {
            goto idle;
        }
    }
    work.path.target.x = ((Object *)self)->goal.x;
    work.path.target.y = ((Object *)self)->goal.y;
    searchPoint = needsSearch(&work.path.target);
    if (searchPoint) {
        func_800B36C4(&work.path.closest, &position, 14, 0);
        work.path.target = work.path.closest;
        if (work.path.target.y | work.path.target.x) {
            ((Object *)self)->goal = work.path.target;
        }
    }
    if (func_800B4FF0(&work.path.target, 14)) {
        goto idle;
    }
move:
    return func_800E7104(self);
idle:
    return func_800E66EC(self);
}
