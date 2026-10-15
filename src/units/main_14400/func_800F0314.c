#include "common.h"
#include "item_records.h"
typedef struct { unsigned char pad0[0x9C]; unsigned char field_9C; } Obj800F0314;
s32 func_800AC670(void *key);
/* The usable id-0xAC item whose pool D_80143094 index the owner holds at +0x9C, or 0. */
Item *func_800F0314(Obj800F0314 *obj) {
    Item *p;
    s32 bad;
    p = func_800AFD78(&D_80143094, obj->field_9C);
    bad = 0;
    if (p == 0) bad = 1;
    else if (func_800AC670(p) != 0) bad = 1;
    else if (p->field01 != 0xAC) bad = 1;
    return bad ? 0 : p;
}
