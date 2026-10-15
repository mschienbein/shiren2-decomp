#include "func_801EA440_13C360/types.h"

extern "C" s32 func_801EA440_13C360(s32 owner, s32 kind) {
    List *list = func_801EA2D8_13C1F8(owner);
    s32 i;
    if (list == 0) {
        return 0;
    }
    i = list->count();
    while (i--) {
        ListEntry *item = list->get(i);
        if (item->kind_00 == kind) {
            return 1;
        }
        if (item->kind_00 == 9 && func_801EA204_13C124(item, kind, 0) != -1) {
            return 1;
        }
    }
    return 0;
}
