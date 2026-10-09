#include "common.h"

typedef unsigned short u16;

typedef struct {
    u16 kind;
    const void *vtable;
} Obj800DFED0;

extern const unsigned char D_80157FA8[];
extern const unsigned char D_80158BC8[];

Obj800DFED0 *func_800DFED0(Obj800DFED0 *obj)
{
    obj->vtable = D_80157FA8;
    obj->kind = 0x35;
    obj->vtable = D_80158BC8;
    return obj;
}
