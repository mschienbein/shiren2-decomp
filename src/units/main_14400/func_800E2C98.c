#include "common.h"

typedef short s16;

/* Slot 18 targets func_800E115C / derived dispatchers; its receiver adjustment is zero. */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *self, s32 a, s32 b, unsigned char c, s32 d);
} VEntry800E2C98;

typedef struct {
    char pad0[0x24];
    VEntry800E2C98 *vtable;
} Obj800E2C98;

s32 func_800E2C98(Obj800E2C98 *obj) {
    VEntry800E2C98 *entry = &obj->vtable[18];
    return entry->func((char *)obj + entry->delta, 0, 0xA, 0xFE, 0);
}
