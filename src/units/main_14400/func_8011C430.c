#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 unk0;
    s32 unk4;
    void *unk8;
} Object_8011C430;

extern u8 D_8015EA70[];
/* Returns the input object through this TU's partial view. */
Object_8011C430 *func_80112D20(Object_8011C430 *obj, s32 arg1);

Object_8011C430 *func_8011C430(Object_8011C430 *obj) {
    func_80112D20(obj, 42);
    obj->unk8 = D_8015EA70;
    return obj;
}
