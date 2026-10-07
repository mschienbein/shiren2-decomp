#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad0[0x24]; void *field_24; char pad28[0x64]; void *field_8c; char pad90[0x10]; char field_a0[8]; char field_a8[0x18]; unsigned char field_c0; } Obj;
extern char D_8015B5C8[];
extern void *func_800EFC70(Obj*,s32,u8);
extern void *func_800CEC90(void*,Obj*,void*,u8,u16);
extern void func_80101750(Obj*);
Obj *func_801016E0(Obj *p,s32 a) { func_800EFC70(p,0x3D,a); p->field_24=D_8015B5C8; func_800CEC90(p->field_a8,p,p->field_a0,8,0); p->field_c0=0; p->field_8c=p->field_a8; func_80101750(p); return p; }
