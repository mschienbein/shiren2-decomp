#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

/* Decided four-pointer direction callback (D_8015B368 +0xC func_80100A10, +0x14 func_80100AC4):
 * receiver, actor, direction-byte address, position. */
typedef void (*DirectionHandler)(void *self, void *actor, u8 *direction, void *position);

/* g++ 2.8.1 vtable entry: receiver delta, index, function. */
typedef struct {
    s16 delta;
    s16 index;
    DirectionHandler func;
} VtblEntry;

/* Wrapped handler: data members first, vtable pointer at +0x0C. */
typedef struct {
    u8 pad_00[0xC];
    VtblEntry *vtbl;
} Handler;

typedef struct {
    u8 pad_00[0x10];
    Handler *target;
} Wrapper;

/* +0x14 slot: replace only the receiver and forward actor/direction/position (a1..a3) to the
 * wrapped object's +0x14 handler. */
void func_800C4E50(Wrapper *self, void *actor, u8 *direction, void *position) {
    Handler *target = self->target;

    target->vtbl[2].func((u8 *)target + target->vtbl[2].delta, actor, direction, position);
}
