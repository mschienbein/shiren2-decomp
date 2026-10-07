#include "common.h"
typedef unsigned char u8;
extern s32 D_801C9F1C[];
void func_800EC68C(void *record, u32 value);
void func_800CC288(void *);
u8 func_800A9958(void);
void func_800CC934(u8, void *);
void func_800CCF20(void *);
s32 func_800ECA38(void *record, u32 value, s32 flag) {
    func_800EC68C(record, value);
    func_800CC288(D_801C9F1C);
    func_800CC934(func_800A9958(), D_801C9F1C);
    if (flag) {
        func_800CCF20(D_801C9F1C);
    }
    return 1;
}
