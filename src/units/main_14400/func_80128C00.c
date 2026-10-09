#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0xC]; u8 flags0C; } Object;

void func_80128C00(Object *self, s32 flags) {
    self->flags0C &= ~flags;
}
