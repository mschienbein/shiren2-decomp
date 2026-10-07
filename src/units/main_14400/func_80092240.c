#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad[0x20]; char *f_20; } S;
void func_80048764(S *s);
void func_80048870(S *s, s32 v);
void func_800487EC(S *s, s32 a, s32 b, char *c);
void func_80092240(S *s) {
    func_80048764(s);
    func_80048870(s, 0x78000000);
    func_800487EC(s, 0, 0, s->f_20);
}
