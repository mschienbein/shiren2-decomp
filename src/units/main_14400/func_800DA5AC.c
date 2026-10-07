#include "common.h"

typedef struct {
    unsigned char value;
} Small;

typedef struct {
    short kind;
    unsigned char pad2[2];
    void *vtable;
    Small field_8;
} Obj;

extern unsigned char D_80157FA8[];
extern unsigned char D_80158308[];

static inline void copy_small(Small *dst, Small *src)
{
    *dst = *src;
}

Obj *func_800DA5AC(Obj *obj, unsigned char *src)
{
    Small tmp;

    obj->vtable = D_80157FA8;
    obj->kind = 3;
    obj->vtable = D_80158308;
    tmp.value = *src & 7;
    copy_small(&obj->field_8, &tmp);
    return obj;
}
