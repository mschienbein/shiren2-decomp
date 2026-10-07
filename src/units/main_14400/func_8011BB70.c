#include "common.h"

void func_801130C0(void *self, void *target);
/* Item-effect slot +0x44 also supplies an item; this override does not use it. */
void func_8011BB70(void *self, void *target, void *item) {
    func_801130C0(self, target);
}
