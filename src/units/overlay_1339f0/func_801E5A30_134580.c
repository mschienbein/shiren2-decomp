#include "func_801E5A30_134580/types.h"

u8 *func_8006A810(u8 *dst, s32 value, s32 count);
void func_8009543C(List801E5A30 *obj, Layout801E5A30 *desc);

void func_801E5A30_134580(List801E5A30 *list, Item801E5A30 *items, s32 count, s32 *origin, s32 style) {
    Layout801E5A30 copy;
    Layout801E5A30 layout;

    func_8006A810((u8 *)&layout, 0, sizeof(layout));
    layout.unk0 = count * 4;
    layout.unk4 = style;
    layout.unk8 = origin[0];
    layout.unkC = origin[1];
    copy = layout;
    func_8009543C(list, &copy);
    list->items = items;
    list->count = count;
}
