#include "common.h"
typedef struct { s32 x; s32 y; } Pos;
typedef struct { char pad[0x80]; short field_80; void (*field_84)(void*,Pos*); } Methods;
typedef struct { char pad[0x3C]; Pos field_3c; char pad44[8]; Methods *field_4c; } Obj;
extern void func_80046E7C(Obj*);
void func_80095D64(Obj *p) { func_80046E7C(p); p->field_4c->field_84((char*)p+p->field_4c->field_80,&p->field_3c); }
