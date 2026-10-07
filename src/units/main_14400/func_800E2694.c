#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x72]; u8 field_72; } Obj;
u8 func_800E2694(Obj *o) { return o->field_72; }
