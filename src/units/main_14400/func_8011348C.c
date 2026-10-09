#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad00[0xC];
    u8 flags0C;
} Obj8011348C;

void func_8011348C(Obj8011348C *obj)
{
    obj->flags0C |= 2;
}
