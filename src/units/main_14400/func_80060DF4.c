#include "common.h"

typedef unsigned char u8;

typedef struct Gfx Gfx;

extern u8 D_801E4E48[];

s32 func_8008BE24(u32 size);
void func_800612D4(s32 mode);
void func_8006E7E0(void);
void func_800553B0(void);
void func_8005CB90(void);
void func_80074374(void);
void func_80054DE0(s32 x);
Gfx *func_80064444(Gfx *gfx);
void func_80055068(Gfx *(*callback)(Gfx *gfx));
s32 func_8006E908(void *pool, s32 count1, s32 count2, s32 count3);
u8 func_8006C508(u8 value);

void func_80060DF4(void) {
    func_8008BE24(0x19000);
    func_800612D4(0);
    func_8006E7E0();
    func_800553B0();
    func_8005CB90();
    func_80074374();
    func_80054DE0(2);
    func_80055068(func_80064444);
    func_8006E908(D_801E4E48, 0x1900, 0x1100, 0x180);
    func_8006C508(2);
}
