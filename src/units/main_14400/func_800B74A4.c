#include "common.h"
typedef struct { unsigned char field0; char pad1[3]; u32 field4; void *field8; } Obj;
extern unsigned char D_80153B40[];
Obj *func_800B74A4(Obj *obj, unsigned char kind, u32 value) { obj->field8 = D_80153B40; obj->field0 = kind; obj->field4 = value; return obj; }
