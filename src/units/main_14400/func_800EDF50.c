#include "common.h"
typedef struct { unsigned char field_0[0x104]; void *field_104; } Object;
typedef struct Values Values;
extern s32 func_800EAE8C(Object *, Values *);
s32 func_800EDF50(Object *self, Values *values) { if (self->field_104) return 0; return func_800EAE8C(self, values); }
