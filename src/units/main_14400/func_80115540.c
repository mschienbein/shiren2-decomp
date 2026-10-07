#include "common.h"

/* Container slot +0x40/+0x44: func_80114E28 passes an actor and an item,
 * then forwards the returned item to func_800CD5C0 (80114F4C..80114F7C).
 * This pass-through override intentionally ignores self and actor. */
void *func_80115540(void *self, void *actor, void *item) {
    return item;
}
