#include "common.h"
extern unsigned char D_80161B44;
extern void func_8005C870(unsigned char value);
extern void func_80053D70(s32,s32,s32,s32,s32,s32,char *,void *);
void func_800548B4(char *fmt,void *args) { D_80161B44=3; func_8005C870(3); func_80053D70(0,0,4,4,0x20,0x14,fmt,args); }
