#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad00[0x4C]; const void *vtable4C; u8 pad50[0x10]; char field60[0x14]; } Object;
extern const unsigned char D_80151E38[144];
extern void func_800951D0(char *arg0, s32 arg1);
extern void func_800D8FA8(void *object);

void func_8009A8DC(Object *self, s32 flags) {
    func_800951D0(self->field60, 2);
    self->vtable4C = D_80151E38;
    if (flags & 1) func_800D8FA8(self);
}
