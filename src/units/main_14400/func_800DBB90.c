#include "common.h"
extern s32 D_80158538[];
typedef struct { s32 field_0; s32 *field_4; } Obj;
extern void *func_800DA8A0(void *obj, s32 kind, void *src);
Obj *func_800DBB90(Obj *a, void *src) { func_800DA8A0(a, 0x14, src); a->field_4 = D_80158538; return a; }
