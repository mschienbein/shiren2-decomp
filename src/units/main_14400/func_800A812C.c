#include "common.h"
typedef struct { unsigned char pad_00[0x1C]; unsigned short flags_1C; } Obj800A812C;
void func_800A812C(Obj800A812C *self) {
    self->flags_1C &= 0x7FFF;
}
