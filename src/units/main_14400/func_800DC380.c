#include "common.h"
typedef struct { s32 field_0; void *field_4; } Obj;
extern char D_801585C8[];
extern void *func_800DA904(void *obj, s32 kind, unsigned char *params);
Obj *func_800DC380(Obj *p,unsigned char *a) { func_800DA904(p,0x17,a); p->field_4=D_801585C8; return p; }
