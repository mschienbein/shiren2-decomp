#include "common.h"
typedef struct { char pad[0x48]; short field_48; s32 (*field_4c)(void*); } Methods;
typedef struct { s32 field_0, field_4, field_8; Methods *field_c; } Obj;
s32 func_800919CC(Obj *p) { Methods *m=p->field_c; p->field_8=1; return m->field_4c((char*)p+m->field_48); }
