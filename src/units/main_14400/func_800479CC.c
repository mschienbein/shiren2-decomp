#include "common.h"

typedef struct { unsigned char pad0[0x2CC]; s32 field_2CC; } Obj;

void func_800479CC(Obj *arg0, s32 arg1) {
    arg0->field_2CC = arg1;
}
