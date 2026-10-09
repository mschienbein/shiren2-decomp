#include "common.h"

typedef struct {
    s32 fields_00[48];
    short adjust_C0;
    short field_C2;
    /* Monster-class slot +0xC4: func_800EF184 (D_80159150) and the derived overrides
     * func_80109A4C/func_8010AC94 take (self, unsigned char mode) and return s32. */
    s32 (*method_C4)(void *, unsigned char);
} MethodTable;

/* Overlay secondary base at +0x84 (constructed by func_801F2AC0, vtable word at +0x30);
 * +0x04 is its active flag. */
typedef struct {
    s32 field_00;
    s32 active_04;
    unsigned char fields_08[0x28];
    void *vtable_30;
} Component;

typedef struct Actor {
    unsigned char fields_00[9];
    unsigned char type_09;
    unsigned char fields_0A[0x16];
    u32 flags_20;
    MethodTable *table_24;
    unsigned char fields_28[0x30];
    struct Actor *target_58;
    unsigned char fields_5C[0x28];
    Component component_84;
} Actor;

typedef struct {
    s32 fields_00[2];
} Position;

/* One-byte direction aggregate passed by value (func_800A2594). */
typedef struct {
    unsigned char value;
} Dir;

typedef union {
    u32 word;
    struct {
        u32 upper : 9;
        u32 track_target : 1;
        u32 lower : 22;
    } bits;
} ActorFlags;

static inline s32 tracks_target(ActorFlags *flags) {
    return flags->bits.track_target;
}

extern signed char D_80148370[];
extern s32 func_801F2B48(Component *, Actor *);
extern unsigned char func_800A6420(Actor *, Actor *);
extern s32 func_800A58B8(Actor *);
extern s32 func_800B58E4(s32, s32, s32, s32);
extern s32 func_800E8694(Actor *);
extern s32 func_800E1CC4(Actor *, s32);
extern s32 func_800E8350(Actor *);
extern s32 func_800E80B8(Actor *);
extern unsigned short func_800E08B0(Actor *);
extern unsigned short func_800E08F0(Actor *);
extern Actor *func_800A492C(Actor *, s32, s32, s32);
extern s32 func_800E776C(Actor *, unsigned char);
extern s32 func_800A65B8(Actor *, Actor *);
extern void *func_800A65E4(Dir *, Actor *, Actor *);
extern void func_800A2F80(Dir *, s32);
extern void *func_800A2594(Position *, Actor *, Dir);
extern s32 func_800B56F0(Position *);
extern s32 func_800A4EFC(Actor *, Dir *);

s32 func_800EEF08(Actor *actor) {
    Position position;
    ActorFlags saved_flags;
    ActorFlags search_flags;
    Dir direction;
    s32 wait;
    unsigned char pursue;
    unsigned short health;
    unsigned short maximum_health;
    Actor *target;
    MethodTable *table;
    if (actor->component_84.active_04 == 0) {
        Component *component = actor ? &actor->component_84 : 0;
        return func_801F2B48(component, actor);
    }
    target = actor->target_58;
    if (target != 0) {
        s32 relation = func_800A6420(actor, target);
        s32 keep = 0;
        if (relation == 3) {
            actor->target_58 = 0;
        } else {
            saved_flags.word = actor->flags_20;
            if (tracks_target(&saved_flags)) {
                keep = 1;
            } else {
                s32 own_type = actor->type_09 & 15;
                s32 own_value = func_800A58B8(actor);
                s32 target_type = target->type_09 & 15;
                if (func_800B58E4(own_type, own_value, target_type, func_800A58B8(target))) {
                    keep = 1;
                }
            }
            if (!keep) {
                actor->target_58 = 0;
            }
        }
    }
    wait = 0;
    if (func_800E8694(actor)) {
        wait = func_800E1CC4(actor, 1) == 0;
    }
    if (wait) {
        return func_800E8350(actor);
    }
    if (func_800E80B8(actor)) {
        return 1;
    }
    pursue = 0;
    health = func_800E08B0(actor);
    maximum_health = func_800E08F0(actor);
    if (health < ((u32)maximum_health >> 2)) {
        Actor *candidate = actor->target_58;
        if (!candidate) {
            search_flags.word = actor->flags_20;
            candidate = func_800A492C(actor, 2, 1, tracks_target(&search_flags));
            actor->target_58 = candidate;
        }
        if (candidate) {
            if (func_800A65B8(actor, candidate) < 2) {
                Dir *direction_ptr = &direction;
                Actor *moving_actor = actor;
                s32 i;
                s32 count;
                func_800A65E4(direction_ptr, actor, candidate);
                count = ((direction.value ^ 1) & 1) ? 3 : 5;
                i = 0;
                for (;;) {
                    if (i >= count) {
                        goto pursue_target;
                    }
                    func_800A2F80(direction_ptr, D_80148370[i]);
                    func_800A2594(&position, moving_actor, direction);
                    if ((func_800B56F0(&position) ^ 1) != 0 && func_800A4EFC(actor, direction_ptr)) {
                        return 1;
                    }
                    ++i;
                }
            }
            pursue_target:
            pursue = 1;
        } else {
            return func_800E776C(actor, 3);
        }
    }
    table = actor->table_24;
    return table->method_C4((unsigned char *)actor + table->adjust_C0, pursue);
}
