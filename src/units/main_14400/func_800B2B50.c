#include "common.h"
#include "map_position_iterator.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { u8 pad_00[0x10]; s16 delta_10; s16 pad_12; void (*finish)(void *); s16 delta_18; s16 pad_1A; void (*start)(void *); } WorldTable;
typedef struct { u8 pad_00[8]; WorldTable *field_08; } World;
typedef struct { u8 pad_00[0x1E]; u8 field_1E; } Unit;
typedef struct { u8 kind, id; } Tile;
extern World *D_80142B10;
/* Rectangle at 0x801429C0: actual min and max MapPos subobjects. */
extern MapRect D_801429C0;
extern u32 D_8013960C;
extern int func_800413E0(void);
extern void func_800413FC(void);
extern void *func_800B4D80(MapPos *);
extern s32 func_80049CB4(s32, ...);
extern s32 func_800A8F6C(s32 *);
extern void *func_800A910C(s32 *);
extern void func_800E4988(Unit *);
extern Unit *func_800C5F60(void);

/* Whole-rectangle corner readers, integrated at their call sites. */
static __inline__ s32 rectX0(MapRect *rect) { return rect->min.x; }
static __inline__ s32 rectY0(MapRect *rect) { return rect->min.y; }
static __inline__ s32 rectX1(MapRect *rect) { return rect->max.x; }
static __inline__ s32 rectY1(MapRect *rect) { return rect->max.y; }

static __inline__ s32 has_next(MapPositionIterator *iterator, s32 row) {
    s32 present = 1;
    if (row > iterator->end.x) {
        present = 0;
    }
    return present;
}

void func_800B2B50(void) {
    MapPositionIterator iterator;
    MapPositionIterator *cursor = &iterator;
    MapPos position;
    s32 index;
    World *world;
    func_800413E0();
    world = D_80142B10;
    world->field_08->start((u8 *)world + world->field_08->delta_18);
    world->field_08->finish((u8 *)world + world->field_08->delta_10);
    position.x = rectX0(&D_801429C0);
    position.y = rectY0(&D_801429C0);
    iterator.start = position;
    iterator.cur = iterator.start;
    position.x = rectX1(&D_801429C0);
    position.y = rectY1(&D_801429C0);
    iterator.end = position;
    for (;;) {
        Tile *tile;
        if (!has_next(cursor, iterator.cur.x)) {
            break;
        }
        func_800A3610(&position, cursor);
        tile = func_800B4D80(&position);
        if (tile && tile->id == 0xD5) {
            func_80049CB4(0xE2, &position);
        }
    }
    index = 0;
    while (func_800A8F6C(&index)) {
        Unit *unit = func_800A910C(&index);
        func_80049CB4(0x86, unit);
        func_80049CB4(0x88, unit);
        D_8013960C = (D_8013960C << 1) & ~1U;
        func_80049CB4(0x1F, unit);
        D_8013960C >>= 1;
        func_80049CB4(0x9D, unit);
        if (unit->field_1E & 0x7C) {
            func_800E4988(unit);
        }
    }
    {
        Unit *unit = func_800C5F60();
        func_80049CB4(0x93, unit);
        func_80049CB4(0xDD);
        func_80049CB4(0x89, unit);
        func_80049CB4(0xDB);
        func_800413FC();
        func_80049CB4(2);
    }
}
