#include "common.h"

/* Linked object kept at +0x80 by the D_801599F8 class; sibling methods dereference it and pass it
 * to func_800D2C5C/func_800D2FB0. Opaque in this TU. */
typedef struct Link Link;

typedef struct {
    unsigned char pad0[0x80];
    Link *field_80;
} Obj800F6B58;

void func_800F6B58(Obj800F6B58 *obj, Link *value) {
    obj->field_80 = value;
}
