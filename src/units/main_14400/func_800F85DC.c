#include "common.h"
typedef struct Object { unsigned char fields00[0x58]; struct Object *field58; } Object;
extern void *D_801476B8;
extern s32 func_800E8694(Object *object);
extern s32 func_800E8350(Object *object);
extern Object *func_800A492C(Object *object, s32 type, s32 first, s32 second);
extern s32 func_800E7104(Object *object);
extern s32 func_800E7424(Object *object, void *target);
s32 func_800F85DC(Object *object) {
    Object *value;
    s32 result;
    if (func_800E8694(object)) {
        result = func_800E8350(object);
    } else {
        value = object->field58;
        if (value == 0) {
            value = func_800A492C(object, 2, 1, 1);
        }
        if (value == 0) {
            result = func_800E7424(object, D_801476B8);
        } else {
            object->field58 = value;
            result = func_800E7104(object);
        }
    }
    return result;
}
