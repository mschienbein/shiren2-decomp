#include "common.h"
typedef unsigned char u8;
typedef struct { s32 field0; void *field4; } Object;
extern s32 D_80158868;
extern void *func_800DA904(Object *, s32, u8 *);
Object *func_800DD7D4(Object *p, u8 *params) { func_800DA904(p, 0x25, params); p->field4 = &D_80158868; return p; }
