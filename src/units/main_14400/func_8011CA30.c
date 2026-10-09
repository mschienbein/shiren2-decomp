#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 field_0;
    u8 field_1;
    u8 pad2[0xA];
    u8 flags_C;
} Obj8011CA30;

extern s32 func_800AD468(u32 bit);

s32 func_8011CA30(Obj8011CA30 *obj, s32 kind) {
    s32 result;

    if (kind == 5 || kind == 0xB) {
        return 1;
    }
    result = 0;
    if (func_800AD468(obj->field_1) && !(obj->flags_C & 1)) {
        result = kind == 8;
    }
    return result;
}
