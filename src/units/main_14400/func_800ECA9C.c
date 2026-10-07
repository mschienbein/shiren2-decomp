#include "common.h"
typedef struct { unsigned char key; unsigned char value; } Pair800ECA9C;
extern Pair800ECA9C D_80158F60[];
extern s32 D_801C9FB8;
unsigned char func_800A9958(void);
void func_800EC68C(void *record, u32 value);
void func_800CC288(void *state);
void func_800CC934(unsigned char key, void *state);
void func_800CCF20(void *state);
s32 func_800ECA9C(void *obj, s32 flag) {
    Pair800ECA9C *p;
    Pair800ECA9C *next;
    s32 end = 0x15;
    next = D_80158F60;
    while (1) {
        s32 c;
        p = next;
        next = p + 1;
        c = func_800A9958();
        if (p->key == c) break;
        if (p->key == end) return 0;
    }
    func_800EC68C(obj, p->value);
    func_800CC288(&D_801C9FB8);
    func_800CC934(func_800A9958(), &D_801C9FB8);
    if (flag != 0) func_800CCF20(&D_801C9FB8);
    return 1;
}
