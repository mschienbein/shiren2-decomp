#include "common.h"
typedef struct { void *collection; void *item; } Link;
typedef struct { unsigned short kind; unsigned short pad_2; void *vtable; } Base;
typedef struct { s32 field_0; void *vtable; Link *entries; s32 capacity; s32 count; } List;
/* The base owns 21 eight-byte links at +8 and the list at +0xB0. */
typedef struct { Base base; Link entries[21]; List list_B0; } Object;
extern void func_800D05A4(List *, Link *);
void func_800DDD28(Object *object, Link *link) {
    func_800D05A4(&object->list_B0, link);
}
