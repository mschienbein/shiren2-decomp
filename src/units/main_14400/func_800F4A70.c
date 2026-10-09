#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
/* Entity vtable view: slot +0x14 is the s32 state query, slot +0x64 the void update. */
typedef struct {
    u8 pad_00[0x10];
    short delta_10, index_12;
    s32 (*query_14)(void *self);
    u8 pad_18[0x48];
    short delta_60, index_62;
    void (*update_64)(void *self);
} ObjectVTable;
typedef struct Object {
    Position position;
    u8 reserved_08[2];
    u8 id;
    u8 reserved_0B[0x19];
    ObjectVTable *vtable;
    u8 field_28, remaining_29;
} Object;
typedef struct { Object *source; s32 kind, field_08; short amount; unsigned short flags; } Damage;
typedef union { Damage *damage; s32 flags; } EventValue;
typedef struct { s32 kind; u8 reserved_04[0xC]; EventValue value; } Event;
extern void func_800A7B68(Object *, Damage *), func_800A59A4(Object *), func_800A578C(Object *), func_800C9588(void);
extern s32 func_800A6FD0(Object *), func_80049CB4(s32, ...), func_800A8FC8(s32 *, s32);
extern Object *func_800A910C(s32 *);
extern void func_800497F0(s32, ...);
extern char *func_800A3B20(Object *);
static inline Position *copyPosition(Position *dst, Position *src) {
    dst->x = src->x;
    dst->y = src->y;
    return dst;
}
s32 func_800F4A70(Object *self, Event *event) {
    switch (event->kind) {
    case 1:
        self->vtable->update_64((u8 *)self + self->vtable->delta_60);
        return 1;
    case 8: case 9: {
        Damage *damage = event->value.damage;
        /* FAKEMATCH: `fatal` plus the dead `damage = 0;` below exist only for codegen.
         * Ending damage's live range after the kill call keeps CSE on damage, so the
         * fatal copy gets a1 and fills the first beq delay slot, as in the ROM. Without
         * them the best form is 103 words off. Tried without them: early-return kill
         * calls, a single fatal flag, a combined || condition, and an inline kill helper. */
        Damage *fatal = damage;
        if (damage->kind != 0x22) {
            if (damage->amount < 0) return 1;
            if (--self->remaining_29) {
                s32 sound = func_80049CB4(0xB1, self);
                if (damage->source && func_800A6FD0(damage->source) && damage->kind == 1)
                    func_800497F0(0x116, sound, func_800A3B20(self));
                return 1;
            }
        }
        func_800A7B68(self, fatal);
        damage = 0; /* FAKEMATCH: dead store, see above. */
        return 1;
    }
    case 10: {
        s32 sound, message;
        self->remaining_29 = 0;
        func_800A59A4(self);
        func_80049CB4(0x1131);
        func_80049CB4(6);
        if (event->value.damage->kind == 0x22) {
            Position position;
            sound = func_80049CB4(0x116, copyPosition(&position, &self->position));
            message = 0x106;
        } else {
            sound = func_80049CB4(0xB2, self);
            message = 0x112;
        }
        func_80049CB4(7);
        func_80049CB4(6);
        func_80049CB4(0x70, self);
        func_80049CB4(7);
        func_800497F0(message, sound, func_800A3B20(self));
        if (self->id == 3) {
            s32 count = 0, iterator = 0;
            while (func_800A8FC8(&iterator, 2)) {
                Object *other = func_800A910C(&iterator);
                s32 alive = 0;
                if (other->id == 3)
                    alive = other->vtable->query_14((u8 *)other + other->vtable->delta_10) == 0;
                if (alive) count++;
            }
            if (count == 0) func_800C9588();
        }
        return 1;
    }
    case 20:
        if (event->value.flags & 2) func_800A578C(self);
        return 1;
    default:
        return 0;
    }
}
