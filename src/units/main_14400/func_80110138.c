#include "common.h"

typedef unsigned char u8;
typedef struct { u8 f0; u8 f1; u8 pad2[0xB]; u8 fD; u8 pad[1]; u8 fF; u8 pad10[0x10]; u8 f20; u8 f21; } S;
void func_8010EAA8(S *);
void func_80110138(S *s, u8 arg1) {
    s->f1 = arg1;
    func_8010EAA8(s);
    s->f21 = 0;
    s->f20 = 0;
    s->fD = 0;
    s->fF = 0;
}
