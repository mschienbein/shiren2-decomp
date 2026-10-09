#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    u8 pad0[0x4];
    void *vtbl;
} Obj800DA320;

extern u8 D_80157FA8[];
extern void func_800D8FE8(void *object);

/* Destructor in slot 1 of D_80158248 (GCC 2.x in-charge flags). */
void func_800DA320(Obj800DA320 *self, s32 flags) {
    self->vtbl = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(self);
    }
}
