#include "common.h"

typedef struct VTable VTable;
typedef struct {
    unsigned char pad_00[0x24];
    const VTable *field_24;
} Obj;
extern const VTable D_80149C60;
extern void func_800EFD28(Obj *obj, s32 flags);
extern void func_800A3918(Obj *obj);

void func_80136788(Obj *obj, s32 flags) {
    obj->field_24 = &D_80149C60;
    func_800EFD28(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
