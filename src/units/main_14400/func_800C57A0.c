#include "common.h"
/* Concrete slot +0xC targets are 800C5CA4 and 800C5E10: integer RNG steps. */
typedef struct {
    unsigned char pad00[8];
    short adjust08;
    unsigned short pad0A;
    s32 (*next)(void *object);
} RngVtable;
typedef struct { void *state; void *saved; s32 depth; RngVtable *vtable; } Rng;
unsigned char func_800C57A0(void *object) {
    Rng *rng = object;
    return rng->vtable->next((unsigned char *)object + rng->vtable->adjust08);
}
