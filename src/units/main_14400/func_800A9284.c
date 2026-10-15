#include "map_unit_iterator.h"
typedef unsigned char u8;
typedef short s16;
typedef struct { s16 delta, index; s32 (*test)(void *); } TestEntry;
typedef struct { u8 pad00[0x10]; TestEntry test10; } Vtable;
typedef struct Unit { u8 pad00[0x1E]; u8 flags1E; u8 pad1F[5]; Vtable *vtable; u8 pad28[0xBC]; } Unit;
extern u8 D_801C51A4[];
extern const u8 D_8015488C[8];
/* Complete thirty-slot table (including fallback) and 0x10C-byte player object. */
extern Unit D_801C36EC[30];
typedef struct { Unit base; u8 player_fields[0x28]; } Player;
extern Player D_801C35E0;
extern s32 func_800A24DC(void *, MapRect *);
extern s32 func_800A650C(Unit *, MapPos *);
/* ODD_C: separate the occupied-slot test and mask predicate from iterator control. */
static inline u8 occupied(u8 *bits, s32 index) { return bits[index >> 3] & D_8015488C[index & 7]; }
static inline u8 nonzero(u32 value) { return value != 0; }
s32 func_800A9284(MapUnitIterator *iterator, s32 mask)
{
    Unit *unit;
    s32 eligible, intersects;
    for (;;) {
        Unit *unit;
        s32 eligible, intersects;
        s32 current = iterator->cursor;
        if (current >= 29) break;
        if (occupied(D_801C51A4, current)) {
            unit = &D_801C36EC[iterator->cursor];
            eligible = unit != iterator->excluded &&
                !unit->vtable->test10.test((u8 *)unit + unit->vtable->test10.delta) &&
                nonzero(unit->flags1E & mask);
            if (eligible) {
                intersects = func_800A24DC(unit, &iterator->area) ||
                    func_800A650C(unit, &iterator->center) == 1;
                if (intersects) return 1;
            }
        }
        iterator->cursor++;
    }
    unit = &D_801C35E0.base;
    eligible = iterator->cursor == 29 && iterator->excluded != unit &&
        !unit->vtable->test10.test((u8 *)unit + unit->vtable->test10.delta) &&
        nonzero(unit->flags1E & mask);
    if (eligible) {
        intersects = func_800A24DC(unit, &iterator->area) ||
            func_800A650C(unit, &iterator->center) == 1;
        if (intersects) return 1;
    }
    return 0;
}
