#include "common.h"

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct { s32 x0; s32 x4; } Pos;
typedef struct { s32 x0; void *position; } Iter;
typedef struct { void *current; short index; } FeatureIter;
typedef struct { Pos current, first, last; } Range;
typedef struct { Pos first, last; } Rect;
typedef struct { void *actor; s32 field_04, field_08; s16 amount; u16 flags; u8 kind; } Effect;
typedef struct { u8 kind; u8 id; } Feature;
typedef struct { u8 pad0[8]; s16 x8; s16 padA; void (*xC)(void *, s32); } VTable;
typedef struct { u8 pad0[8]; VTable *vtable; } Object;
typedef struct {
    s32 x0;
    s32 x4;
    u8 pad8[2];
    u8 room;
    u8 padB[0x11];
    u16 x1C;
    u8 x1E;
} Unit;
typedef struct { u8 index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; u8 field_0A; } SelectionSave;
extern SelectionSave D_80142F24;
extern u32 D_8013960C;
extern Rect D_801429C0;
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
Feature *func_800B0864(FeatureIter *it);
void *func_8011422C(Feature *feature);
s32 func_80121B8C(Feature *feature);
Pos *func_800A3610(Pos *pos, Range *cur);
Object *func_800B4D80(Pos *pos);
s32 func_801199E0(void *obj, Object *target, u8 room, u8 kind);
void func_800AD868(Pos *pos);
void func_80112EAC(void *obj);
/* ODD_C: individual state setters keep the whole-object accesses separate. */
static inline void save_kind(SelectionSave *state, u8 kind) {
    state->field_03 = kind;
}
static inline void save_room(SelectionSave *state, u8 room) {
    state->previous = room;
}
static inline void unit_pos(Pos *pos, Unit *unit) {
    pos->x0 = unit->x0;
    pos->x4 = unit->x4;
}
static inline void range_init(Range *range, Pos *corner, s32 x0, s32 y0, s32 x1, s32 y1) {
    corner->x0 = x0;
    corner->x4 = y0;
    range->first = *corner;
    range->current = range->first;
    corner->x0 = x1;
    corner->x4 = y1;
    range->last = *corner;
}
void func_80119AE4(void *obj, void *source /* unused, supplied by the event caller */, Unit *unit) {
    Pos pos;
    Effect effect;
    FeatureIter featureIt;
    Range range;
    Pos scratch;
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
        Feature *feature = func_800B0864(&featureIt);
        if (feature->kind != 9) {
            continue;
        }
        if (!func_80119A40(obj, func_8011422C(feature), room, kind)) {
            continue;
        }
        if (feature->id != 0xAC) {
            continue;
        }
        D_8013960C = (D_8013960C << 1) & ~1;
        func_80121B8C(feature);
        D_8013960C >>= 1;
    }
    range_init(&range, &scratch, D_801429C0.first.x0, D_801429C0.first.x4, D_801429C0.last.x0, D_801429C0.last.x4);
    while (1) {
        Object *target;
        s32 more = range.current.x0 <= range.last.x0;
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
