#include "common.h"

extern unsigned char D_8015D114[];
extern s32 func_8010B920(void *, unsigned char, void *);
/* Effect-query slot +0x44 accepts a full-width id and returns a byte. */
unsigned char func_8010DCEC(void *state, s32 value) { return (unsigned char)func_8010B920(state, (unsigned char)value, D_8015D114); }
