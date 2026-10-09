#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct {
    const VTable *field_00;
    u8 pad_04[4];
    u8 *field_08;
    u8 field_0C;
    u8 field_0D;
} Obj;
extern const VTable D_80143094;
extern void func_800CCF80(Obj *o);

void func_800CE658(Obj *obj, u8 *arg1, u8 arg2) {
    obj->field_00 = &D_80143094;
    obj->field_08 = arg1;
    obj->field_0C = arg2;
    func_800CCF80(obj);
    obj->field_0D = arg2;
}
