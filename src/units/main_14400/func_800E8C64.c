#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef short s16;

/*
 * Item vtable slot +0x38/+0x3C: message handler. Its targets (func_80110258 for type 3,
 * func_8010D83C for type 4) return an integer status, which these calls discard.
 */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *self, void *message);
} VirtualEntry;

typedef struct {
    u8 pad0[0x38];
    VirtualEntry handle;
} VTable;

typedef struct {
    u8 pad0[0x8];
    VTable *vtable;
} Object;

typedef struct {
    s32 kind;
    u8 pad4[0x1C];
} Message;

void *func_800E8A68(void *owner, u8 slot);

s32 func_800E8C64(void *owner) {
    Message message;
    Message *msg;
    Object *first;
    Object *second;
    Object *target;

    message.kind = 0x18;
    msg = &message;
    target = func_800E8A68(owner, 3);
    first = target;
    if (target != 0) {
        (void)target->vtable->handle.func((u8 *)target + target->vtable->handle.delta, msg);
    }
    second = func_800E8A68(owner, 4);
    if (second != 0) {
        (void)second->vtable->handle.func((u8 *)second + second->vtable->handle.delta, msg);
    }
    if (first == 0 && second == 0) {
        return 0;
    }
    return 1;
}
