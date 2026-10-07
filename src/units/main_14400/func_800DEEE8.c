#include "common.h"
typedef struct { unsigned short field0; void *field4; } Object;
extern s32 D_80157FA8, D_80158A18;
Object *func_800DEEE8(Object *p) { p->field4 = &D_80157FA8; p->field0 = 0x2D; p->field4 = &D_80158A18; return p; }
