#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 value; } Dir;
typedef struct { u32 high:16; u32 reflect:1; u32 low:15; } Flags;
typedef struct Actor { Pos pos; u8 pad_08[0x16]; u8 flags_1E, pad_1F; Flags flags_20; } Actor;
typedef struct { s32 kind; Actor *source; Actor *target; Dir direction; u8 pad_0D[0xB]; s32 field_18; Actor *owner; } Event;
typedef struct { u8 pad_00[0x38]; short delta_38, index_3A; s32 (*event_3C)(void *, Event *); } Methods;
typedef struct { u8 pad_00[8]; Methods *table_08; } Item;
typedef struct { Pos pos; Dir direction; u8 pad_09[0xF]; u8 active_18; u8 pad_19[7]; Actor *owner_20, *target_24; Item *item_28; Pos end_2C; u8 flags_34; } Context;
extern void func_800498E4(s32, ...);
extern s32 func_80049CB4(s32, ...);
extern Pos *func_800A25D8(Pos *, Pos *, Dir, s32);
extern void func_800A2758(Pos *, Dir);
extern void func_800A665C(void *, u8 *);
extern u32 func_800B1C6C(Pos *);
extern Actor *func_800B4928(Pos *);
extern s32 func_800E2074(void *);
extern void func_800E370C(void *, u8 *);
extern Actor *func_801014EC(void *, Pos *, void *);
extern s32 func_80111EB8(void *, Pos *, Dir);
/* ODD_C: inline value helpers retain original aggregate temporaries and scheduling. */
static inline void copyPosition(Pos *out, Pos *in) { out->x = in->x; out->y = in->y; }
static inline Flags actorFlags(Actor *actor) { return actor->flags_20; }
static inline s32 hasReflection(Flags *flags) { return flags->reflect; }
static inline void reflectDirection(Dir *direction) { direction->value = (direction->value + 4) & 7; }
static inline void makeEvent(Event *event, Actor *source, Actor *target, Actor *owner, Dir *direction) {
    event->kind = 0x14;
    event->target = target;
    event->source = source;
    event->direction = *direction;
    event->field_18 = 0;
    event->owner = owner;
}
static inline Dir opposite(Dir direction) { direction.value = (direction.value + 4) & 7; return direction; }
s32 func_80112084(Context *self, s32 kind, Pos *position, Dir direction) {
    Pos copy;
    switch (kind) {
    case 0:
        if (!(func_800B1C6C(position) & 0x4000)) {
            func_800A25D8(&copy, position, direction, 10);
            *position = copy;
            func_80049CB4(0xD4, &self->end_2C, position);
            return 0;
        }
        return 1;
    case 2:
    case 6:
        if (kind == 2) func_800A2758(position, opposite(direction));
        func_80049CB4(0xD4, &self->end_2C, position);
        if (kind == 2) func_800498E4(0xE7);
        return 0;
    case 1:
        func_80049CB4(0xD4, &self->end_2C, position);
        copyPosition(&copy, position);
        if (func_80111EB8(self, &copy, direction)) {
            Dir changed = self->direction;
            func_80049CB4(0xD5, position, &direction, &changed);
            return 1;
        }
        func_80049CB4(0x11A, position, direction.value);
        return 0;
    case 4: {
        Actor *target = func_800B4928(position);
        Event event;
        if (!(self->flags_34 & 2) && (self->owner_20->flags_1E & 0x7C)) {
            Actor *replacement = func_801014EC(self->owner_20, &self->end_2C, target);
            if (replacement) { *position = replacement->pos; target = replacement; }
        }
        copyPosition(&copy, position);
        func_80049CB4(0xD4, &self->end_2C, position);
        if (target) {
            Flags flags = actorFlags(target);
            if (hasReflection(&flags) && !(self->flags_34 & 2)) {
                reflectDirection(&direction);
                func_800A2758(position, direction);
                self->direction = direction;
                self->pos = *position;
                self->active_18 = 1;
                self->end_2C = *position;
                self->target_24 = target;
                self->flags_34 |= 2;
                {
                    s32 turn = 0;
                    if (target->flags_1E & 0x7C) turn = func_800E2074(target) != 0;
                    if (turn) func_800A665C(target, &direction.value);
                }
                func_80049CB4(0xD6, &copy, &direction);
                func_80049CB4(0x65, target);
                return 1;
            }
        }
        func_80049CB4(0x118, position);
        if (target->flags_1E & 0x7C) {
            Dir back = opposite(direction);
            func_800E370C(target, &back.value);
        }
        makeEvent(&event, self->target_24, target, self->owner_20, &direction);
        self->item_28->table_08->event_3C((u8 *)self->item_28 + self->item_28->table_08->delta_38, &event);
        return 0;
    }
    default:
        return 1;
    }
}
