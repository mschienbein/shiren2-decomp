#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* 12-byte node: kind at +0, vtable at +4, 16-bit value at +8. */
typedef struct {
    short kind;
    u8 pad2[2];
    void *vtable;
    u16 value;
} Obj800DA160;

extern u8 D_80157FA8[];
extern u8 D_801581E8[];

Obj800DA160 *func_800DA160(Obj800DA160 *obj, u16 value)
{
    obj->vtable = D_80157FA8;
    obj->kind = 0x3C;
    obj->vtable = D_801581E8;
    obj->value = value;
    return obj;
}
