#include "common.h"
/* Contained collection/item link at +8; func_800DA9DC reads its item pointer at +4. */
typedef struct { void *collection; void *item; } Link;
typedef struct { unsigned char pad0[8]; Link field_8; } Object;
void func_800DA9DC(unsigned char *out, Link *link);
void func_800DADE8(Object *self, unsigned char *out) { func_800DA9DC(out, &self->field_8); }
