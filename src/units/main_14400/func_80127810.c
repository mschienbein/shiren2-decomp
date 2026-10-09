#include "common.h"
typedef struct VTable VTable;
typedef struct { unsigned char pad_00[8]; const VTable *field_08; s32 field_0C; } Object;
extern const VTable D_80160640;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Object *func_801128F0(Object *object, s32 type);
Object *func_80127810(void) {
    Object *object = func_800AC5B4(0x10, 0);
    func_801128F0(object, 0xED);
    object->field_08 = &D_80160640;
    return object;
}
