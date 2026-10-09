#include "common.h"
typedef struct { unsigned char field_00[8]; const void *field_08; } Object;
extern const unsigned char D_8015DA28[];
extern void *func_8010C8C0(Object *object, s32 kind, s32 subtype);
Object *func_80116C30(Object *object, s32 subtype) {
    func_8010C8C0(object, 12, subtype);
    object->field_08 = D_8015DA28;
    return object;
}
