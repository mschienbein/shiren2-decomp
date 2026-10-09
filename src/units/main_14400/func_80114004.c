#include "common.h"

typedef struct {
    unsigned char pad0[0xD];
    unsigned char field0D;
} Object;

void func_80114004(Object *self, s32 mask)
{
    self->field0D &= ~mask;
}
