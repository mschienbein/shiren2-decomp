#include "common.h"

/* +0 owner: func_800C4864 dereferences it as an actor at 0x800C4890/0x800C489C. */
typedef struct {
    void *field_0;
    short field_4;
    s32 field_8;
    void *field_C;
} Obj;

extern char D_80149E10[];

Obj *func_800C4840(Obj *obj, void *a, s32 b) {
    obj->field_C = D_80149E10;
    obj->field_0 = a;
    obj->field_4 = b;
    obj->field_8 = 1;
    return obj;
}
