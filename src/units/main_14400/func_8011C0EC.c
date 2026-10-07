#include "common.h"

void func_801130C0(void *self, void *actor);
/* Item-effect slot +0x44 also supplies an item; this override does not use it. */
void func_8011C0EC(void *self, void *actor, void *item) { func_801130C0(self, actor); }
