#include "common.h"

typedef struct {
    unsigned char field_0;
    unsigned char field_1;
} Obj;

s32 func_800DEF50(Obj *obj, unsigned char *out)
{
    *out = obj->field_1;
    return 1;
}
