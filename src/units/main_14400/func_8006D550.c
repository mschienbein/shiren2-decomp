#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;

typedef struct {
    u16 held;
    u16 h2;
    u16 h4;
    u16 h6;
    u16 h8;
    u16 hA;
    u16 dir;
    u16 dirRepeat;
    u16 h10;
    u16 h12;
    u16 h14;
    s8 stickX;
    s8 stickY;
    u8 mag;
} Input;
typedef s32 (*InputHandler)(Input *, Input *);
extern InputHandler D_8013D3F0[];
extern s32 D_8013D40C;
extern s32 D_8013D410;
extern u8 D_801A70E4;
extern u8 D_801A70E5;
extern s32 D_801A70E8;
extern u16 D_801A70F0;
extern u16 D_801A70F2;
extern u16 D_801A70F4;
extern u16 D_801A70F6;
extern u16 D_801A70F8;
extern u16 D_801A70FA;
extern s32 D_801A70FC;
u8 func_8006C540(void);
void func_800740A8(s32);
void func_8006D34C(void);
void func_80058CE4(u16 *, u16 *, u16 *, s8 *, s8 *);
s32 func_80042B5C(void);
void func_80058D50(void);
void func_8006D184(s32, s32);
void func_800418FC(s32 *, s32 *);
s32 func_80083920(void);
s32 func_80041FF8(void);
s32 func_80041E3C(void);
s32 func_80041E50(void);
s32 func_80041EB0(void);
u8 func_80041EC4(void);
s32 func_80062C64(s32, s32);
void func_8007C410(s32, s32);
void func_8007C324(void);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 func_8006D550(s32 mode) {
    Input in;
    Input saved;
    s32 posX;
    s32 posY;
    u8 elapsed;
    s8 y;
    u8 ax;
    u8 ay;
    u16 mask;
    s32 result;

    elapsed = func_8006C540();
    func_800740A8(0);
    if (mode != D_801A70FC) {
        func_8006D34C();
        D_801A70FC = mode;
    }
    func_80058CE4(&in.held, &in.h2, &in.h4, &in.stickX, &in.stickY);
    if (!func_80042B5C()) {
        in.stickX = 0;
        in.stickY = 0;
    }
    in.stickX = (ABS(in.stickX) < 40) ? 0 : in.stickX;
    y = (ABS(in.stickY) < 40) ? 0 : in.stickY;
    in.stickY = y;
    ay = ABS(y);
    ax = ABS(in.stickX);
    in.mag = (ay < ax) ? ax : ay;
    in.dir = 0;
    if (in.mag >= 8) {
        if (y > 0) {
            in.dir = 0x800;
        } else if (y < 0) {
            in.dir = 0x400;
        }
        if (in.stickX > 0) {
            in.dir |= 0x100;
        } else if (in.stickX < 0) {
            in.dir |= 0x200;
        }
        if (ax >= ay * 2) {
            in.dir &= 0xF3FF;
        } else if (ay >= ax * 2) {
            in.dir &= 0xFCFF;
        }
    }
    in.dirRepeat = 0;
    if (in.dir != 0) {
        if (D_801A70E8 != 0) {
            D_801A70E8 -= elapsed;
            if (D_801A70E8 < 0) {
                D_801A70E8 = 0;
            }
        } else {
            in.dirRepeat = in.dir;
            if (D_801A70F8 == 0) {
                D_801A70E8 = 12;
            } else {
                D_801A70E8 = 4;
            }
        }
    } else {
        D_801A70E8 = 0;
    }
    in.h6 = in.held & 0xF00;
    in.hA = in.h6;
    in.h8 = in.h4 & 0xF00;
    if (in.h6 != 0) {
        if (in.h6 & ~D_801A70F0) {
            D_801A70F0 = in.h6;
        } else {
            in.hA = D_801A70F0;
        }
    } else {
        D_801A70F0 = 0;
    }
    if (in.h6 != 0) {
        in.h10 = in.h6;
        in.h12 = in.h8;
        in.h14 = in.hA;
    } else {
        in.h10 = in.dir;
        in.h14 = in.dir;
        in.h12 = in.dirRepeat;
    }
    mask = (mode == 1) ? 0x90EF : 0xF0FF;
    if (!(in.held & mask)) D_801A70E4 = 0;
    if (in.h10 != D_801A70F2) D_801A70E5 = 0;
    __builtin_memcpy(&saved, &in, sizeof(Input));
    if (D_801A70E4) {
        in.held &= 0xF00;
        in.h2 &= 0xF00;
        in.h4 &= 0xF00;
    }
    if (D_801A70E5) {
        in.h6 = 0;
        in.h8 = 0;
        in.hA = 0;
        in.dir = 0;
        in.dirRepeat = 0;
        in.h10 = 0;
        in.h12 = 0;
        in.h14 = 0;
        in.stickX = 0;
        in.stickY = 0;
        in.mag = 0;
        in.held &= 0xF0FF;
        in.h2 &= 0xF0FF;
        in.h4 &= 0xF0FF;
    }
    result = D_8013D3F0[mode](&in, &saved);
    D_801A70F6 = saved.h6;
    D_801A70F8 = saved.dir;
    D_801A70FA = saved.mag;
    D_801A70F2 = saved.h10;
    D_801A70F4 = saved.h14;
    func_80058D50();
    func_8006D184(2, 0);
    func_800418FC(&posX, &posY);
    if (result == -1 && !(u8)func_80083920() && !(u8)func_80041FF8() && (u8)func_80041E3C() == 1 && !(u8)func_80041EB0() &&
        !func_80041EC4() && !func_80062C64(posX, posY)) {
        if (++D_8013D40C == 60) {
            func_8007C410(0x1B, 1);
            func_8007C410(0x17, 1);
        }
    } else {
        D_8013D40C = 0;
    }
    if (result == -1 && !(u8)func_80041FF8() && !(u8)func_80041E50() && !(u8)func_80041EB0() && !func_80041EC4()) {
        if (++D_8013D410 >= 4 && !(D_8013D410 & 1)) {
            func_8007C324();
        }
    } else {
        D_8013D410 = 0;
    }
    func_8006D184(2, 1);
    return result;
}
