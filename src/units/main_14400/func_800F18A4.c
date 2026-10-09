#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct Unit800F18A4 Unit800F18A4;

/* Unit vtable: slot 2 = is-defeated query, slot 8 = relation to another unit. */
typedef struct {
    u8 pad00[0x10];
    s16 defeated_delta;
    s16 defeated_index;
    s32 (*defeated)(void *self);
    u8 pad18[0x40 - 0x18];
    s16 relation_delta;
    s16 relation_index;
    s32 (*relation)(void *self, void *other, u8 *priority);
} UnitVtable800F18A4;

struct Unit800F18A4 {
    u8 pad00[0x1E];
    u8 flags_1E;
    u8 pad1F[0x24 - 0x1F];
    UnitVtable800F18A4 *vtable;
};

extern s32 func_800A8F6C(s32 *p);
extern void *func_800A910C(s32 *it);
extern s32 func_800A674C(Unit800F18A4 *self, Unit800F18A4 *other);
extern s32 func_800E1CC4(Unit800F18A4 *obj, s32 kind);
extern s32 func_800A6E90(void *p);
extern s32 func_800A65B8(Unit800F18A4 *obj, void *target);
extern void *func_800A65E4(u8 *out, Unit800F18A4 *obj, void *target);

/* Pick the best hostile target: highest priority, then nearest.
 * level is caller-supplied but unused by this implementation. */
void *func_800F18A4(Unit800F18A4 *obj, u8 level, s32 any) {
    s32 it;
    u8 priority;
    u8 adjacent;
    s32 *itp;
    u8 *priorityp;
    Unit800F18A4 *best;
    s32 bestDistance;
    u8 bestPriority;
    u8 *adjacentp;
    best = 0;
    bestDistance = 0x4C;
    bestPriority = 0;
    itp = &it;
    priorityp = &priority;
    adjacentp = &adjacent;
    *priorityp = 0;
    *itp = 0;

    for (;;) {
        Unit800F18A4 *unit;
        s32 candidate;
        s32 distance;

        if (!func_800A8F6C(itp)) {
            return best;
        }
        unit = func_800A910C(itp);
        candidate = 0;
        if (obj->vtable->relation((u8 *)obj + obj->vtable->relation_delta, unit, &priority) == 2
            && (any != 0 || func_800A674C(obj, unit))
            && (!(unit->flags_1E & 0x7C) || !func_800E1CC4(unit, 1))
            && !unit->vtable->defeated((u8 *)unit + unit->vtable->defeated_delta)
            && !func_800A6E90(unit)) {
            candidate = 1;
        }
        if (!candidate) {
            continue;
        }
        distance = func_800A65B8(obj, unit);
        if (distance == 1) {
            func_800A65E4(adjacentp, obj, unit);
            if ((adjacent ^ 1) & 1) {
                distance = 0;
            }
        }
        if (priority > bestPriority) {
            best = unit;
            bestDistance = distance;
            bestPriority = priority;
        } else if (priority == bestPriority && distance < bestDistance) {
            best = unit;
            bestDistance = distance;
        }
    }
}
