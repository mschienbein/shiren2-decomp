#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x11];
    u8 field_11;
} Obj8011DE64;

void func_8011DE64(Obj8011DE64 *obj) {
    obj->field_11 = 1;
}
