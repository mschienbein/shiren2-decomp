#include "common.h"
#include "item_records.h"
/* Item container: pool +0 (+4 not accessed here), modified flag +8. Records are copied
 * whole. */
typedef struct { ItemPool *field00; unsigned char pad04[4]; s32 field08; } Owner;
extern void func_800AE648(Item *entry);
static inline s32 needs_move(ItemPool *container, unsigned char index) {
    return func_800AF920(container, index) ^ 1;
}
void func_800D0E38(Owner *owner) {
    s32 source = 0;
    s32 destination;
    s32 count;
    Item *entry;
    if (owner->field08 != 0) {
        count = owner->field00->capacity;
        destination = 0;
        while (1) {
            if (destination < count - 1) {
                if (needs_move(owner->field00, destination)) {
                    if (destination >= source) {
                        source = destination + 1;
                    }
                    if (source >= count) {
                        return;
                    }
                    while (func_800AF920(owner->field00, source) == 0) {
                        source++;
                        if (source >= count) {
                            break;
                        }
                    }
                    if (source >= count) {
                        return;
                    }
                    entry = func_800AFD78(owner->field00, destination);
                    *entry = *func_800AFD78(owner->field00, source);
                    func_800AFCD8(owner->field00, destination);
                    func_800AFCA8(owner->field00, source++);
                    func_800AE648(entry);
                }
                destination++;
            } else {
                owner->field08 = 0;
                break;
            }
        }
    }
}
