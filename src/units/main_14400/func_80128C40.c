#include "common.h"
typedef struct { s32 field_00[2]; const void *field_08; } Object;
extern const unsigned char D_80160770[72];
extern Object *func_80117230(Object *, s32);
Object *func_80128C40(Object *self) {
    func_80117230(self, 0xf2);
    self->field_08 = D_80160770;
    return self;
}
