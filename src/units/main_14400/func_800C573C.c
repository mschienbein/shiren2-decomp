#include "common.h"
/* Slot +0x2C is the per-instance saved-state stride (4 or 12 bytes).
 * The original dispatch forwards this, even though concrete size getters ignore it. */
typedef struct {
    unsigned char pad00[0x28]; short adjustment; unsigned short pad2A;
    s32 (*size)(void *object);
} Vtable;
typedef struct { void *state; unsigned char *saved; s32 depth; Vtable *vtable; } Rng;
extern void func_800C5684(Rng *object, void *saved);
void func_800C573C(void *object) {
    Rng *rng = object;
    if (rng->depth > 0) {
        s32 stride = rng->vtable->size((unsigned char *)object + rng->vtable->adjustment);
        s32 depth = rng->depth - 1;
        s32 offset = stride * depth;
        rng->depth = depth;
        func_800C5684(rng, rng->saved + offset);
    }
}
