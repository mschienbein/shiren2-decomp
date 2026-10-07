#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad[0x94]; u8 flags94; } Obj;
s32 func_800EE218(Obj *obj) {
    return (obj->flags94 >> 3) & 1;
}
