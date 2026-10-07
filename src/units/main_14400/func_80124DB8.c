#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

extern u8 D_80153AA0[];
void func_800AC68C(void *ptr);

typedef struct {
    u8 pad0[8];
    void *vtable;
} Obj80124DB8;

void func_80124DB8(Obj80124DB8 *self, s32 flags) {
    self->vtable = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(self);
    }
}
