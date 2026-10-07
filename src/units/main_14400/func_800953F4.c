#include "common.h"
typedef struct { s32 field_0[3]; s32 field_C; void *field_10; unsigned char field_14[0x38]; void *field_4C; } Object;
extern unsigned char D_80151E38[], D_80151EC8[];
extern void func_8009543C(Object *, void *);
Object *func_800953F4(Object *arg, void *settings) { arg->field_4C = D_80151E38; arg->field_C = -1; arg->field_10 = D_80151EC8; func_8009543C(arg, settings); return arg; }
