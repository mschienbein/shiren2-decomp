#include "common.h"

typedef struct {
    char pad0[8];
    char field_8[4];
    s32 field_C;
    char pad10[4];
    char field_14[8];
    s32 field_1C;
} Obj;

extern void func_800C25D0(Obj *obj, void *a, void *b, s32 c);

void func_800C5244(Obj *obj) {
    func_800C25D0(obj, obj->field_14, obj->field_8, obj->field_C);
    obj->field_1C = 1;
}
