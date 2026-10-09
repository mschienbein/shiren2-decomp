#include "common.h"

typedef struct {
    unsigned char unk00[8];
    const void *unk08;
} Object;
extern const unsigned char D_801606D0[68];
extern Object *func_80117230(Object *, s32);

Object *func_80127D40(Object *object) {
    func_80117230(object, 0xF0);
    object->unk08 = D_801606D0;
    return object;
}
