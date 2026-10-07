#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x109];
    u8 field_109;
} Obj_800EE248;

u8 func_800EE248(Obj_800EE248 *obj) {
    return obj->field_109;
}
