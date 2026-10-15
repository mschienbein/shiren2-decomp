#include "func_801E5AE0_134630/types.h"

void func_80047624(List801E5A30 *win, Item801E5A30 *entry, s32 index, s32 detailed);

static inline s32 has_item(List801E5A30 *list, s32 i) {
    return i < list->count;
}

void func_801E5AE0_134630(List801E5A30 *list) {
    s32 i = 0;
    s32 detailed = list->mode != 1;

    for (; has_item(list, i); i++) {
        func_80047624(list, &list->items[i], i, detailed);
    }
}
