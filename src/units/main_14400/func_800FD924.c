#include "common.h"

typedef unsigned char u8;
typedef struct { char pad0[0x89]; u8 field_89; } Obj;

u8 func_800FD924(Obj *o) {
    return o->field_89;
}
