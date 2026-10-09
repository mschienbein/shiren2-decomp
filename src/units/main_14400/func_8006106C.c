#include "common.h"
typedef unsigned char u8;
typedef struct { u32 word0, word1; } Gfx;
extern void func_800612D4(s32);
extern void func_80054DE0(s32 x);
extern void func_80055068(Gfx *(*callback)(Gfx *));
extern Gfx *func_80064444(Gfx *);
extern u8 func_8006C508(u8 value);
void func_8006106C(void) {
    func_800612D4(0);
    func_80054DE0(2);
    func_80055068(func_80064444);
    func_8006C508(2);
}
