#include "common.h"
typedef struct { s32 fields00[2]; const void *field08; } Object;
extern const unsigned char D_80160688[72];
extern Object *func_80117230(Object *object, s32 type);
Object *func_80127BF0(Object *object) {
    func_80117230(object, 0xEF);
    object->field08 = D_80160688;
    return object;
}
