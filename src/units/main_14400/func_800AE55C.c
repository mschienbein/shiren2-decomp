#include "common.h"

typedef short s16;
typedef unsigned char u8;

typedef struct {
    s32 kind;
    u8 pad4[0x14];
    s32 value;
    u8 pad1C[4];
} Message800AE55C;

typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *self, Message800AE55C *message);
} VEntry800AE55C;

typedef struct {
    u8 pad0[8];
    VEntry800AE55C *vtable;
} Unit800AE55C;

/* Send message 0x19 carrying arg through the unit's message slot. */
void func_800AE55C(Unit800AE55C *unit, s32 arg) {
    Message800AE55C message;
    Message800AE55C *msg = &message;

    message.kind = 0x19;
    msg->value = arg;
    unit->vtable[7].func((u8 *)unit + unit->vtable[7].delta, msg);
}
