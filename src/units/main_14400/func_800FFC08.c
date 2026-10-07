#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x89]; u8 field_89; } Obj;
u8 func_800FFC08(Obj *o) { return o->field_89; }
