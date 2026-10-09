#include "common.h"
typedef struct { s32 field_00; u32 field_04; u32 field_08; s32 field_0C; unsigned char field_10; unsigned char pad_11[3]; const char *field_14; s32 *field_18; u32 field_1C; unsigned char pad_20[0x20]; s32 field_40; s32 field_44; } Object;
extern s32 D_8014A840[];
extern u32 D_8014A7EC[];
extern s32 D_80138B00;
extern u32 D_801541E4[];
extern u32 D_801541E0;
extern const char D_8014A804[];
extern Object *func_800CA048(Object *object, s32 kind, s32 index);
extern u32 func_80043B48(u32 address);
extern s32 func_800CA118(Object *object);
static inline void initialize(Object *object, s32 value, s32 *status, s32 index) {
    func_800CA048(object, value, index);
    object->field_18 = D_8014A840;
    object->field_40 = -1;
    object->field_44 = 0;
    switch (object->field_10) {
        case 0: object->field_1C = 0x08005000; break;
        case 1: object->field_1C = D_8014A7EC[index]; break;
        case 3: object->field_1C = 0x08007E00; break;
        case 2:
        default: *status = 0; return;
    }
    if (D_80138B00) {
        object->field_04 = 8;
        object->field_08 = 0;
        *status = 0;
        return;
    }
    object->field_04 = func_80043B48(object->field_1C);
    object->field_08 = func_80043B48(object->field_1C + 4);
    if (D_801541E4[object->field_10] < object->field_04) {
        *status = 2;
    } else if (object->field_04 == 0 && object->field_08 == 0) {
        object->field_04 = 8;
        object->field_08 = D_801541E0;
        *status = 1;
    } else if (object->field_08 != func_800CA118(object)) {
        object->field_00 = 8;
        object->field_04 = 8;
        object->field_08 = D_801541E0;
        *status = 2;
        object->field_14 = D_8014A804;
    } else {
        object->field_00 = 8;
        object->field_14 = 0;
        *status = 3;
    }
}
Object *func_800439A8(Object *object, s32 value, s32 index, s32 *status) {
    initialize(object, value, status, index);
    return object;
}
