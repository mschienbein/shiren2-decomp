#include "common.h"

typedef short s16;
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s16 delta;
    s16 index;
    void (*func)(void *self, s32 size, void *data);
} VtableEntry;

typedef struct {
    char pad0[0x18];
    VtableEntry entry18;
} Vtable800CE918;

typedef struct {
    char pad0[0x18];
    Vtable800CE918 *vtable;
} Obj800ECB5C;

/* Embedded 0x18-byte list: item bytes, their count and two header bytes. */
typedef struct {
    void *context;
    void *vtable;
    u8 *items;
    u8 field_C;
    u8 field_D;
    u8 field_E;
    void *owner;
    u16 label;
} List800ECB5C;

extern u8 D_80154384[];

void func_800CA4A4(void *, void *);

/* Serializes the list header bytes and then its items into obj's stream. */
void func_800CE918(List800ECB5C *list, Obj800ECB5C *obj)
{
    func_800CA4A4(obj, D_80154384);
    obj->vtable->entry18.func((u8 *)obj + obj->vtable->entry18.delta, 1, &list->field_C);
    obj->vtable->entry18.func((u8 *)obj + obj->vtable->entry18.delta, 1, &list->field_D);
    obj->vtable->entry18.func((u8 *)obj + obj->vtable->entry18.delta, 1, &list->field_E);
    obj->vtable->entry18.func((u8 *)obj + obj->vtable->entry18.delta, list->field_C, list->items);
}
