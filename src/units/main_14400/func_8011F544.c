#include "common.h"

typedef struct { s32 x; s32 y; } Position;
extern void func_8011F560(void *self, void *actor, const Position *position, const unsigned char *direction);

/* Trap apply slot +0x54 supplies attacker too; this forwarding override
 * intentionally ignores it and forwards the first four pointers to the helper. */
void func_8011F544(void *self, void *actor, void *position, void *direction, void *attacker) {
    func_8011F560(self, actor, position, direction);
}
