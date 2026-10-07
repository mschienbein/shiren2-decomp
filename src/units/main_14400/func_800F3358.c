#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef short s16;

typedef struct {
    s16 delta;
    s16 index;
    u32 (*func)(void *self); /* slot 13 (+0x68/+0x6C): u32 func_800E0E88(Obj *) */
} VirtualEntry;

typedef struct {
    u8 pad0[0x68];
    VirtualEntry getValue;
} VTable;

typedef struct {
    u8 pad0[0x24];
    VTable *vtable;
} Actor;

typedef struct Message {
    void *field_0;
    s32 field_4;
    s32 field_8;
    s16 field_C;
    u16 field_E;
    u8 field_10;
} Message;

s32 func_800E33D0(Actor *actor, Message message, u8 rays, u8 range);

s32 func_800F3358(Actor *actor) {
    Message message;
    Message copy;
    Message *init;
    s16 value;

    value = (s16)actor->vtable->getValue.func((u8 *)actor + actor->vtable->getValue.delta);
    init = &message;
    init->field_C = value;
    init->field_10 = 10;
    init->field_E = 0;
    init->field_8 = 0;
    copy = message;
    func_800E33D0(actor, copy, 1, 1);
    return 1;
}
