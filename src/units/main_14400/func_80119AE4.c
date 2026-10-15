#include "common.h"
#include "item_records.h"
#include "map_position_iterator.h"

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct { s32 x0; void *position; } Iter;
typedef struct { void *current; short index; } FeatureIter;
typedef struct { void *actor; s32 field_04, field_08; s16 amount; u16 flags; u8 kind; } Effect;
typedef struct { u8 pad0[8]; s16 x8; s16 padA; void (*xC)(void *, s32); } VTable;
typedef struct { u8 pad0[8]; VTable *vtable; } Object;
typedef struct {
    MapPos position;
    u8 pad8[2];
    u8 room;
    u8 padB[0x11];
    u16 x1C;
    u8 x1E;
} Unit;
typedef struct { u8 index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; u8 field_0A; } SelectionSave;
extern SelectionSave D_80142F24;
extern u32 D_8013960C;
extern MapRect D_801429C0;
s32 func_800E4454(Unit *unit);
void func_800E4470(Unit *unit);
char *func_800A3B20(Unit *unit);
void func_800498E4(s32 id, ...);
void func_80049BF0(s32 arg);
s32 func_80049CB4(s32 id, ...);
s32 func_800E0F40(Unit *unit);
void func_80136910(Effect *effect, void *actor, u32 amount, u32 kind, u32 flags);
s32 func_800A9070(Iter *it, s32 room);
s32 func_800A8F6C(Iter *it);
Unit *func_800A910C(Iter *it);
void func_800A7B68(Unit *unit, Effect *effect);
u16 func_800E08B0(Unit *unit);
void *func_800A6DE4(Unit *unit);
s32 func_80119A40(void *obj, void *target, u8 room, u8 kind);
FeatureIter *func_800B07F0(FeatureIter *it);
s32 func_800B0808(FeatureIter *it);
Item *func_800B0864(FeatureIter *it);
void *func_8011422C(Item *feature);
s32 func_80121B8C(Item *feature);
Object *func_800B4D80(MapPos *pos);
s32 func_801199E0(void *obj, Object *target, u8 room, u8 kind);
void func_800AD868(MapPos *pos);
void func_80112EAC(void *obj);
/* ODD_C: individual state setters keep the whole-object accesses separate. */
static inline void save_kind(SelectionSave *state, u8 kind) {
    state->field_03 = kind;
}
static inline void save_room(SelectionSave *state, u8 room) {
    state->previous = room;
}
static inline void unit_pos(MapPos *pos, Unit *unit) {
    pos->x = unit->position.x;
    pos->y = unit->position.y;
}
static inline void range_init(MapPositionIterator *range, MapPos *corner, s32 x0, s32 y0, s32 x1, s32 y1) {
    corner->x = x0;
    corner->y = y0;
    range->start = *corner;
    range->cur = range->start;
    corner->x = x1;
    corner->y = y1;
    range->end = *corner;
}
void func_80119AE4(void *obj, void *source /* unused, supplied by the event caller */, Unit *unit) {
    MapPos pos;
    Effect effect;
    FeatureIter featureIt;
    MapPositionIterator range;
    MapPos scratch;
    Iter it;
    s32 original_room;
    u8 room;
    s32 kind;
    if (func_800E4454(unit)) {
        func_800E4470(unit);
        func_800498E4(0x226, func_800A3B20(unit));
        func_80049BF0(0);
    }
    unit_pos(&pos, unit);
    func_80049CB4(0xD9);
    func_80049CB4(0x113, &pos);
    original_room = unit->room;
    kind = func_800E0F40(unit);
    save_kind(&D_80142F24, kind);
    save_room(&D_80142F24, original_room);
    room = original_room;
    func_80136910(&effect, 0, 0, 0x15, 0);
    it.x0 = 0;
    while (func_800A9070(&it, room)) {
        Unit *other = func_800A910C(&it);
        Unit *target = other;
        s32 ok = (u8)func_800E0F40(other) == (u8)kind && !(other->x1C & 1);
        if (ok) {
            func_80049CB4(0x75, target, &pos);
            func_80049CB4(6);
            D_8013960C = (D_8013960C << 1) & ~1;
            func_800A7B68(target, &effect);
            D_8013960C >>= 1;
            func_80049CB4(7);
        }
    }
    if (!func_800E08B0(unit)) {
        func_80049CB4(0x12A);
        func_800498E4(0xC8, func_800A3B20(unit));
        if ((unit->x1E >> 2) & 1) {
            func_80049CB4(0x129, 0x1E);
        }
    }
    it.x0 = 0;
    while (func_800A8F6C(&it)) {
        void *target = func_800A6DE4(func_800A910C(&it));
        if (target != 0) {
            func_80119A40(obj, target, room, kind);
        }
    }
    func_800B07F0(&featureIt);
    while (func_800B0808(&featureIt)) {
        Item *feature = func_800B0864(&featureIt);
        if (feature->field00 != 9) {
            continue;
        }
        if (!func_80119A40(obj, func_8011422C(feature), room, kind)) {
            continue;
        }
        if (feature->field01 != 0xAC) {
            continue;
        }
        D_8013960C = (D_8013960C << 1) & ~1;
        func_80121B8C(feature);
        D_8013960C >>= 1;
    }
    range_init(&range, &scratch, D_801429C0.min.x, D_801429C0.min.y, D_801429C0.max.x, D_801429C0.max.y);
    while (1) {
        Object *target;
        s32 more = range.cur.x <= range.end.x;
        if (!more) {
            break;
        }
        func_800A3610(&scratch, &range);
        target = func_800B4D80(&scratch);
        if (target != 0 && func_801199E0(obj, target, room, kind)) {
            func_800AD868(&scratch);
            target->vtable->xC((u8 *)target + target->vtable->x8, 3);
        }
    }
    func_80112EAC(obj);
}
