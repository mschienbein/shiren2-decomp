#include "common.h"
typedef unsigned char u8;
typedef struct {
    s32 field_0;
    u32 field_4, field_8;
    s32 field_C;
    u8 field_10;
    char fields_11[3];
    void *field_14, *field_18;
    s32 field_1C, field_20;
} Object;
extern s32 D_8014A87C[];
extern char D_8014A8E8[], D_8014A894[], D_8014A8AC[];
extern u32 D_801541E0, D_801541E4[];
extern Object *func_800CA048(Object *, s32, s32);
extern u32 func_80044580(Object *, s32);
extern s32 func_800CA118(Object *);
Object *func_80044308(Object *object, s32 index, s32 *status) {
    s32 value;
    func_800CA048(object, 2, index);
    value = D_8014A87C[index];
    object->field_18 = D_8014A8E8;
    object->field_20 = -1;
    object->field_1C = value;
    object->field_4 = func_80044580(object, 0);
    object->field_8 = func_80044580(object, 4);
    if (D_801541E4[object->field_10] < object->field_4) {
        *status = 2;
    } else if (object->field_4 == 0 && object->field_8 == 0) {
        *status = 2;
        object->field_14 = D_8014A894;
    } else if (object->field_8 != func_800CA118(object)) {
        object->field_0 = 8;
        object->field_4 = 8;
        object->field_8 = D_801541E0;
        *status = 2;
        object->field_14 = D_8014A8AC;
    } else {
        object->field_0 = 8;
        object->field_14 = 0;
        *status = 3;
    }
    return object;
}
