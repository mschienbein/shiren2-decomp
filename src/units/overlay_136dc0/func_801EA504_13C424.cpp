#include "func_801EA504_13C424/types.h"

extern "C" s32 func_801EA504_13C424(s32 owner) {
    List *list = func_801EA2D8_13C1F8(owner);
    s32 i;
    s32 j;
    if (list == 0) {
        return 0;
    }
    i = list->count();
    while (i--) {
        ListEntry *item = list->get(i);
        func_800ACD34(item);
        if (item->kind_00 == 9) {
            j = item->list_0C.count();
            while (j--) {
                ListEntry *child = item->list_0C.get(j);
                func_800ACD34(child);
            }
        }
    }
    return 0;
}
