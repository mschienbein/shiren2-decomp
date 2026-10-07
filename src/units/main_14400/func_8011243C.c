#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern u8 D_80153AA0[];
extern void func_800AC68C(void *);
typedef struct { u8 pad[8]; void *x8; } S;
void func_8011243C(S *p, s32 flags){ p->x8 = D_80153AA0; if (flags & 1) func_800AC68C(p);}
