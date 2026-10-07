#include "common.h"

/* Partial view of the animated record through its float at 0x180. */
typedef struct { unsigned char pad0[0x180]; float field_180; } Obj;

void func_80090BE0(Obj *arg0, float arg1) {
    arg0->field_180 = arg1;
}
