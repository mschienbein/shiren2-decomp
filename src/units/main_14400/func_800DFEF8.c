#include "common.h"

typedef struct { unsigned short field_00; const void *field_04; } Obj;
extern const unsigned char D_80157FA8[];
extern const unsigned char D_80158BC8[];

/* The parser factory supplies its payload pointer; this action has no payload. */
Obj *func_800DFEF8(Obj *obj, unsigned char *unused_data)
{
    obj->field_04 = D_80157FA8;
    obj->field_00 = 0x35;
    obj->field_04 = D_80158BC8;
    return obj;
}
