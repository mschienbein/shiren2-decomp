#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    s16 delta;
    s16 index;
    void (*func)(void *self);
} VEntry800CED08;

typedef struct {
    u8 pad0[0x60];
    VEntry800CED08 refresh;
} VTable800CED08;

typedef struct {
    u8 pad0[0x1E];
    u8 flags1E;
    u8 pad1F[0x24 - 0x1F];
    VTable800CED08 *vtbl;
} Owner800CED08;

typedef struct {
    u8 pad0[0x10];
    Owner800CED08 *owner;
} Obj800CED08;

extern void func_800EC0F4(Owner800CED08 *obj, void *item);
extern void func_800CE718(Obj800CED08 *s, void *obj);

/* Vtable slot 6 of D_80154438. */
void func_800CED08(Obj800CED08 *self, void *item) {
    Owner800CED08 *owner;

    if ((self->owner->flags1E >> 2) & 1) {
        func_800EC0F4(self->owner, item);
    }
    func_800CE718(self, item);
    owner = self->owner;
    if (owner->flags1E & 0xC) {
        owner->vtbl->refresh.func((u8 *)owner + owner->vtbl->refresh.delta);
    }
}
