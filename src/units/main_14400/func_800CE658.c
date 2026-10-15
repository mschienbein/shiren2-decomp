#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
/* Item list: pool +0 (always D_80143094 here), method table +4, entry buffer +8,
 * capacity +0xC and limit +0xD. */
typedef struct {
    ItemPool *field_00;
    u8 pad_04[4];
    u8 *field_08;
    u8 field_0C;
    u8 field_0D;
} Obj;
extern void func_800CCF80(Obj *o);

void func_800CE658(Obj *obj, u8 *arg1, u8 arg2) {
    obj->field_00 = &D_80143094;
    obj->field_08 = arg1;
    obj->field_0C = arg2;
    func_800CCF80(obj);
    obj->field_0D = arg2;
}
