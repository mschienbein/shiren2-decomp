#include "map_unit_iterator.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* Actor method table entry; slot +0x44 (func_800A44E8 family) writes one priority byte. */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *self, void *target, u8 *out);
} VtableEntry;

typedef struct {
    char pad0[0x40];
    VtableEntry entry40;
} Vtable800F073C;

typedef struct {
    s32 x;
    s32 y;
} Pos800F073C;

typedef struct {
    u32 high : 7;
    u32 ignoreBlocked : 1;
    u32 low : 24;
} Flags800F073C;

static inline s32 flagsIgnoreBlocked(const Flags800F073C *flags) {
    return flags->ignoreBlocked;
}

typedef struct {
    Pos800F073C pos;
    char pad8[0x1E - 0x8];
    u8 unk1E;
} Entity800F073C;

typedef struct {
    char pad0[0x1C];
    u16 unk1C;
    char pad1E[0x20 - 0x1E];
    Flags800F073C flags20;
    Vtable800F073C *vtable;
    char pad28[0x58 - 0x28];
    Entity800F073C *target;
    char pad5C[0x64 - 0x5C];
    Pos800F073C targetPos;
    char pad6C[0x9A - 0x6C];
    u16 unk9A;
    char pad9C[0x9F - 0x9C];
    u8 targetPriority;
} Obj800F073C;

extern s32 func_800A674C(Obj800F073C *obj, Entity800F073C *entity);
extern s32 func_800E1CC4(Entity800F073C *entity, s32 kind);
extern s32 func_800A6E90(Entity800F073C *entity);
extern u32 func_800B1C6C(Pos800F073C *pos);
extern s32 func_800A650C(Obj800F073C *obj, Pos800F073C *pos);
extern void *func_800A6538(u8 *out, Obj800F073C *obj, Pos800F073C *pos);
extern u8 func_800A6420(Obj800F073C *obj, Entity800F073C *entity);
extern s32 func_800A251C(Pos800F073C *a, Pos800F073C *b);

/*
 * ODD_C: the priority out-byte is cleared through its address before the
 * loop, so its stack slot (sp+0x3C) is assigned before the direction byte
 * (sp+0x3D). A plain `priority = 0;` moves it behind direction (6 words).
 */
static inline void clear_priority(u8 *priority) {
    *priority = 0;
}

void func_800F073C(Obj800F073C *obj) {
    MapUnitIterator iter;
    Pos800F073C pos;
    Flags800F073C flags;
    u8 priority;
    u8 direction;
    s32 bestDist;
    s32 ignoreBlocked;
    u8 *directionp;
    Entity800F073C *best;
    u8 bestState;
    s32 noFallback;
    u8 bestPriority;
    Pos800F073C *posp;
    MapUnitIterator *iterp;
    Entity800F073C *entity;
    s32 dist;
    s32 skip;
    s32 diff;
    u8 state;

    best = 0;
    bestDist = 0x4C;
    bestPriority = 0;
    bestState = 3;
    /*
     * ODD_C: the fallback test is stored and then inverted in the same local
     * (the shape of C++ `!b` on a bool). The single `== 0` form gives the
     * local three references instead of five and loses s5 (106 words).
     */
    noFallback = (obj->unk1C & 0xC) != 0;
    noFallback = !noFallback;
    flags = obj->flags20;
    ignoreBlocked = flagsIgnoreBlocked(&flags);
    clear_priority(&priority);
    func_800A915C(&iter, obj);
    iterp = &iter;
    posp = &pos;
    directionp = &direction;
    while (func_800A9284(iterp, 0xFF) != 0) {
        entity = func_800A942C(iterp);
        skip = 0;
        pos.x = entity->pos.x;
        pos.y = entity->pos.y;
        if (obj->vtable->entry40.func((char *)obj + obj->vtable->entry40.delta, entity, &priority) != 2
            || (func_800A674C(obj, entity) == 0 && noFallback)
            || ((entity->unk1E & 0x7C) && func_800E1CC4(entity, 1) != 0)
            || func_800A6E90(entity) != 0
            || ((func_800B1C6C(&pos) & 0x4000) && ignoreBlocked == 0)) {
            skip = 1;
        }
        if (skip) {
            continue;
        }
        dist = func_800A650C(obj, posp);
        if (dist == 1) {
            func_800A6538(directionp, obj, posp);
            if ((direction ^ 1) & 1) {
                dist = 0;
            }
        }
        state = func_800A6420(obj, entity);
        diff = priority - bestPriority;
        if (diff < -1) {
            continue;
        }
        if (diff < 0 && state != 0) {
            continue;
        }
        if (diff < 2) {
            if (state != 0 || (diff <= 0 && bestState == 0)) {
                if (bestState == 0) {
                    continue;
                }
                if (diff <= 0 && (diff != 0 || dist >= bestDist)) {
                    continue;
                }
            }
        }
        best = entity;
        bestPriority = priority;
        bestState = state;
        bestDist = dist;
    }
    if (best != 0) {
        if (best != obj->target) {
            obj->unk9A &= ~0x20;
        }
        obj->target = best;
        obj->targetPriority = bestPriority;
    } else {
        obj->unk9A &= ~0x20;
        best = obj->target;
        if (best != 0) {
            Pos800F073C *targetPos = &obj->targetPos;

            if (func_800A251C(targetPos, &best->pos) != 0) {
                targetPos->x = 0;
                targetPos->y = 0;
            }
        }
        obj->target = 0;
        obj->targetPriority = 0;
    }
}
