#include "common.h"

typedef struct { char pad0[0xC]; short x,y,z; } Actor;
typedef struct { char pad0[4]; unsigned short field4; char pad6[2]; unsigned short state; char padA[8]; unsigned short flags12; s32 field14; char pad18[4]; s32 timer; char pad20[4]; s32 field24; char pad28[0x34]; s32 field5C; char pad60[8]; s32 field68; } Object;
extern Actor *func_8007946C(s32,s32);
extern void func_8005A234(void),func_80061820(s32,s32);
void func_80088E70(Object *obj) { Actor *a=func_8007946C(0,obj->field14); Actor *b=func_8007946C(0,obj->field24); switch(obj->state) { case 0: obj->timer=8; obj->state++; break; case 1: if(!obj->timer--) { s32 x=a->x,y=a->y,z=a->z; a->x=b->x; a->y=b->y; a->z=b->z; b->x=x; b->y=y; b->z=z; if(obj->flags12 & 0x4000) { func_8005A234(); func_80061820(obj->field5C,obj->field68); } obj->timer=4; obj->state++; } break; case 2: if(!obj->timer--) obj->field4=4; break; } }
