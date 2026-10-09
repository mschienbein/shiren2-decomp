#include "common.h"

typedef struct Object_8011E930 Object_8011E930;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Object_8011E930 *func_8011E930(Object_8011E930 *obj);
Object_8011E930 *func_8011E968(void)
{
    return func_8011E930(func_800AC5B4(0x10, 0));
}
