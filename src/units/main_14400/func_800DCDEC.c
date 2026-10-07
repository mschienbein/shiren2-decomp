#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

extern u8 D_801586E8[];
void *func_800DA904(void *obj, s32 kind, u8 *params);

typedef struct {
    u8 pad0[4];
    void *vtable;
} Obj800DCDEC;

Obj800DCDEC *func_800DCDEC(Obj800DCDEC *self, u8 *arg) {
    func_800DA904(self, 0x1D, arg);
    self->vtable = D_801586E8;
    return self;
}
