#include "common.h"

typedef struct Obj Obj;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Obj *func_80120910(Obj *obj);

Obj *func_80120948(void)
{
    return func_80120910(func_800AC5B4(0x2C, 0));
}
