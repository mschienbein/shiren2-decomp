#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x89];
    u8 field_89;
} Obj_800F9404;

u8 func_800F9404(Obj_800F9404 *obj) {
    return obj->field_89;
}
