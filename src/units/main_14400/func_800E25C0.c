#include "common.h"
typedef struct { unsigned char pad0[0x72]; unsigned char field_72; } Obj;
void func_800E25C0(Obj *obj) { obj->field_72 |= 0x10; }
