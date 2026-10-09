#include "common.h"
typedef struct { s32 field0; s32 field4; void *field8; } Object;
extern unsigned char D_80153AA0[];
extern void func_800AC68C(Object *);
void func_80117050(Object *obj, s32 flags) { obj->field8 = D_80153AA0; if (flags & 1) func_800AC68C(obj); }
