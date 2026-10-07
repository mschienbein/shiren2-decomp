#include "common.h"

typedef unsigned char u8;
typedef struct { void *field_00; void *field_04; } Collection;
typedef struct {
    u8 field_00[0x18];
    short field_18;
    short field_1A;
    void (*field_1C)(void *, s32);
} Methods;
typedef struct { s32 field_00; Methods *field_04; } Member;
typedef struct { u8 field_00[0xC]; Member field_0C; } Object;
extern s32 D_80149DB8[];
extern s32 D_80149DA8[];
/* Three-entry table of pool-record addresses: D_801430D4, D_801430E4, D_801430F4. */
extern void *D_80147FD0[3];
extern void func_8013687C(void **pool);
extern void *func_800D55F0(void *owner, u32 selector);
extern void func_800D4AE8(void *owner, s32 key);
extern void *func_8011422C(Object *);
extern s32 func_800CD5C0(void *container, void *item);
extern s32 func_800CD1FC(void *);

static inline void set_methods(Collection *collection, void *methods) {
    collection->field_04 = methods;
}

static inline void initialize_collection(Collection *collection) {
    collection->field_04 = D_80149DB8;
    func_8013687C(&collection->field_00);
    collection->field_04 = D_80149DA8;
}

void func_8012286C(Object *object, u8 value) {
    Collection collection;
    s32 index;
    s32 result;
    Member *member;
    initialize_collection(&collection);
    collection.field_00 = D_80147FD0[(u8)value];
    index = 0;
    for (;;) {
        s32 has_more = index < 2;
        void *element;
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
    set_methods(&collection, D_80149DA8);
    collection.field_00 = 0;
    set_methods(&collection, D_80149DB8);
}
