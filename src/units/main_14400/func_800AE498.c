#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
extern u32 D_8013960C;
s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 message_id, ...);

s32 func_800AE498(Item *obj)
{
    if (obj->flags02 & 4) {
        if (obj->field08->test14((u8 *)obj + obj->field08->delta10)) {
            if (D_8013960C & 1) {
                func_80049CB4(0x12B);
                func_800498E4(0x2F, func_800AE674(obj));
            }
            return 0;
        }
    }
    return 1;
}
