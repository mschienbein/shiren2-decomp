#include "common.h"

typedef unsigned short u16;
typedef struct {
    unsigned char pad_00[0x18];
    signed short delta_18;
    unsigned short reserved_1A;
    s32 (*test_1C)(void *self, s32 kind);
} ItemVtable;
typedef struct { unsigned char pad_00[8]; ItemVtable *vtable_08; } Item;
typedef struct {
    unsigned char pad_00[0x60];
    signed short delta_60;
    unsigned short reserved_62;
    s32 (*insert_64)(void *self, void *item, s32 verbose);
} ListVtable;
typedef struct { void *table_00; ListVtable *vtable_04; } List;
extern u16 func_800AE710(void *item);
extern s32 func_800AC1FC(void);
extern void func_800498E4(s32 id, ...);

s32 func_800CD7E0(List *list, Item *item, s32 notify)
{
    s32 multiple = 0;
    ItemVtable *item_vtable = item->vtable_08;
    if (item_vtable->test_1C((char *)item + item_vtable->delta_18, 0x1E)) {
        multiple = (u32)func_800AE710(item) >= 2;
    }
    if (multiple) {
        s32 failed = func_800AC1FC() != 1;
        if (failed) {
            if (notify) {
                /* Message 0x9F has no conversion specifiers. */
                func_800498E4(0x9F);
            }
            return 0;
        }
    }
    {
        ListVtable *vtable = list->vtable_04;
        return vtable->insert_64((char *)list + vtable->delta_60, item, notify);
    }
}
