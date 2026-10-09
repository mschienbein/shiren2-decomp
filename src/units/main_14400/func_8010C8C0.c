#include "common.h"

typedef struct VTable VTable;
typedef struct S {
    unsigned char pad_00[8];
    const VTable *field_08;
    short field_0C;
} S;
extern const VTable D_8015D0D0;
extern void *func_800AC0C0(S *self, s32 a, s32 b);

S *func_8010C8C0(S *obj, s32 a, s32 b) {
    func_800AC0C0(obj, a, b);
    obj->field_08 = &D_8015D0D0;
    obj->field_0C = 1;
    return obj;
}
