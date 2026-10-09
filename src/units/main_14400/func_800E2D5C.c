#include "common.h"
typedef struct {
    unsigned char pad0[0x90];
    short adjust_90;
    short pad92;
    s32 (*method_94)(void *, s32, s32, unsigned char, s32);
} Vtable;
typedef struct { unsigned char pad0[0x24]; Vtable *field_24; } Obj;
/* The action slot includes func_800F212C and takes all five arguments. */
s32 func_800E2D5C(Obj *obj) {
    return obj->field_24->method_94((unsigned char *)obj + obj->field_24->adjust_90, 2, 9, 0, 0);
}
