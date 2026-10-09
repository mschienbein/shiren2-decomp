#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { unsigned char pad_00[8]; s32 count_08; } List;
typedef struct { List *list_00; union { u16 index; u8 bytes[2]; } cursor_04; } ListIter800AC6F8;
extern s32 func_800AF920(List *list, u8 index);
s32 func_800B0808(ListIter800AC6F8 *it) {
    for (;;) {
        List *list = it->list_00;
        if (it->cursor_04.index >= list->count_08) return 0;
        if (func_800AF920(list, it->cursor_04.bytes[1])) return 1;
        it->cursor_04.index++;
    }
}
