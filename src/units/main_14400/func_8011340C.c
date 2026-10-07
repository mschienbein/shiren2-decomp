#include "common.h"
typedef struct { char pad[0x28]; short field_28; void (*field_2c)(void*,s32,void*); } Methods;
typedef struct { char pad[0x18]; Methods *field_18; } Obj;
extern char D_8015D724[];
extern void func_800AF174(void*,Obj*);
extern void func_800CA4E8(Obj*,void*);
void func_8011340C(char *p,Obj *arg) { func_800AF174(p,arg); func_800CA4E8(arg,D_8015D724); arg->field_18->field_2c((char*)arg+arg->field_18->field_28,1,p+0xC); }
