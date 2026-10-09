#include "common.h"
typedef struct { unsigned char pad_00[0xC]; unsigned char flags_0C; } Obj80128B34;
void func_80128B34(Obj80128B34 *self) {
    self->flags_0C &= 0xFB;
}
