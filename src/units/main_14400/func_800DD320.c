#include "common.h"
extern s32 D_801587A8[];
extern void *func_800DA8A0(void *obj, s32 kind, void *src);
typedef struct { s32 field_00; s32 *field_04; } Object;
Object *func_800DD320(Object *object, void *src) {
    func_800DA8A0(object, 0x21, src);
    object->field_04 = D_801587A8;
    return object;
}
