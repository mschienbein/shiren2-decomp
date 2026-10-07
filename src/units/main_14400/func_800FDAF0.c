#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x8A]; u8 field_8A; } Obj;

u8 func_800FDAF0(Obj *obj) { return obj->field_8A; }
