#include "common.h"

typedef struct Actor Actor;

/* Partial class D_8015A7C0 view: +0xA0 stores an actor pointer. */
typedef struct { char pad0[0xA0]; Actor *field_A0; } Obj;

void func_800FD90C(Obj *obj, Actor *value) {
    obj->field_A0 = value;
}
