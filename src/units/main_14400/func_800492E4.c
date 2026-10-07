#include "common.h"
typedef unsigned char u8;
extern u8 D_8013960A;
u32 func_80032D70(void *);
s32 func_800491C0(void *);
void func_80049110(void);
void func_800492E4(void *self) {
    if (D_8013960A != 0 && func_80032D70(self) < 0xFF && func_80032D70(self) < 0x50) {
        while (1) {
            s32 failed = func_800491C0(self) != 1;
            if (!failed) {
                break;
            }
            func_80049110();
        }
    }
}
