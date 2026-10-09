#include "common.h"
typedef unsigned char u8;
typedef struct Owner Owner;
extern void *D_801476B8;
extern u8 D_80148644[4];
extern Owner D_801C9BD8;
extern void func_80046C30(s32);
extern void func_801EF6C4(void);
extern void func_801EF748(void);
extern void func_80046270(void);
extern void func_80045650(void);
extern void func_800AF83C(void);
extern void func_800A8740(void);
extern s32 func_80049CB4(s32, ...);
extern void func_800B1080(void);
extern void func_800B1130(void);
extern void func_800EB2F4(void *);
extern void func_800EF700(void);
extern void func_800AC118(void);
extern void func_800D7710(void);
extern void func_801224A0(void);
extern void func_800D1830(Owner *);
extern void func_800A94F0(void);
extern void func_800485C0(void);
extern void func_80041418(void);

void func_800C6120(void)
{
    u8 *flag;
    s32 remaining;
    func_80046C30(2);
    func_801EF6C4();
    func_801EF748();
    func_80046270();
    func_80045650();
    func_800AF83C();
    func_800A8740();
    func_80049CB4(1);
    func_80049CB4(8);
    func_80049CB4(10);
    func_800B1080();
    func_800B1130();
    func_800EB2F4(D_801476B8);
    func_800EF700();
    func_800AC118();
    flag = D_80148644;
    remaining = 3;
    do {
        *flag++ = 0;
    } while (remaining-- > 0);
    func_800D7710();
    func_801224A0();
    func_800D1830(&D_801C9BD8);
    func_800A94F0();
    func_800485C0();
    func_80041418();
    func_80049CB4(2);
}
