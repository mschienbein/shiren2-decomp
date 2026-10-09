#include "common.h"
extern void *func_80044C60(void *obj, unsigned char frame);
/* Frame lookup is a pointer-returning virtual method. */
void *func_800EE060(void *self, unsigned char frame) { return func_80044C60(self, frame); }
