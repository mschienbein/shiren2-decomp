#include "common.h"
typedef struct { char pad[0x4C]; const void *vtbl; } Obj800970B8;
extern const unsigned char D_80151E38[144];
void func_800D8FA8(void *object);
/* Destructor bound at D_80152000+0x54 (widget family destroy slot, like the root's func_800960E0):
 * void (void *self, s32 flags). */
void func_800970B8(Obj800970B8 *self, s32 flags) {
    self->vtbl = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(self);
    }
}
