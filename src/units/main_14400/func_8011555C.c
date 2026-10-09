#include "common.h"

typedef struct {
    char pad0[0x28];
    unsigned char flag28;
} Obj8011555C;

void func_8011555C(Obj8011555C *obj)
{
    obj->flag28 = 1;
}
