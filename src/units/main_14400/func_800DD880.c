#include "common.h"
typedef struct { s32 field_0; const void *field_4; } Object;
extern const unsigned char D_80158898[48];
extern Object *func_800DA8A0(Object *, s32, void *);
Object *func_800DD880(Object *object, void *value) {
    func_800DA8A0(object, 0x26, value);
    object->field_4 = D_80158898;
    return object;
}
