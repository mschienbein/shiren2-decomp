#include "common.h"
typedef unsigned char u8;
typedef struct { unsigned char field_0[0x24]; void *field_24; } Object;
extern s32 D_8015AC40[];
extern void *func_800EFC70(void *,s32,u8);
extern void func_800E4D88(Object *,s32);
extern void func_800E4D90(Object *,s32);
Object *func_800FEE00(Object *arg,unsigned char value) { func_800EFC70(arg,0x30,value); arg->field_24=D_8015AC40; func_800E4D88(arg,4); func_800E4D90(arg,3); return arg; }
