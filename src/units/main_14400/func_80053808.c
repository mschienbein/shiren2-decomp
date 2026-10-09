#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 value;
    s32 step;
} Counter80053808;

extern Counter80053808 D_80161700;
extern Counter80053808 D_80161708;
extern s32 D_80161710;
u8 D_801398A0 = 0;
extern void func_80053100(void);
extern void func_800535A4(void);
extern void func_8005348C(void);
extern void func_80052E90(void);
extern void func_80129EB4(s32 channel, s32 value);

void func_80053808(void) {
    s32 value;

    value = D_80161700.value += D_80161700.step;
    D_80161708.value += D_80161708.step;
    if (D_801398A0 == 1) {
        if (value >= 0x4FFF) {
            D_801398A0 = 0;
            func_80053100();
            func_800535A4();
            return;
        }
    } else if (value <= 0 || D_80161710 <= 0) {
        D_801398A0 = 0;
        func_8005348C();
        func_80052E90();
        return;
    }
    func_80129EB4(2, D_80161700.value);
    func_80129EB4(1, D_80161708.value);
}
