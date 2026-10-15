#include "common.h"

typedef unsigned char u8;
typedef struct Pos80127888 { s32 x, y; } Pos80127888;
typedef struct Rect80127888 { Pos80127888 tl, br; } Rect80127888;
/* 0x20-byte iterator: func_800A9204 initializes every field; func_800A9284
 * filters with them and func_800A942C advances cursor_00. Their C views remain
 * nominally distinct across TUs; this is the same field layout, not a shared type. */
typedef struct UnitIter80127888 {
    s32 cursor_00;
    struct Unit80127888 *excluded_04;
    Pos80127888 center_08;
    Rect80127888 rectangle_10;
} UnitIter80127888;
/* Slot 0x94 targets func_800E115C and the derived func_800F212C. */
typedef struct UnitMethods80127888 {
    u8 pad_00[0x90];
    short delta_90;
    short index_92;
    s32 (*method_94)(void *self, s32 mode, s32 value, u8 byte, s32 extra);
} UnitMethods80127888;
/* Partial unit view: position, status flags at 0x1E, vtable at 0x24. */
typedef struct Unit80127888 {
    Pos80127888 pos;
    u8 pad_08[0x1E - 0x08];
    u8 flags_1E;
    u8 pad_1F[0x24 - 0x1F];
    UnitMethods80127888 *vtable_24;
} Unit80127888;
typedef struct SelfMethods80127888 {
    u8 pad_00[8];
    short delta_08;
    short index_0A;
    void (*destroy_0C)(void *self, s32 flags);
} SelfMethods80127888;
/* Partial item view: vtable at 0x08, mode at 0x0C. */
typedef struct Self80127888 {
    u8 pad_00[8];
    SelfMethods80127888 *vtable_08;
    s32 mode_0C;
} Self80127888;
typedef struct Msg80127888 {
    s32 type_00;
    Unit80127888 *user_04;
    Unit80127888 *target_08;
} Msg80127888;

s32 func_80112B38(void *self, void *msg);
s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 id, ...);
Rect80127888 func_800B3080(void *pos);
void *func_800A2FD0(Rect80127888 *out, Pos80127888 *pos, u8 arg);
char *func_800A3B20(void *u);
char *func_800AE674(void *obj);
UnitIter80127888 *func_800A9204(UnitIter80127888 *o, Rect80127888 *q, void *p);
s32 func_800A9284(UnitIter80127888 *it, s32 mask);
Unit80127888 *func_800A942C(UnitIter80127888 *it);
u32 func_800B1C6C(Pos80127888 *pos);
s32 func_800A58B8(void *entity);
void func_800D3650(void *arg);

/* Member-wise position copy, as in canonical func_8011C2E0. */
static inline void Pos_copy(Pos80127888 *dst, Pos80127888 *src)
{
    dst->x = src->x;
    dst->y = src->y;
}

static inline void consume(Self80127888 *self)
{
    func_800D3650(self);
    if (self != 0) {
        self->vtable_08->destroy_0C((u8 *)self + self->vtable_08->delta_08, 3);
    }
}

/* Message handler: 0x12/0x13 hit one target, 8 sweeps the area around the user. */
s32 func_80127888(Self80127888 *self, Msg80127888 *msg)
{
    Pos80127888 pos;
    Rect80127888 area;
    Rect80127888 tmp;
    UnitIter80127888 iter;
    Unit80127888 *user;
    s32 any;

    switch (msg->type_00) {
    case 0x12:
    case 0x13: {
        Unit80127888 *target = msg->target_08;
        if (!(target->flags_1E & 0x7C)) {
            break;
        }
        target->vtable_24->method_94((u8 *)target + target->vtable_24->delta_90, 0, 0, 0xFE, 0);
        Pos_copy(&pos, &target->pos);
        func_80049CB4(0x11C, &pos);
        if (msg->type_00 == 0x12) {
            consume(self);
        }
        return 1;
    }
    case 8:
        user = msg->user_04;
        Pos_copy(&pos, &user->pos);
        if (self->mode_0C == 3) {
            tmp = func_800B3080(&pos);
            area = tmp;
            func_80049CB4(0x111E, &pos, 1);
        } else {
            func_800A2FD0(&tmp, &pos, 1);
            area = tmp;
            func_80049CB4(0x111E, &pos, 0);
        }
        any = 0;
        func_800498E4(0x11A, func_800A3B20(user), func_800AE674(self));
        func_800A9204(&iter, &area, user);
        while (func_800A9284(&iter, 0x7C)) {
            Unit80127888 *unit = func_800A942C(&iter);
            /* Kept as its own pointer, as in func_80122DD0's sweep. */
            Unit80127888 *receiver = unit;
            s32 affected = 0;
            /* Units on a protected tile (0x4000) are skipped. */
            if (!(func_800B1C6C(&unit->pos) & 0x4000)) {
                affected = func_800A58B8(unit) != 1;
            }
            if (affected) {
                receiver->vtable_24->method_94((u8 *)receiver + receiver->vtable_24->delta_90, 0, 0, 0xFE, 0);
                any = 1;
            }
        }
        if (!any) {
            func_800498E4(0x223);
        }
        consume(self);
        return 1;
    }
    return func_80112B38(self, msg);
}
