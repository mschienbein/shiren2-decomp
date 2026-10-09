#include "common.h"

typedef short s16;
typedef unsigned char u8;

typedef struct {
    s16 delta;
    s16 index;
    void *fn;
} VtableEntry;

/* List vtable: slot 7 returns the element at an index (e.g. func_800CE7A0). */
typedef struct {
    VtableEntry entries[7];
    struct {
        s16 delta;
        s16 index;
        void *(*fn)(void *list, u32 index);
    } get;
} ListVtable;

/* Partial list view: +0x0 holds the primary table pointer, +0x4 the list table. */
typedef struct {
    const void *field_0;
    ListVtable *vtable;
} List;

typedef struct {
    u8 pad00[0xC];
    List list;
} Owner801142D0;

extern void *func_8011422C(Owner801142D0 *obj);
extern void func_800CD304(List *list, u32 value);

/* Take the element at index out of the owner's list. */
void *func_801142D0(Owner801142D0 *obj, u32 index) {
    List *list = &obj->list;
    void *item = list->vtable->get.fn((char *)list + list->vtable->get.delta, index);

    func_800CD304(func_8011422C(obj), index);
    return item;
}
