#include "common.h"

/* Slot 18 targets func_800E115C / derived dispatchers; its receiver adjustment is zero. */
typedef struct {
    short delta;
    short index;
    s32 (*func)(void *self, s32 a, s32 b, unsigned char c, s32 d);
} VtblEntry_800E2D94;

typedef struct {
    char pad0[0x24];
    VtblEntry_800E2D94 *vtbl;
} Obj_800E2D94;

void func_800E2D94(Obj_800E2D94 *obj) {
    VtblEntry_800E2D94 *entry = &obj->vtbl[18];

    entry->func((char *)obj + entry->delta, 1, 8, 0, 0);
}
