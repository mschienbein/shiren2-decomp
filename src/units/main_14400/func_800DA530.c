#include "common.h"

/* One-byte value passed by value (left-justified in the argument register). */
typedef struct {
    signed char value;
} Small;

typedef struct {
    short kind;
    unsigned char pad2[2];
    void *vtable;
    Small field_8;
} Obj;

extern unsigned char D_80157FA8[];
extern unsigned char D_80158308[];

Obj *func_800DA530(Obj *obj, Small value)
{
    obj->vtable = D_80157FA8;
    obj->kind = 3;
    obj->vtable = D_80158308;
    obj->field_8 = value;
    return obj;
}
