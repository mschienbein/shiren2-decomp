#include "common.h"

typedef struct {
    unsigned char pad0[0xC];
    unsigned char field0C;
} Object;

s32 func_80116B34(Object *self)
{
    return (self->field0C >> 2) & 1;
}
