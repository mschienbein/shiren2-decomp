#include "common.h"

typedef struct Obj8011FD60 Obj8011FD60;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Obj8011FD60 *func_8011FD60(Obj8011FD60 *obj);

Obj8011FD60 *func_8011FDD0(void) {
    return func_8011FD60(func_800AC5B4(0x10, 0));
}
