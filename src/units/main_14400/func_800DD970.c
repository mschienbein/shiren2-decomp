#include "common.h"
typedef struct { s32 field00; const void *field04; } Object;
extern const s32 D_801588C8[12]; /* Complete 0x30-byte command vtable. */
extern Object *func_800DA8A0(Object *object, s32 type, void *context);
Object *func_800DD970(Object *object, void *context) {
    func_800DA8A0(object, 0x27, context);
    object->field04 = &D_801588C8;
    return object;
}
