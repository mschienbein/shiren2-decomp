#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 raw; } ActorFlags;
typedef union { u16 raw; struct { u8 high, low; } bytes; } StateFlags;
typedef struct Object { u8 pad00[0xA]; u8 kind0A; u8 pad0B[0x13]; ActorFlags flags1E; u8 type1F; u8 pad20[0x55]; u8 group75; u8 pad76[0x24]; StateFlags state9A; u8 pad9C[0x68]; struct Object *target104; } Object;
typedef Object Obj_80049414;
typedef Object S;
extern Object *D_801476B8;
extern s32 func_800E1CC4(Obj_80049414 *obj, s32 kind);
extern s32 func_800F069C(void *self);
extern s32 func_800E1CD4(S *self, s32 kind);
extern s32 func_800E20CC(void *self);

static __inline__ s32 is_actor(u8 flags) { return (flags >> 4) & 1; }
static __inline__ s32 is_ally(u8 flags) { return (flags >> 2) & 1; }
static __inline__ s32 special_state(Object *self) {
    s32 result = 0;
    if (self->state9A.raw & 0x20) result = 1;
    return result;
}

s32 func_800F1E70(Object *self, Object *other, u8 *result) {
    s32 blocked = 0;
    if (other == 0 || (self->state9A.bytes.high & 1) ||
        ((other->flags1E.raw & 0x7C) && func_800E1CC4(other, 1))) blocked = 1;
    if (blocked) { *result = 0; return 0; }
    if (func_800F069C(self)) {
        if (other->flags1E.raw & 0x7C) {
            s32 invalid = 0;
            if (func_800E1CD4(other, 15) ||
                (is_actor(other->flags1E.raw) && (other->state9A.bytes.high & 1)) ||
                (!(self->state9A.raw & 0x20) && is_actor(other->flags1E.raw) &&
                 !func_800F069C(other) && other->type1F == self->kind0A && other->group75 == self->group75)) invalid = 1;
            if (invalid) { *result = 0; return 0; }
        }
        switch (other->kind0A) {
        case 4: *result = 7; return 2;
        case 0x3E: *result = 10; return 2;
        case 0x5B: case 0x5C: *result = 8; return 2;
        }
        if (is_ally(other->flags1E.raw)) { *result = 5; return 2; }
        {
            s32 special = (other->flags1E.raw & 0xC) && special_state(self);
            if (special) { *result = 4; return 2; }
        }
        if (is_actor(other->flags1E.raw)) {
            if (other->state9A.raw & 0x40) { *result = 4; return 2; }
            if (func_800E20CC(other)) { *result = 3; return 2; }
            *result = 0; return 1;
        }
        *result = 0; return 0;
    } else {
        s32 invalid = (other->flags1E.raw & 0x7C) && func_800E1CD4(other, 15);
        if (invalid) { *result = 0; return 0; }
        {
            s32 friendly = 0;
            if ((other->flags1E.raw & 0xC) ||
                (is_actor(other->flags1E.raw) && (other->state9A.raw & 0x40)) ||
                other == D_801476B8->target104) friendly = 1;
            if (friendly) { *result = 0; return 1; }
        }
        {
            s32 classified = 0;
            if (is_actor(other->flags1E.raw) || other->kind0A == 0x5D) classified = 1;
            if (classified) {
                if (other->kind0A == 0x3E) *result = 10;
                else *result = 2;
                return 2;
            }
        }
        *result = 0; return 0;
    }
}
