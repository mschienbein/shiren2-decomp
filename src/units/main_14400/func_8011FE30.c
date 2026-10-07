#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj_8011FE30;

extern u8 D_8015F578[];
extern void func_8010E290(Obj_8011FE30 *obj, s32 kind);

Obj_8011FE30 *func_8011FE30(Obj_8011FE30 *obj) {
    func_8010E290(obj, 0xA1);
    obj->field_8 = D_8015F578;
    return obj;
}
