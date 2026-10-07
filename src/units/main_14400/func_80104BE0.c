#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x89];
    u8 field_89;
} Obj80104BE0;

u8 func_80104BE0(Obj80104BE0 *obj) {
    return obj->field_89;
}
