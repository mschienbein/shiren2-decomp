#include "common.h"
typedef struct { unsigned char pad0[0xC]; unsigned char field_C; } Obj;
void func_80116B14(Obj *obj) { obj->field_C &= 0xFB; }
