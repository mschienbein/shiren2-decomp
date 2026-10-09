#include "common.h"
typedef struct { void *field00; void *field04; s32 field08; const void *field0C; s32 field10; s32 field14; } Object;
extern const unsigned char D_80154058[48];
extern Object *func_800C561C(Object *object);
extern void func_800C5CD0(Object *object);
Object *func_800C5C50(Object *object) {
    func_800C561C(object);
    object->field0C = D_80154058;
    object->field00 = &object->field10;
    object->field04 = &object->field14;
    func_800C5CD0(object);
    return object;
}
