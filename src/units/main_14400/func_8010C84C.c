#include "common.h"
typedef struct Obj {
    unsigned char pad_00[0xE];
    unsigned char field_0E;
    unsigned char field_0F;
} Obj;
s32 func_8010C84C(Obj *self) {
    return self->field_0E - self->field_0F;
}
