#include "common.h"
unsigned char func_8006C540(void);
s32 func_8006D550(s32 mode);
void func_8006E6D8(void);
void func_8006E74C(void);
s32 func_8006E758(void);
s32 func_8006D44C(s32 mode, s32 time) {
    s32 value;
    s32 tick = func_8006C540();
    if (time < 0) {
        do {
            if (!func_8006E758()) func_8006E6D8();
            value = func_8006D550(mode);
            func_8006E74C();
        } while (value == -1);
        return value;
    }
    time /= 1000;
    time = (time * 60) / 1000;
    do {
        if (!func_8006E758()) { time -= tick; func_8006E6D8(); }
        value = func_8006D550(mode);
        func_8006E74C();
    } while (value == -1 && time > 0);
    return value;
}
