#include "common.h"

typedef unsigned char u8;

typedef short s16;
typedef unsigned short u16;

typedef struct {
    s32 kind;
    void *sender;
    u8 pad8[0x18];
} Msg80100160;

/* Partial item vtable: each slot is a this-adjust delta plus a method. Slot 0x08 is the
 * destructor; slot 0x38 is the message handler, whose targets (func_80110258,
 * func_8010D83C) return int. */
typedef struct {
    u8 pad0[8];
    s16 delta08;
    s16 index0A;
    void (*destroy)(void *self, s32 flags);
    u8 pad10[0x38 - 0x10];
    s16 delta38;
    s16 index3A;
    s32 (*handle)(void *self, Msg80100160 *msg);
} ItemVtbl80100160;

typedef struct {
    s32 field0;
    s32 field4;
    ItemVtbl80100160 *vtable8;
} Item80100160;

typedef struct {
    u8 pad0[0x58];
    void *value58;
    u8 pad5C[0x3E];
    u16 flags9A;
} Obj80100160;

extern s32 func_800F10F8(Obj80100160 *obj, void *arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 func_800E0F40(Obj80100160 *obj);
extern Item80100160 *func_800AC244(u8 id);

/* Actor vtable slot 0xB4 override (D_8015B128+0xB4). Slot callers (e.g. func_800F27A4 at
 * 0x800F2B28) pass the receiver and a target in a1; this override never reads the target. */
s32 func_80100160(Obj80100160 *obj, void *slot_target) {
    u8 ids[3] = { 0x92, 0x93, 0x94 };
    Msg80100160 msg;
    Item80100160 *item;
    void *value = obj->value58;
    s32 hidden = obj->flags9A & 0x40;

    switch (func_800F10F8(obj, value, 0, hidden != 0, 1)) {
        case 1:
            return 0;
        case 2:
            return 1;
    }
    item = func_800AC244(ids[(u8)func_800E0F40(obj) - 1]);
    if (item == 0) {
        return 0;
    }
    msg.kind = 5;
    msg.sender = obj;
    item->vtable8->handle((u8 *)item + item->vtable8->delta38, &msg);
    item->vtable8->destroy((u8 *)item + item->vtable8->delta08, 3);
    return 1;
}
