#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 field0;
    u8 field1;
} Obj_8010E574;

s32 func_8010E574(Obj_8010E574 *obj) {
    return obj->field1 != 0xA4;
}
