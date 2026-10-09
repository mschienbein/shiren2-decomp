#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field_00[0x90]; short field_90, field_92; s32 (*field_94)(void *, s32, s32, u8, s32); } VTable;
typedef struct { u8 field_00[0x24]; VTable *field_24; u8 field_28[0xD]; u8 field_35[10]; u8 field_3F[3]; u8 field_42; u8 field_43; u8 field_44; } Object;
extern u32 D_8013960C;
extern s32 func_800E1CD4(Object *object, s32 value);
extern u32 func_800E110C(const Object *object);
/* D_80158C98 + 0x94 is func_800E115C, whose fourth argument is u8. */
static inline s32 dispatch(Object *object, s32 mode, s32 value) {
    VTable *table = object->field_24;
    return table->field_94((u8 *)object + table->field_90, mode, value, 0, 0);
}
void func_800E1ED0(Object *object) {
    s32 i;
    D_8013960C <<= 1;
    if (func_800E1CD4(object, 15)) dispatch(object, 1, 15);
    if (dispatch(object, 2, 9)) dispatch(object, 1, 9);
    object->field_42 = 0;
    object->field_44 = 0;
    i = 0;
    do {
        if (i != 3) object->field_35[i] = 0;
        i++;
    } while (i < 10);
    if ((func_800E110C(object) << 24) == 0) dispatch(object, 1, 17);
    D_8013960C >>= 1;
}
