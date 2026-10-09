#include "common.h"

typedef struct { unsigned char pad_00[8]; const void *field_08; } Obj;
extern const unsigned char D_80153AA0[];
extern void func_800AC68C(void *a);
void func_80127D10(Obj *obj, s32 flags)
{
    obj->field_08 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
