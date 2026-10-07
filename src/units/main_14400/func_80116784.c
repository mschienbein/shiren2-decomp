#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern u16 D_801486E8[]; extern u8 D_80148719; extern u8 D_80148718; extern s32 D_801486C0;
extern s32 func_80116928(u8); extern void func_80116508(u16*); extern void func_801169D0(void);
void func_80116784(void){
  for (;;) { u16 *p = &D_801486E8[D_80148719];
    if (func_80116928(0xE8)) { func_80116508(p); func_801169D0(); }
    if (D_80148719 == D_80148718) break;
    D_80148719++;
    if (D_80148719 >= 24) D_80148719 = 0; }
  D_801486C0 = 0; }
