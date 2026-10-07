#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Collection vtable (D_801545E0 family): slot +0x20 is the count method (func_800D0414). */
typedef struct {
    char pad0[0x20];
    short delta_20;
    short index_22;
    s32 (*count_24)(void *self);
} CollectionVTable800DEBF8;

/*
 * Inline collection subobject built by func_800D0380/func_800D03B4:
 * +4 vtable, +8 element storage, +0xC capacity, +0x10 count.
 */
typedef struct {
    s32 field_0;
    CollectionVTable800DEBF8 *vtable;
    void *elements;
    s32 capacity;
    s32 count;
} Collection800DEBF8;

/* Partial owner view: the fields below are the ones this function touches. */
typedef struct {
    u8 field_0;
    u8 field_1;
    u8 pad2[0xB0 - 0x2];
    Collection800DEBF8 collection;
} Obj800DEBF8;

void func_800DDB64(Obj800DEBF8 *obj, u8 *buf, s32 count);

s32 func_800DEBF8(Obj800DEBF8 *obj, u8 *buf) {
    Collection800DEBF8 *collection = &obj->collection;
    CollectionVTable800DEBF8 *vtable = collection->vtable;
    s32 count = vtable->count_24((char *)collection + vtable->delta_20);

    *buf++ = obj->field_1;
    *buf++ = count + 1;
    func_800DDB64(obj, buf, count);
    return count + 3;
}
