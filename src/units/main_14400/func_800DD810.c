#include "common.h"
typedef struct { s32 field_0[3]; unsigned char *field_C; } Object;
extern s32 func_800DADCC(Object *);
s32 func_800DD810(Object *self) { s32 result = func_800DADCC(self) ^ 1; if (result) return 0; return self->field_C[1] == 0xAC; }
