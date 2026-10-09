#include "common.h"

typedef unsigned char u8;
typedef struct { u8 value; } Dir;
typedef struct { u8 pad_00[8]; Dir direction; u8 flags; } Action;
typedef struct {
    unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode;
    signed char coordinates[2], status;
} SelectionRecord;

/* Component record views at +0x78 and +0x84 of an actor. */
typedef struct { u8 pad0[4]; s32 field_4; u8 pad8[4]; s32 field_C; } Part78;
typedef struct { u8 pad0[0xC]; s32 field_C; } Part84;

typedef struct Actor {
    u8 pad0[0xA];
    u8 kind_A;
    u8 padB[0x13];
    u8 flags_1E;
    u8 pad1F[0x53];
    u8 field_72;
    u8 pad73[5];
    /* Overlapping component views: the +0x84 component begins inside the +0x78 one. */
    union {
        Part78 part_78;
        struct {
            u8 pad78[0xC];
            Part84 part_84;
        } inner;
    } parts;
    u8 pad94[0x10];
    s32 field_A4;
} Actor;

extern Actor *D_801476B8;
extern SelectionRecord D_80142F18;
s32 func_800E1CD4(Actor *actor, s32 flag);
s32 func_800A4E30(void *position, Dir *direction);
s32 func_800A44F4(void *self, void *target);
s32 func_800F6244(Actor *actor);
s32 func_800F6264(u8 *obj);

static inline Part78 *part78(Actor *actor) {
    return actor ? &actor->parts.part_78 : 0;
}

static inline Part84 *part84(Actor *actor) {
    return actor ? &actor->parts.inner.part_84 : 0;
}

static inline s32 canFaceBack(Actor *actor, Dir *direction) {
    Dir opposite;

    opposite.value = (direction->value + 4) & 7;
    return func_800A4E30(actor, &opposite);
}

static inline s32 isMarked(Actor *actor) {
    return actor->field_72 & 1;
}

s32 func_800DF40C(Action *action, Actor *self, Dir direction, Actor *target) {
    s32 reject = 0;
    s32 allowed;
    s32 relation;

    if (func_800E1CD4(self, 0x10) || !func_800A4E30(self, &direction)
        || func_800E1CD4(target, 0x10) || !canFaceBack(target, &direction)) {
        reject = 1;
    }
    if (reject) {
        return 0;
    }
    allowed = action->flags != 0;
    relation = func_800A44F4(D_801476B8, target);
    if (target == D_801476B8) {
        return 1;
    }
    if ((target->flags_1E >> 3) & 1 && relation == 0) {
        if (part84(target)->field_C != 0) {
            return allowed;
        }
    } else if ((target->flags_1E >> 6) & 1) {
        s32 result;

        if (part78(target)->field_C == 0) {
            return 0;
        }
        result = 0;
        if (allowed || part78(target)->field_4 != 0) {
            result = 1;
        }
        return result;
    } else if (relation == 1) {
        return 1;
    }
    if (target->kind_A == 0x57) {
        if (relation == 2) {
            return 0;
        }
        if (func_800E1CD4(target, 0xF)) {
            return allowed;
        }
        if (func_800F6244(target)) {
            return 0;
        }
        return func_800F6264((u8 *)target);
    }
    if (target->kind_A == 0x5A && !((D_80142F18.flags >> 2) & 1) && allowed) {
        s32 result = 0;

        if (target->field_A4 == 0) {
            result = isMarked(target) == 0;
        }
        return result;
    }
    return 0;
}
