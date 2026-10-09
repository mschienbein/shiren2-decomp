#include "common.h"
typedef struct VTable VTable;
typedef struct { unsigned char pad_0[0x24]; VTable *field_24; } S;
typedef struct Obj800A38A0 Obj800A38A0;
extern VTable D_80149980;
extern void func_800EFD28(S *, s32);
extern void func_800A3918(Obj800A38A0 *obj);
void func_80136638(S *obj, s32 flags) {
    obj->field_24 = &D_80149980;
    func_800EFD28(obj, 0);
    if (flags & 1) func_800A3918((Obj800A38A0 *)obj);
}
