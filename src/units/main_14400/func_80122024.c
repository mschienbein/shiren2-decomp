#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 x, y; } Position;
typedef struct { u8 value; } Dir;

/* g++ method-table entries: receiver adjustment, then the function. */
typedef struct {
    u8 pad_00[0x8];
    s16 adjust_08;
    s16 index_0A;
    void (*destroy_0C)(void *self, s32 flags);
} ObjectVt;
typedef struct {
    u8 pad_00[0x18];
    s16 adjust_18;
    s16 index_1A;
    void (*set_1C)(void *self, s32 value);
    s16 adjust_20;
    s16 index_22;
    s32 (*count_24)(void *self);
    u8 pad_28[0x38 - 0x28];
    s16 adjust_38;
    s16 index_3A;
    void *(*get_3C)(void *self, u32 index);
} ListVt;
typedef struct {
    u8 pad_00[0x38];
    s16 adjust_38;
    s16 index_3A;
    s32 (*handle_3C)(void *receiver, void *event);
} HandlerVt;

/* Contents list subobject (0x20 bytes, see func_80121940). */
typedef struct {
    void *owner_00;
    ListVt *vt_04;
    u8 pad_08[0x20 - 0x8];
} List;

/* The container object handling these messages (func_80121940's Trigger). */
typedef struct {
    u8 pad_00[0x8];
    ObjectVt *vt_08;
    List list_0C;
    u8 kind_2C;
} Container;

/* Unit addressed by a message. */
typedef struct {
    Position pos;
    HandlerVt *vt_08;
    u8 pad_0C[0x1E - 0xC];
    u8 flags_1E;
} Unit;

/* Throw/put target: map position and facing. */
typedef struct {
    Position pos;
    u8 dir_08;
} Target;

typedef struct {
    s32 kind;
    Target *target_04;
    Unit *unit_08;
    u8 pad_0C[0x1C - 0xC];
    Unit *other_1C;
} Message;

/* Kind 0x1B: "placed next to" notification sent to a unit. */
typedef struct {
    s32 kind;
    Target *target;
    void *sender;
    s32 pad_0C;
    Position pos;
    s32 dir_18;
    s32 pad_1C;
} Notice;

extern s8 D_80148770[];  /* facing offsets tried in turn: 0, -1, 2, -3, 4, -5, 6, -7 */
extern u32 D_8013960C;
s32 func_80114E28(void *self, void *event);
s32 func_800E1CD4(Unit *obj, s32 value);
s32 func_80121940(Container *trigger, Unit *obj, Position *target, s32 forced);
char *func_800AE674(void *item);
char *func_800A3B20(Unit *unit);
void func_800498E4(s32 id, ...);
void func_80049A04(u16 id, ...);
s32 func_80049CB4(s32 id, ...);
void func_80049BF0(s32 mode);
s32 func_800ADD50(Container *object, Position *result);
s32 func_800AD8AC(Container *item, Position *position);
s32 func_800A8A50(void);
void func_800A2F80(Dir *self, s32 step);
s32 func_80121B8C(Container *item);

static inline void Notice_init(Notice *notice, Target *target, void *sender, s32 dir) {
    notice->kind = 0x1B;
    notice->target = target;
    notice->sender = sender;
    notice->pos = target->pos;
    notice->dir_18 = dir;
}

static inline void Container_delete(Container *self, s32 flags) {
    if (self != 0) {
        self->vt_08->destroy_0C((char *)self + self->vt_08->adjust_08, flags);
    }
}

static inline s32 List_count(List *list) {
    return list->vt_04->count_24((char *)list + list->vt_04->adjust_20);
}

static inline s32 Container_place(Container *self, Unit *unit) {
    return func_80121940(self, unit, &unit->pos, 1) == 1;
}

static inline s32 Container_isClosedEmpty(Container *self) {
    return self->kind_2C != 0xFF && List_count(&self->list_0C) == 0;
}

s32 func_80122024(Container *self, Message *msg) {
    Position from;
    Position to;
    Dir dir;

    switch (msg->kind) {
        case 18: {
            Unit *unit = msg->unit_08;
            Unit *other;
            Position *src;
            Position *dst;
            s32 blocked = 0;

            if (!((unit->flags_1E >> 4) & 1) || func_800E1CD4(unit, 0xF) || List_count(&self->list_0C)) {
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
            Unit *unit = msg->unit_08;
            Dir *facing;
            s32 i;

            i = List_count(&self->list_0C);
            if (func_800A8A50() < i) {
                func_800498E4(0xA5, func_800AE674(unit));
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
                if (unit->vt_08->handle_3C((char *)unit + unit->vt_08->adjust_38, &notice)) {
                    return 1;
                }
            }
            func_80049A04(0xA6, func_800AE674(unit));
            return 0;
        }

        case 14: {
            List *list = &self->list_0C;
            s32 count = List_count(list);

            if (func_800A8A50() < count) {
                func_800498E4(0xA2, func_800AE674(self));
                func_800498E4(0xA5, func_800AE674(list->vt_04->get_3C((char *)list + list->vt_04->adjust_38, 0)));
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
                List *list = &self->list_0C;

                list->vt_04->set_1C((char *)list + list->vt_04->adjust_18, 0);
            }
            self->kind_2C = 0xFF;
            break;
    }
    return func_80114E28(self, msg);
}
