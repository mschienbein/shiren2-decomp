#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 value; } Dir;
typedef struct {
    ItemMessage base;
    void *arg4;
    void *arg8;
    Dir dir;
    Pos pos;
    s32 unk18;
    s32 unk1C;
} Event;
typedef struct { char pad[0x1E]; u8 flags1E; } Thing;
typedef struct {
    char pad0[0x20];
    Thing *thing20;
    Item *actor24;
    char pad28[0x90];
    s32 unkB8;
} Self;
u32 func_800B1C6C(void *pos);
void func_800A2758(Pos *, Dir);
Item *func_800B4D80(Pos *);
s32 func_80049CB4(s32 id, ...);
char *func_800AC9D8(Item *obj);
s32 func_800ADD50(Item *, Pos *);
s32 func_800AD8AC(Item *, Pos *);
s32 func_800B56F0(Pos *);
void func_800D3650(Item *);
void func_800498E4(s32 message_id, ...);
static inline void Pos_set(Pos *dst, Pos *src) {
    dst->x = src->x;
    dst->y = src->y;
}
static inline void Event_init(Event *event, s32 type, void *arg, Pos *pos) {
    event->base.kind = type;
    event->arg4 = arg;
    event->pos = *pos;
}
static inline void Event_init_move(Event *event, s32 type, void *arg4, void *arg8, Pos *pos, Dir *dir) {
    event->base.kind = type;
    event->arg4 = arg4;
    event->arg8 = arg8;
    event->pos = *pos;
    event->dir = *dir;
}
static inline s32 has_any(s32 flags, s32 mask) {
    return (flags & mask) != 0;
}
static inline s32 is_special(u8 kind) {
    s32 special = 0;
    if (kind == 0x98) {
        special = 1;
    } else if (kind == 0x99) {
        special = 1;
    }
    return special;
}
static inline Dir Dir_turn(Dir *dir, s32 steps) {
    Dir turned;
    turned.value = (dir->value + steps) & 7;
    return turned;
}
void func_800C33FC(Self *self, Pos *target, Dir dir) {
    Pos pos;
    Event event;
    Item *actor;
    Pos *where;
    s32 ok;
    s32 moved;
    s32 blocked;
    char *name;
    u16 sound;
    Pos_set(&pos, target);
    ok = self->actor24->field08->test1C((char *)self->actor24 + self->actor24->field08->delta18, 0x1D);
    if (ok) {
        if (self->unkB8) {
            ok = has_any(func_800B1C6C(target), 0x6100);
        } else if (self->actor24->field01 == 0xCA) {
            ok = 1;
        } else if (func_800B1C6C(target) & 0x2000) {
            ok = 0;
        }
    }
    if (func_800B1C6C(target) & 0x4000) {
        func_800A2758(&pos, Dir_turn(&dir, 4));
    }
    if (ok) {
        Event_init(&event, 0xE, self->thing20, &pos);
        if (self->actor24->field08->handle3C((char *)self->actor24 + self->actor24->field08->delta38, &event.base)) {
            return;
        }
    }
    actor = func_800B4D80(&pos);
    if (actor != 0 && actor->field00 == 0x10) {
        func_80049CB4(0x10C0, self->actor24, target, &pos);
        Event_init_move(&event, 0x17, 0, self->actor24, &pos, &dir);
        actor->field08->handle3C((char *)actor + actor->field08->delta38, &event.base);
        return;
    }
    moved = 0;
    blocked = moved;
    name = func_800AC9D8(self->actor24);
    where = &pos;
    if (!(func_800B1C6C(where) & 0x100) && func_800ADD50(self->actor24, where)) {
        func_80049CB4(0x10C0, self->actor24, target, where);
        moved = func_800AD8AC(self->actor24, where);
        if (!moved) {
            blocked = func_800B56F0(where);
        }
    } else {
        func_80049CB4(0x10C0, self->actor24, target, &pos);
        func_800D3650(self->actor24);
        if (self->actor24 != 0) {
            self->actor24->field08->method0C((char *)self->actor24 + self->actor24->field08->delta08, 3);
        }
    }
    sound = 0;
    if (moved) {
        Thing *thing = self->thing20;
        if (thing != 0 && ((thing->flags1E >> 2) & 1)) {
            Item *mover = self->actor24;
            if (mover->field03 == 2) {
                sound = 0x69;
                if (self->unkB8 == 0) {
                    sound = is_special(mover->field01) ? 0x66 : 0x65;
                }
            } else {
                sound = is_special(mover->field01) ? 0x68 : 0x67;
            }
        }
    } else {
        Thing *thing = self->thing20;
        if (thing != 0 && (thing->flags1E & 0xC) && blocked == 0) {
            sound = 0x87;
        }
    }
    if (sound) {
        func_800498E4(sound, name);
    }
}
