#include "common.h"
typedef struct { s32 field_0; const void *field_4; } Object;
extern const unsigned char D_80158808[48];
extern void *func_800DA904(Object *, s32, unsigned char *);
Object *func_800DD518(Object *self, unsigned char *params) { func_800DA904(self, 0x23, params); self->field_4 = D_80158808; return self; }
