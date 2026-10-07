#include "common.h"

typedef unsigned char u8;

typedef struct {
    char pad0[0x14];
    s32 unk14;
} State800CC8A4;

extern char D_80147EA8[];
extern State800CC8A4 *D_80147F44;
extern s32 func_800CBD2C(void);
extern void func_800CBEBC(u8 id, u8 value);
extern void func_800CBF00(void *arg0);
extern s32 func_800CC6E4(void *arg0);
extern void func_800CC248(void);

s32 func_800CC8A4(u8 id) {
    s32 result;

    if (id >= 6) {
        return 0;
    }
    result = 0;
    if (func_800CBD2C() == 3) {
        func_800CBEBC(id, result);
        func_800CBF00(D_80147EA8);
        if (D_80147F44->unk14 == 0) {
            result = func_800CC6E4(D_80147EA8) == 0;
        }
    }
    func_800CC248();
    return result;
}
