#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef struct {
    u8 field_00[0x18];
    short field_18;
    short field_1A;
    void (*field_1C)(void *, s32);
} Methods;
typedef struct { s32 field_00; Methods *field_04; } Member;
typedef struct { u8 field_00[0xC]; Member field_0C; } Object;
extern void *func_8011422C(Object *);
extern s32 func_800CD5C0(void *container, void *item);
extern s32 func_800CD1FC(void *);

static inline void set_methods(Handle *collection, const HandleVTable *methods) {
    collection->vtable = methods;
}

static inline void initialize_collection(Handle *collection) {
    collection->vtable = &D_80149DB8;
    func_8013687C(&collection->pool);
    collection->vtable = &D_80149DA8;
}

void func_8012286C(Object *object, u8 value) {
    Handle collection;
    s32 index;
    s32 result;
    Member *member;
    initialize_collection(&collection);
    collection.pool = D_80147FD0[(u8)value];
    index = 0;
    for (;;) {
        s32 has_more = index < 2;
        Item *element;
        if (!has_more) break;
        element = func_800D55F0(&collection, index);
        if (element) {
            func_800CD5C0(func_8011422C(object), element);
        } else {
            func_800D4AE8(&collection, index);
        }
        index++;
    }
    result = func_800CD1FC(func_8011422C(object));
    member = &object->field_0C;
    member->field_04->field_1C((u8 *)member + member->field_04->field_18, result);
    set_methods(&collection, &D_80149DA8);
    collection.pool = 0;
    set_methods(&collection, &D_80149DB8);
}
