#include "func_801EA364_13C284/types.h"

extern "C" s32 func_801EA364_13C284(s32 owner, s32 id) {
    List *list = func_801EA2D8_13C1F8(owner);
    s32 i;
    if (list == 0) {
        return 0;
    }
    i = list->count();
    if (id == -1) {
        return i == 0;
    }
    while (i--) {
        ListEntry *item = list->get(i);
        if (item->id_01 == id) {
            return 1;
        }
        if (item->kind_00 == 9 && func_801EA204_13C124(item, id, 1) != -1) {
            return 1;
        }
    }
    return 0;
}
