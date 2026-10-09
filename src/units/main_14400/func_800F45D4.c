#include "common.h"
extern s32 func_800E20CC(void *self);
extern s32 func_800F4498(void *self);
/* Event dispatch supplies payload; this handler only needs the receiver. */
s32 func_800F45D4(void *self, void *payload) {
    if (func_800E20CC(self)) {
        return func_800F4498(self);
    }
    return 0;
}
