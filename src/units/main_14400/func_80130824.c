#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
extern u8 *D_80148D84;
void func_800326CC(void *);
void func_800326AC(void *, void *);
void func_80130824(void *a) { func_800326CC(a); func_800326AC(a, D_80148D84 + 0x14); }
