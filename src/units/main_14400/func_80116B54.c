#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0xC]; u8 flags0C; } Object;

void func_80116B54(Object *self) {
    self->flags0C |= 2;
}
