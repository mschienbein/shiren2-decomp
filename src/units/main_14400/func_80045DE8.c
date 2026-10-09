#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

extern s32 D_80138BDC;
extern s32 func_8005275C(void);
extern void func_80045A60(void);
extern void func_800528AC(void);
extern void func_800458BC(s32 arg);

void func_80045DE8(void) {
    s32 id = (s16)func_8005275C();

    func_80045A60();
    func_800528AC();
    if (id != D_80138BDC) {
        func_800458BC(-1);
        func_800458BC(id);
    }
}
