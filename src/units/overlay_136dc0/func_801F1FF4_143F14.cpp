extern "C" {
#include "func_801F1FF4_143F14/types.h"
}

/* ODD_C: real 0/1 complement predicates retain the s32 external contracts.
 * Direct ^1 branch expressions in the first repaired receipt differed by
 * 66 words; the compiler compared against 1 instead of keeping the XOR. */
static inline s32 requirement_fails(u8 id, u8 low, u8 high) {
    return func_801F1974_143894(id, low, high) ^ 1;
}

static inline s32 item_is_available(void *item) {
    return func_800AFBFC(item) ^ 1;
}

extern "C" void func_801F1FF4_143F14(void) {
    Pos801F1FF4 pos;
    Placement801F1FF4 rec;
    u16 first = 0;
    u16 last = 0;
    s32 i;
    Item801F1FF4 *item;

    func_801E5B50_137A70(1, D_80142F18.mode, &first, &last);
    func_800B15C8(0xF4);
    func_800B03F4();
    for (i = first; i < last; i++) {
        func_801E5BE4_137B04((u32)i, 0xC, (u8 *)&rec);
        if (rec.floor != -1 && rec.floor != D_80142F18.coordinates[0]) {
            continue;
        }
        if (requirement_fails(rec.req_id, rec.req_low, rec.req_high)) {
            continue;
        }
        item = (Item801F1FF4 *)func_800AC244(0xF4);
        if (item_is_available(item)) {
            item->field_0C = rec.field_08;
            item->field_0E = rec.field_02;
            item->field_10 = rec.field_0A;
            pos.y = rec.y;
            pos.x = rec.x;
            func_800AD7E0(item, &pos, 1);
        }
    }
}
