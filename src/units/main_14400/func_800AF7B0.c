#include "common.h"
typedef struct { s32 field_0, field_4; void *field_8; } Object;
/* Base item method table (0x40 bytes of rodata at 0x80153AA0). */
extern char D_80153AA0[];
extern void func_800AC68C(Object *);
void func_800AF7B0(Object *object, s32 flags) {
    object->field_8 = D_80153AA0;
    if (flags & 1) func_800AC68C(object);
}
