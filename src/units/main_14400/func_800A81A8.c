#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad[0x1D]; u8 flags1D; } Obj;
u32 func_800A81A8(Obj *obj) {
    return obj->flags1D >> 7;
}
