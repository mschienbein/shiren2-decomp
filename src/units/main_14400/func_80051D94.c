#include "common.h"
typedef struct { signed char x,y; } Pair;
typedef struct { short field_0; s32 field_4; } State;
typedef struct { s32 field_0; Pair field_4; } StateTail;
extern State D_80161650;
extern StateTail D_80161654;
extern unsigned char D_8016166C;
extern void *D_801D4D04;
extern char D_8014B844[];
extern void func_8005239C(void),func_800533DC(void),func_8012A58C(s32,unsigned char);
extern void func_80052914(s32 index, void *dst, void *ranges);
extern s32 func_8012A534(s32 id, s32 value);
extern s32 func_80052AD8(void *resource),func_80129EE0(void *resource);
void func_80051D94(short type,Pair position) { void *saved=D_801D4D04; if(D_8016166C) { D_80161650.field_0=type; func_8005239C(); func_800533DC(); func_80052914((short)(type-0x54),D_801D4D04,D_8014B844); if((unsigned short)(type-0x56)<2U) D_80161650.field_4=func_80052AD8(saved); else D_80161650.field_4=func_80129EE0(saved); func_8012A534(D_80161654.field_0,(unsigned char)position.x); func_8012A58C(D_80161654.field_0,position.y); D_80161654.field_4=position; } }
