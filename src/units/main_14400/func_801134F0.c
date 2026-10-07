#include "common.h"

typedef struct { char pad[8]; void *field8; } Object;
extern char D_80153AA0[];
extern void func_800AC68C(Object *);
void func_801134F0(Object *obj,s32 flags) { obj->field8=D_80153AA0; if (flags & 1) func_800AC68C(obj); }
