#include "common.h"
typedef struct { s32 field0, field4; const void *field8; } Object;
extern const unsigned char D_801605B0[72];
extern Object *func_801128F0(Object *, s32);
Object *func_801273F4(Object *p) { func_801128F0(p, 0xEB); p->field8 = D_801605B0; return p; }
