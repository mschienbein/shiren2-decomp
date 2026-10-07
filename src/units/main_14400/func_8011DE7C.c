#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
} Obj8011DE7C;

u8 func_8011DE7C(Obj8011DE7C *obj) {
    return obj->unk10;
}
