#include "func_801E5B54_1346A4/types.h"

void func_80045A24(s32 sound);

void func_801E5B54_1346A4(List801E5A30 *list, s32 direction) {
    Cursor801E5B54 cursor = list->cursor;

    switch (direction) {
    case 0:
        if (cursor.offset < (list->count - 1) * 4) {
            cursor.offset += 4;
        } else {
            cursor.offset = 0;
        }
        break;
    case 1:
        if (cursor.offset > 0) {
            cursor.offset -= 4;
        } else {
            cursor.offset = (list->count - 1) * 4;
        }
        break;
    default:
        return;
    }
    list->methods->method_84((u8 *)list + list->methods->delta_80, &cursor);
    func_80045A24(2);
}
