#include "common.h"

typedef struct Obj800A3D0C Obj800A3D0C;

typedef struct {
    char pad0[0x74];
    Obj800A3D0C *field_74;
} Obj80097054;

extern char *func_800A3D0C(Obj800A3D0C *p);

char *func_80097054(Obj80097054 *self) {
    return func_800A3D0C(self->field_74);
}
