#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 unk0;
    s32 unk4;
    void *unk8;
} Object_8011E930;

extern u8 D_8015F078[];
/* Returns the input object through this TU's partial view. */
Object_8011E930 *func_80111530(Object_8011E930 *obj, s32 arg1);

Object_8011E930 *func_8011E930(Object_8011E930 *obj) {
    func_80111530(obj, 148);
    obj->unk8 = D_8015F078;
    return obj;
}
