#include "common.h"
typedef struct { char pad[0xE]; unsigned char field_e; } Obj;
s32 func_8010BCB0(Obj *p,signed char delta) { if(delta>0 && p->field_e>=16) return 0; if(delta<0 && p->field_e==0) return 0; p->field_e+=delta; return 1; }
