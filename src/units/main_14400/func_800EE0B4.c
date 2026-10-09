#include "common.h"
typedef struct { unsigned char pad_00[0xE4]; unsigned short flags_E4; } Obj800EE0B4;
void func_800EE0B4(Obj800EE0B4 *self) {
    self->flags_E4 &= 0xFF7F;
}
