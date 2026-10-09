#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct { s32 x, y; } Position;
typedef struct { Position a, b; } Rect;
typedef struct { s16 delta, index; s32 (*test)(void *); } TestEntry;
typedef struct { u8 pad00[0x10]; TestEntry test10; } Vtable;
typedef struct Unit { u8 pad00[0x1E]; u8 flags1E; u8 pad1F[5]; Vtable *vtable; u8 pad28[0xBC]; } Unit;
typedef struct { s32 current; Unit *excluded; Position point08; Rect rect10; } Iterator;
extern u8 D_801C51A4[];
extern const u8 D_8015488C[8];
/* Complete thirty-slot table (including fallback) and 0x10C-byte player object. */
extern Unit D_801C36EC[30];
typedef struct { Unit base; u8 player_fields[0x28]; } Player;
extern Player D_801C35E0;
extern s32 func_800A24DC(void *, Rect *);
extern s32 func_800A650C(Unit *, Position *);
/* ODD_C: separate the occupied-slot test and mask predicate from iterator control. */
static inline u8 occupied(u8 *bits, s32 index) { return bits[index >> 3] & D_8015488C[index & 7]; }
static inline u8 nonzero(u32 value) { return value != 0; }
s32 func_800A9284(Iterator *iterator, s32 mask)
{
    Unit *unit;
    s32 eligible, intersects;
    for (;;) {
        Unit *unit;
        s32 eligible, intersects;
        s32 current = iterator->current;
        if (current >= 29) break;
        if (occupied(D_801C51A4, current)) {
            unit = &D_801C36EC[iterator->current];
            eligible = unit != iterator->excluded &&
                !unit->vtable->test10.test((u8 *)unit + unit->vtable->test10.delta) &&
                nonzero(unit->flags1E & mask);
            if (eligible) {
                intersects = func_800A24DC(unit, &iterator->rect10) ||
                    func_800A650C(unit, &iterator->point08) == 1;
                if (intersects) return 1;
            }
        }
        iterator->current++;
    }
    unit = &D_801C35E0.base;
    eligible = iterator->current == 29 && iterator->excluded != unit &&
        !unit->vtable->test10.test((u8 *)unit + unit->vtable->test10.delta) &&
        nonzero(unit->flags1E & mask);
    if (eligible) {
        intersects = func_800A24DC(unit, &iterator->rect10) ||
            func_800A650C(unit, &iterator->point08) == 1;
        if (intersects) return 1;
    }
    return 0;
}
