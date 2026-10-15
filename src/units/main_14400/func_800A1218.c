#include "common.h"
#include "item_records.h"

s32 func_800A1218(ItemCursor *cursor) {
    func_8010BE60(cursor->item, cursor->index.byte[3]);
    return cursor->item->field00;
}
