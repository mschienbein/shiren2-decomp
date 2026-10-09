#include "common.h"
typedef struct { unsigned short field0; const void *field4; } Object;
extern const s32 D_80157FA8[];
extern const unsigned char D_80158A18[48];
Object *func_800DEEE8(Object *p, unsigned char *unused_payload) { p->field4 = &D_80157FA8; p->field0 = 0x2D; p->field4 = D_80158A18; return p; }
