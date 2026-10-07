#include "common.h"

/* Partial view of the animated record through its float at 0x198. */
typedef struct { unsigned char pad0[0x198]; float field_198; } Obj;

void func_80090C10(Obj *arg0, float arg1) {
    arg0->field_198 = arg1;
}
