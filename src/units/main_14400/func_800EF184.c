#include "common.h"
typedef struct { unsigned char field_0[0x58]; void *field_58; } Object;
extern unsigned char func_800A6420(Object *, void *);
extern void *func_800A492C(Object *, s32, s32, s32);
extern s32 func_800E776C(Object *, unsigned char);
extern s32 func_800E7104(Object *);
/* The byte mode is passed by callers but unused by this override. */
s32 func_800EF184(Object *arg, unsigned char mode) { s32 result; void *target = arg->field_58; if (target && func_800A6420(arg, target) == 3) target = 0; if (!target) { target = func_800A492C(arg, 2, 1, 1); arg->field_58 = target; } if (!target) result = func_800E776C(arg, 3); else result = func_800E7104(arg); return result; }
