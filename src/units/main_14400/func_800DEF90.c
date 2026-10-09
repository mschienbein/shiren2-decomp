#include "common.h"

typedef struct {
    short kind;
    unsigned char pad2[2];
    const void *vtable;
    unsigned char field_8;
    unsigned char field_9;
} Obj;

extern unsigned char D_80157FA8[];
extern const unsigned char D_80158A48[48];

Obj *func_800DEF90(Obj *obj, unsigned char *src, unsigned char value)
{
    obj->vtable = D_80157FA8;
    obj->kind = 2;
    obj->vtable = D_80158A48;
    obj->field_8 = *src;
    obj->field_9 = value;
    return obj;
}
