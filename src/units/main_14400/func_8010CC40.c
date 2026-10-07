#include "common.h"
extern s32 D_80153AA0[];
typedef struct { char pad[8]; s32 *field_8; } Obj;
extern void func_800AC68C(Obj *);
void func_8010CC40(Obj *a, s32 b) { a->field_8 = D_80153AA0; if (b & 1) func_800AC68C(a); }
