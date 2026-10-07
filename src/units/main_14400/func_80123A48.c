#include "common.h"
typedef struct { s32 field_0[2]; void *field_8; } Object;
extern s32 D_80153AA0[];
extern void func_800AC68C(Object *);
void func_80123A48(Object *arg, s32 flags) { arg->field_8=D_80153AA0; if(flags & 1) func_800AC68C(arg); }
