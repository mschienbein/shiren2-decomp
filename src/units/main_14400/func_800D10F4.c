#include "common.h"

typedef struct {
    unsigned char pad_00[0x20];
    signed short delta_20;
    unsigned short reserved_22;
    s32 (*count_24)(void *self);
} ListVtable;
typedef struct { void *table_00; ListVtable *vtable_04; } List;
typedef struct {
    unsigned char pad_00[0x98];
    signed short delta_98;
    unsigned short reserved_9A;
    void *(*list_9C)(void *self);
} ObjectVtable;
typedef struct { void *owner_00; s32 active_04; } Component;
typedef struct {
    unsigned char pad_00[0x24];
    ObjectVtable *vtable_24;
    unsigned char pad_28[0x5C];
    Component component_84;
} Object;
typedef struct { List *current_00; List *shared_04; List *other_08; } Selection;
typedef struct { s32 cursor; } Iterator;
extern Object *D_801476B8;
/* 800D4EA0 indexes the pointer table D_8015483C, not integer values. */
extern void *func_800D4EA0(u32 index);
extern s32 func_800A9070(Iterator *iterator, s32 kind);
extern void *func_800A910C(Iterator *iterator);

void func_800D10F4(Selection *selection)
{
    Object *object = D_801476B8;
    ObjectVtable *vtable = object->vtable_24;
    List *list;
    ListVtable *list_vtable;
    Iterator iterator;
    selection->current_00 = vtable->list_9C((char *)object + vtable->delta_98);
    list = func_800D4EA0(1);
    selection->shared_04 = list;
    list_vtable = list->vtable_04;
    if (list_vtable->count_24((char *)list + list_vtable->delta_20) == 0) {
        selection->shared_04 = 0;
    }
    selection->other_08 = 0;
    iterator.cursor = 0;
    if (func_800A9070(&iterator, 0x1C)) {
        Object *found = func_800A910C(&iterator);
        /* Null-preserving conversion to the +0x84 component; the original
         * reads the flag through the converted pointer unconditionally. */
        Component *component = 0;
        object = found;
        if (found != 0) component = &found->component_84;
        if (component->active_04 != 0) {
            vtable = object->vtable_24;
            selection->other_08 = vtable->list_9C((char *)object + vtable->delta_98);
        }
    }
}
