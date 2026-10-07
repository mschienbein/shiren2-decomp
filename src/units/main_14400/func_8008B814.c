#include "common.h"
typedef struct { char pad0[4]; short field_4; char pad6[8]; short field_e; char pad10[0x14]; s32 field_24; } Obj;
extern s32 D_801E4E70;
extern void func_8005CB70(s32),func_80061D28(s32,s32,s32,s32);
extern s32 func_80042A50(void);
void func_8008B814(Obj *p) { if(p->field_24) { func_8005CB70(1); func_80061D28(10,10,0x41,0x2B); D_801E4E70=0; } else { func_8005CB70(0); func_80061D28(10,10,0x41,0x2B); D_801E4E70=func_80042A50(); } p->field_e=1; p->field_4=4; }
