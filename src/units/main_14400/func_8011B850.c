#include "common.h"
#include "item_records.h"

typedef unsigned char u8;


s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 id, ...);

/* Virtual method (vtable D_8015E840 slot 0x44): the receiver and position are unused. */
void func_8011B850(void *self, void *pos, Item *target)
{
    if (target != 0 && target->field00 == 9) {
        if (target->payload.pot.field28 != 0) {
            target->payload.pot.field28 = 0;
            func_80049CB4(0x2E);
            func_800498E4(0xD8, func_800AE674(target));
        } else {
            func_80049CB4(0x132);
            func_800498E4(0x222);
        }
    } else {
        func_80049CB4(0x132);
        func_800498E4(0x223);
    }
}
