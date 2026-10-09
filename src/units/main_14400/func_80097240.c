#include "common.h"

typedef struct Item Item;
typedef struct Layout Layout;
typedef struct {
    s32 count;
    s32 width;
} Selection;
typedef struct {
    unsigned char pad_00[0x20];
    s32 field_20;
    s32 field_24;
    unsigned char pad_28[0x28];
    Item *field_50;
    s32 field_54;
    s32 field_58;
} Menu;
extern void func_8009543C(Menu *obj, Layout *desc);

void func_80097240(Menu *menu, Item *items, Layout *layout, Selection *sel) {
    func_8009543C(menu, layout);
    menu->field_50 = items;
    menu->field_24 = (sel->count - 1) / menu->field_20 + 1;
    menu->field_54 = sel->width;
    menu->field_58 = 1;
}
