#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj_8011BC00;

extern u8 D_8015E8E0[];
/* Returns the input object through this TU's partial view. */
extern Obj_8011BC00 *func_80112D20(Obj_8011BC00 *obj, s32 kind);

Obj_8011BC00 *func_8011BC00(Obj_8011BC00 *obj) {
    func_80112D20(obj, 0x25);
    obj->field_8 = D_8015E8E0;
    return obj;
}
