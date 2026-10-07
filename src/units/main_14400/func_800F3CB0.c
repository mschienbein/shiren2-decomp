#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x88]; u8 field_88; } Obj;

u8 func_800F3CB0(Obj *obj) { return obj->field_88; }
