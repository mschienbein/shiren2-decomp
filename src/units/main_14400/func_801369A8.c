#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;
typedef struct VTable VTable;

typedef struct {
    u8 pad0[0xC];
    VTable *vtbl;
} Obj801369A8;

/* Opaque mixed adjustment/function-pointer table; only its address is used here. */
extern VTable D_80151350;
extern void func_800D8FA8(void *object);

/* Destructor in slot 7 of D_80149F30 (GCC 2.x in-charge flags). */
void func_801369A8(Obj801369A8 *self, s32 flags) {
    self->vtbl = &D_80151350;
    if (flags & 1) {
        func_800D8FA8(self);
    }
}
