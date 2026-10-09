#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct Item800F4F8C Item800F4F8C;

typedef struct {
    s32 unk0;
    void *target;
    u8 dir;
    u8 pad9[0x10 - 9];
} Side800F4F8C;

typedef struct {
    s32 kind;
    s32 fromId;
    void *fromTarget;
    u8 fromDir;
    u8 pad0D[0x18 - 0xD];
    s32 unk18;
    s32 toId;
    u8 toDir;
    u8 pad21[7];
} Msg800F4F8C;

/* Vtable entries are {this-adjust delta, index, target}. Only two slots are
 * used here, with their decided contracts:
 * slot 1 (+0x08/+0x0C) is the destructor void (void *self, s32 flags) (e.g.
 * func_80117050 in D_8015DA98 tests flags & 1 before freeing); slot 7 (+0x38/+0x3C)
 * is the message handler s32 (void *receiver, void *event) (e.g. func_80116E1C in
 * D_8015DA98 reads the message kind word; its result is not needed here).
 * Other slots are opaque here.
 */
typedef struct {
    s16 delta;
    s16 index;
    void (*func)(void *self, s32 flags);
} DtorEntry800F4F8C;

typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *receiver, void *event);
} MsgEntry800F4F8C;

typedef struct {
    u8 slot0[0x8];
    DtorEntry800F4F8C dtor;
    u8 slots2to6[0x38 - 0x10];
    MsgEntry800F4F8C message;
} VTable800F4F8C;

struct Item800F4F8C {
    u8 type;
    u8 subtype;
    u8 pad2[6];
    VTable800F4F8C *vtable;
};

typedef struct {
    s32 type;
    s32 id;
    Item800F4F8C *item;
} Event800F4F8C;

typedef struct {
    u8 pad0[8];
    u8 dir;
    u8 pad9[0x1C - 9];
    u16 flags1C;
} Unit800F4F8C;

typedef struct {
    u8 data[0x20];
} Iter800F4F8C;

s32 func_80049CB4(s32 id, ...);
char *func_800AC9D8(Item800F4F8C *item);
char *func_800A3B20(void *self);
void func_800497F0(s32 id, ...);
Iter800F4F8C *func_800A915C(Iter800F4F8C *it, void *self);
s32 func_800A9284(Iter800F4F8C *it, s32 kind);
Unit800F4F8C *func_800A942C(Iter800F4F8C *it);
void func_800D3650(Item800F4F8C *item);
s32 func_800F4A70(void *self, Event800F4F8C *ev);

static inline s32 isHidden(Unit800F4F8C *unit) {
    return unit->flags1C & 1;
}

s32 func_800F4F8C(void *self, Event800F4F8C *ev) {
    if (ev->type == 0xF) {
        Item800F4F8C *item = ev->item;
        s32 match = 0;

        if (item->type == 1 || item->type == 7 || item->subtype == 0x17) {
            match = 1;
        }
        if (match) {
            s32 a = func_80049CB4(0xB6, self);
            char *b = func_800AC9D8(item);
            Iter800F4F8C it;

            func_800497F0(0x63, a, b, func_800A3B20(self));
            func_800A915C(&it, self);
            while (func_800A9284(&it, 0x7C)) {
                Unit800F4F8C *unit = func_800A942C(&it);

                s32 active = isHidden(unit) != 1;

                if (active) {
                    Msg800F4F8C msg;
                    s32 id = ev->id;
                    u8 dir = (unit->dir + 4) & 7;

                    msg.kind = 0x13;
                    msg.fromTarget = unit;
                    msg.unk18 = 0;
                    msg.toDir = dir;
                    msg.fromId = id;
                    msg.fromDir = dir;
                    msg.toId = id;
                    item->vtable->message.func((u8 *)item + item->vtable->message.delta, &msg);
                }
            }
            func_800D3650(item);
            if (item != 0) {
                item->vtable->dtor.func((u8 *)item + item->vtable->dtor.delta, 3);
            }
            return 1;
        }
    }
    return func_800F4A70(self, ev);
}
