#include "common.h"

typedef struct { unsigned char pad0[0x58]; s32 field_58; } Obj;

void func_800975A0(Obj *arg0, s32 arg1) {
    arg0->field_58 = arg1;
}
