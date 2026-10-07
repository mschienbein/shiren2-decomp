#include "common.h"

typedef unsigned short u16;
typedef struct { unsigned char pad0[0x1C]; u16 field_1C; } Obj;

u16 func_800A8354(Obj *arg0) {
    return arg0->field_1C;
}
