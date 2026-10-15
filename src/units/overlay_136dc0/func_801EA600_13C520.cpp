#include "func_801EA600_13C520/types.h"

extern "C" void func_801EA600_13C520(s32 owner, s32 id) {
    List *list = func_801EA2D8_13C1F8(owner);
    ListEntry *item;
    s32 i;
    if (list == 0) {
        return;
    }
    if (id == 0) {
        func_800CD468(list);
        return;
    }
    item = func_800CDA70(list, id);
    if (item != 0) {
        func_800CD364(list, item);
        return;
    }
    i = list->count();
    while (i--) {
        s32 index;
        item = list->get(i);
        if (item->kind_00 != 9) {
            continue;
        }
        index = func_801EA204_13C124(item, id, 1);
        if (index != -1) {
            OvlBItemDispatchView *removed = func_801142D0(item, index);
            if (removed) {
                removed->table_08->destroy_0C((u8 *)removed + removed->table_08->delta_08, 3);
            }
            return;
        }
    }
}
