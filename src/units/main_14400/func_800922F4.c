#include "common.h"

/* Pointer-to-member pair passed by value (see func_80092160). */
typedef struct { short delta; short index; void *fn; } Pair;
typedef struct S S;

typedef struct {
    s32 unk0;
    S *owner;
    Pair callback;
} Binding800922F4;

void func_800922F4(Binding800922F4 *binding, S *owner, Pair callback) {
    binding->owner = owner;
    binding->callback = callback;
}
