#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef unsigned short u16;
/* Record iterator: the pool, then a halfword cursor whose low byte indexes the record. */
typedef struct { ItemPool *list_00; union { u16 index; u8 bytes[2]; } cursor_04; } ListIter800AC6F8;
s32 func_800B0808(ListIter800AC6F8 *it) {
    for (;;) {
        ItemPool *list = it->list_00;
        if (it->cursor_04.index >= list->capacity) return 0;
        if (func_800AF920(list, it->cursor_04.bytes[1])) return 1;
        it->cursor_04.index++;
    }
}
