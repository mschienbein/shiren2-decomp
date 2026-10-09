#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x89];
    u8 field_89;
} Obj;

u8 func_80109170(Obj *obj) {
    return obj->field_89;
}
