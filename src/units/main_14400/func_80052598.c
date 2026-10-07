#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct {
    s32 handle;
    u8 unk4;
} State;
extern u8 D_8016166E;
extern State D_80161648;
extern u8 D_8014B894[];
void func_80051D14(s16);
s32 func_8012A534(s32 id, s32 value);
void func_8005312C(s32, u8);

void func_80052598(s16 index, u8 arg)
{
    if (D_8016166E == 0) {
        func_80051D14(index);
        func_8012A534(D_80161648.handle, 0x1E);
        {
            s32 sound = D_8014B894[index];

            D_80161648.unk4 = 0x1E;
            func_8005312C(sound, arg);
        }
    }
}
