#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 kind;
    u8 pad1[0xB];
    u8 field_C;
} Obj8009A330;

s32 func_8009A330(Obj8009A330 *obj)
{
    if (obj->kind != 6) {
        return 0;
    }
    return obj->field_C != 0;
}
