#include "common.h"

typedef unsigned char u8;
typedef struct { u8 field0; u8 field1; } Item;
typedef struct {
    u8 pad0[0x20];
    short adjust20; short pad22;
    s32 (*field24)(void *);
    u8 pad28[0x10];
    short adjust38; short pad3A;
    Item *(*field3C)(void *, u32);
} VTable;
typedef struct { u8 pad0[4]; VTable *field4; } List;
extern void *func_800AC244(u8 id);
extern s32 func_800CD538(void *self, void *obj);

void func_800CE358(void *destination, List *source)
{
    u32 index = 0;
    while (index < (u32)source->field4->field24((u8 *)source + source->field4->adjust20)) {
        Item *entry = source->field4->field3C((u8 *)source + source->field4->adjust38, index);
        void *item = func_800AC244(entry->field1);
        if (item == 0) {
            break;
        }
        func_800CD538(destination, item);
        ++index;
    }
}
