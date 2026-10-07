#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj_8011E080;

extern u8 D_8015EEC8[];
/* Returns the input object through this TU's partial view. */
extern Obj_8011E080 *func_80111530(Obj_8011E080 *obj, s32 kind);

Obj_8011E080 *func_8011E080(Obj_8011E080 *obj) {
    func_80111530(obj, 0x90);
    obj->field_8 = D_8015EEC8;
    return obj;
}
