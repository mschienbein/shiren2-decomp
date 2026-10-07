#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

extern u8 D_80151E38[];
void func_800D8FA8(void *object);

typedef struct {
    u8 pad0[0x4C];
    void *vtable;
} Obj8009ED70;

void func_8009ED70(Obj8009ED70 *self, s32 flags) {
    self->vtable = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(self);
    }
}
