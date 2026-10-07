#include "common.h"
void func_801130C0(void *obj, void *entity);
/* Item-effect slot +0x44 also supplies an item; this override does not use it. */
void func_8011BF3C(void *obj, void *entity, void *item) { func_801130C0(obj, entity); }
