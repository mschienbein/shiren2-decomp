#include "common.h"

typedef unsigned short u16;
typedef struct { char pad0[0x2E]; u16 field_2E; } Obj;

u16 func_800E3334(Obj *obj) { return obj->field_2E; }
