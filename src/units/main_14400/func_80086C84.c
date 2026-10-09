#include "common.h"

typedef unsigned char u8;
typedef signed short s16;
typedef struct { u8 pad00[4]; s16 field04; u8 pad06[0x1A]; void *field20; } Object;
extern void func_800427F0(void *arg0);

void func_80086C84(Object *self) {
    if (self->field20) func_800427F0(self->field20);
    self->field04 = 4;
}
