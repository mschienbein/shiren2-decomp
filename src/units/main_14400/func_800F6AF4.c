#include "common.h"

typedef unsigned char u8;

/* Linked object kept at +0x80 by the D_801599F8 class; sibling methods dereference it and pass it
 * to func_800D2C5C/func_800D2FB0. Opaque in this TU. */
typedef struct Link Link;

typedef struct {
    u8 pad0[0x80];
    Link *field_80;
} Obj;

Link *func_800F6AF4(Obj *obj) {
    return obj->field_80;
}
