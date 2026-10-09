#include "common.h"
typedef struct { unsigned char pad_0[0xD]; unsigned char field_D; } Obj;
void func_80113FB8(Obj *obj) { obj->field_D &= ~4; }
