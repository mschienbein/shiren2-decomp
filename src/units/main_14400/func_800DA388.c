#include "common.h"

typedef unsigned short u16;
typedef struct {
    u16 field_00;
    u16 field_02;
    void *volatile field_04;
} Object;
extern s32 D_80157FA8[];
extern s32 D_80158278[];

Object *func_800DA388(Object *object) {
    object->field_04 = D_80157FA8;
    object->field_00 = 0x38;
    object->field_04 = D_80158278;
    return object;
}
