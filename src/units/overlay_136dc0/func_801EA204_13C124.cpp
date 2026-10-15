#include "func_801EA204_13C124/types.h"

/* Index of the last entry of entry's nested list whose id byte (byId) or kind byte
 * (!byId) equals id, or -1. */
extern "C" s32 func_801EA204_13C124(ListEntry *entry, s32 id, s32 byId) {
    u16 i = entry->list_0C.count();
    while (i--) {
        List *list = &entry->list_0C;
        ListEntry *item = list->table_04->get_3C((u8 *)list + list->table_04->delta_38, i);
        if ((item->id_01 == id || !byId) && (item->kind_00 == id || byId)) {
            return i;
        }
    }
    return -1;
}
