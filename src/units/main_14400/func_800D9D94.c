#include "common.h"

typedef struct VTable VTable;
typedef struct {
    unsigned char pad_00[4];
    const VTable *field_04;
} Obj;
extern const VTable D_80157FA8;
extern void func_800D8FE8(void *object);

void func_800D9D94(Obj *obj, s32 flags) {
    obj->field_04 = &D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
