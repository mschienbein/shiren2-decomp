#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj8011F310;

void *func_800AC5B4(s32 size, s32 alternate);
Obj8011F310 *func_8011F310(Obj8011F310 *obj);

Obj8011F310 *func_8011F348(void) {
    return func_8011F310(func_800AC5B4(0x14, 0));
}
