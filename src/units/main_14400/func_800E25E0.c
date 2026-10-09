#include "common.h"

typedef struct {
    unsigned char pad0[0x72];
    unsigned char field72;
} Object;

void func_800E25E0(Object *self)
{
    self->field72 &= ~8;
}
