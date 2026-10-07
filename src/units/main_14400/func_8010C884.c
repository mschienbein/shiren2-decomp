#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xC];
    u8 field_C;
} Obj8010C884;

void func_8010C884(Obj8010C884 *obj, u8 value) {
    obj->field_C = value;
}
