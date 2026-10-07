#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

extern u8 D_801541C4[];
extern u8 D_801C9BD8[];
extern u8 D_80142920[];
extern u8 D_80142930[];
extern u8 D_80138C40[];
void func_800CA4A4(void *a, void *b);
void func_800D1C34(void *a, void *b);
void func_800B02A0(void *a);
void func_801EF52C(void *a);
void func_80112DE4(void *a);
void func_800B0D18(void *a);
void func_800D831C(void *a);
void func_801227D4(void *a);
void func_800A176C(void *a, void *b);
void func_800A1A04(void *a, void *b);
void func_80046424(void *a, void *b);
void func_800CA2C0(void *a);
void func_800C9A54(void *a) {
    func_800CA4A4(a, D_801541C4);
    func_800D1C34(D_801C9BD8, a);
    func_800B02A0(a);
    func_801EF52C(a);
    func_80112DE4(a);
    func_800B0D18(a);
    func_800D831C(a);
    func_801227D4(a);
    func_800A176C(D_80142920, a);
    func_800A1A04(D_80142930, a);
    func_80046424(D_80138C40, a);
    func_800CA2C0(a);
}
