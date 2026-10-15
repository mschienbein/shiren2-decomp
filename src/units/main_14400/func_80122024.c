#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 x, y; } Position;
typedef struct { u8 value; } Dir;


/* Unit addressed by a message (kind 18): map position, then the state flags at +0x1E. */
typedef struct {
    Position pos;
    u8 pad_08[0x1E - 0x8];
    u8 flags_1E;
} Unit;


/* Throw/put target: map position and facing. */
typedef struct {
    Position pos;
    u8 dir_08;
} Target;

/* +0x08 is the message's object: an Item for kind 12, a Unit for kind 18. */
typedef struct {
    s32 kind;
    Target *target_04;
    void *object_08;
    u8 pad_0C[0x1C - 0xC];
    Unit *other_1C;
} Message;

/* Kind 0x1B: "placed next to" notification sent to the kind-12 message's item. */
typedef struct {
    ItemMessage base;
    Target *target;
    void *sender;
    s32 pad_0C;
    Position pos;
    s32 dir_18;
    s32 pad_1C;
} Notice;

extern s8 D_80148770[];  /* facing offsets tried in turn: 0, -1, 2, -3, 4, -5, 6, -7 */
extern u32 D_8013960C;
s32 func_80114E28(Item *self, void *event);
s32 func_800E1CD4(Unit *obj, s32 value);
s32 func_80121940(Item *trigger, Unit *obj, Position *target, s32 forced);
char *func_800A3B20(Unit *unit);
void func_800498E4(s32 id, ...);
void func_80049A04(u16 id, ...);
s32 func_80049CB4(s32 id, ...);
void func_80049BF0(s32 mode);
s32 func_800ADD50(Item *object, Position *result);
s32 func_800AD8AC(Item *item, Position *position);
s32 func_800A8A50(void);
void func_800A2F80(Dir *self, s32 step);
s32 func_80121B8C(Item *item);

static inline void Notice_init(Notice *notice, Target *target, void *sender, s32 dir) {
    notice->base.kind = 0x1B;
    notice->target = target;
    notice->sender = sender;
    notice->pos = target->pos;
    notice->dir_18 = dir;
}

static inline void Container_delete(Item *self, s32 flags) {
    if (self != 0) {
        self->field08->method0C((char *)self + self->field08->delta08, flags);
    }
}

static inline s32 List_count(ItemList *list) {
    return list->methods->count24((char *)list + list->methods->delta20);
}

static inline s32 Container_place(Item *self, Unit *unit) {
    return func_80121940(self, unit, &unit->pos, 1) == 1;
}

static inline s32 Container_isClosedEmpty(Item *self) {
    return self->payload.pot.field2C != 0xFF && List_count(&self->payload.pot.list) == 0;
}

s32 func_80122024(Item *self, Message *msg) {
    Position from;
    Position to;
    Dir dir;

    switch (msg->kind) {
        case 18: {
            Unit *unit = msg->object_08;
            Unit *other;
            Position *src;
            Position *dst;
            s32 blocked = 0;

            if (!((unit->flags_1E >> 4) & 1) || func_800E1CD4(unit, 0xF) || List_count(&self->payload.pot.list)) {
                blocked = 1;
            }
            if (blocked) {
                break;
            }
            other = msg->other_1C;
            if (!Container_place(self, unit) && other != 0 && ((other->flags_1E >> 2) & 1)) {
                func_800498E4(0x64, func_800AE674(self), func_800A3B20(unit));
                func_800498E4(0xA4);
            }
            dst = &to;
            from.x = unit->pos.x;
            src = &from;
            src->y = unit->pos.y;
            to.x = src->x;
            to.y = src->y;
            if (func_800ADD50(self, dst)) {
                func_80049CB4(0x10C0, self, src, dst);
                func_800AD8AC(self, dst);
            } else if (other != 0 && ((other->flags_1E >> 2) & 1)) {
                func_800498E4(0xA8, func_800A3B20(unit));
                Container_delete(self, 3);
            }
            return 1;
        }

        case 12: {
            Target *target = msg->target_04;
            Item *item = msg->object_08;
            Dir *facing;
            s32 i;

            i = List_count(&self->payload.pot.list);
            if (func_800A8A50() < i) {
                func_800498E4(0xA5, func_800AE674(item));
            }
            i = 0;
            facing = &dir;
            dir.value = target->dir_08;
            for (;; i++) {
                Notice notice;

                if (i >= 8) {
                    break;
                }
                func_800A2F80(facing, D_80148770[i]);
                Notice_init(&notice, target, self, dir.value);
                if (item->field08->handle3C((char *)item + item->field08->delta38, &notice.base)) {
                    return 1;
                }
            }
            func_80049A04(0xA6, func_800AE674(item));
            return 0;
        }

        case 14: {
            ItemList *list = &self->payload.pot.list;
            s32 count = List_count(list);

            if (func_800A8A50() < count) {
                func_800498E4(0xA2, func_800AE674(self));
                func_800498E4(0xA5, func_800AE674(list->methods->get3C((char *)list + list->methods->delta38, 0)));
                func_80049BF0(1);
                return 0;
            }
            break;
        }

        case 30:
            if (Container_isClosedEmpty(self)) {
                D_8013960C <<= 1;
                func_80121B8C(self);
                D_8013960C >>= 1;
            }
            return 1;

        case 28:
            if (Container_isClosedEmpty(self)) {
                ItemList *list = &self->payload.pot.list;

                list->methods->set1C((char *)list + list->methods->delta18, 0);
            }
            self->payload.pot.field2C = 0xFF;
            break;
    }
    return func_80114E28(self, msg);
}
