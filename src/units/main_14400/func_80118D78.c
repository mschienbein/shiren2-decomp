#include "common.h"

typedef struct VTable VTable;
typedef struct { unsigned char pad00[8]; VTable *vtable08; } Obj;
extern VTable D_80153AA0;
extern void func_800AC68C(void *a);

void func_80118D78(Obj *obj, s32 flags)
{
    obj->vtable08 = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
