#include "common.h"
typedef struct { s32 field_0, field_4; const void *field_8; } Object;
extern const unsigned char D_801605F8[72];
extern Object *func_801128F0(Object *, s32);
Object *func_801277A8(Object *object) {
    func_801128F0(object, 0xEC);
    object->field_8 = D_801605F8;
    return object;
}
