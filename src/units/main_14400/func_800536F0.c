#include "common.h"
typedef struct { s32 field_0,field_4; unsigned char field_8; } State;
typedef struct { State *field_0; short field_4; unsigned char field_6,field_7,field_8,field_9; } Obj;
extern s32 func_8012A434(s32 id, s32 arg1);
extern s32 func_8012A534(s32 id, s32 value);
extern void func_80053590(Obj*);
/* The setter stores the low halfword of the full-word value itself. */
void func_800536F0(Obj *p) { s32 value=p->field_0->field_8; if(value<=0) { func_8012A434(p->field_0->field_4,1); func_80053590(p); p->field_0->field_8=0; } else if(p->field_9>=p->field_6) { value=p->field_7; func_80053590(p); if(value) func_8012A534(p->field_0->field_4,value); else func_8012A434(p->field_0->field_4,1); p->field_0->field_8=value; } else { value=p->field_8+p->field_4*p->field_9/p->field_6; func_8012A534(p->field_0->field_4,value); p->field_0->field_8=value; p->field_9++; } }
