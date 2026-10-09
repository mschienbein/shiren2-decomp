#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u16 held, pressed, field4, field6, field8, fieldA, fieldC, fieldE, field10, field12, field14; } Input;
extern s32 D_801A70EC;
extern u8 D_801A70E6, D_801A70E5;
extern u16 D_801A70F4, D_801A70F6, D_801A70F8;
void func_8007409C(s32 value);
s32 func_80041ACC(void);
u8 func_8006C540(void);
void func_80074078(s32 value);
s32 func_8006DF78(u16 input, u16 repeat, s32 mode);
void func_80074090(s32 value);
s32 func_80041FF8(void);
s32 func_80041E7C(void);
s32 func_8006DC68(Input *input, Input *other) {
    u8 tapped = 0;
    u16 mask, repeat;
    s32 mode, result;
    func_8007409C(0);
    if (func_80041ACC() > 0) { repeat = other->held; mask = input->held; }
    else { repeat = other->pressed; mask = input->pressed; }
    if (input->held & 0x4000) {
        if (D_801A70EC >= 0) {
            D_801A70EC += func_8006C540();
            if (D_801A70EC > 200) D_801A70EC = 200;
            if ((input->held & 0xBFFF) || input->field10) D_801A70EC = -1;
        }
    } else {
        if ((u32)(D_801A70EC - 1) < 30) tapped = 1;
        D_801A70EC = 0;
    }
    if (D_801A70E6) {
        if (!(input->held & 0xE02F)) {
            func_80074078(1);
            return func_8006DF78(input->field14 | (other->held & 0x10), D_801A70F4, 1);
        }
        D_801A70E6 = 0;
        func_80074078(0);
        func_80074090(0);
        tapped = 0;
        D_801A70EC = -1;
    }
    if ((other->held & 0x4000) && (repeat & 0x8000)) return 7;
    if (!(u8)func_80041FF8() && tapped) {
        D_801A70E6 = 1;
        func_80074090(1);
        return 10;
    }
    if (mask & 0x8000) { D_801A70E5 = 0; return 6; }
    if (mask & 0x20) return 9;
    if (input->pressed & 0xF) return 8;
    mode = 0;
    mask = mode;
    repeat = mode;
    if (input->fieldC) {
        mask = input->fieldC;
        repeat = D_801A70F8;
        if (other->held & 0x2000) mode = (other->held & 0x4000) ? 4 : 2;
        else { mode = 5; if (other->held & 0x4000) mode = 6; }
    } else if (input->field6) {
        mask = input->field6;
        repeat = D_801A70F6;
        if (other->held & 0x2000) { mode = 5; if (other->held & 0x4000) mode = 6; }
        else mode = (other->held & 0x4000) ? 4 : 2;
    }
    if (mode) {
        if (mode == 4 && !func_80041E7C()) mode = 3;
        if ((u32)(mode - 2) < 2) repeat = 0;
        mask |= other->held & 0x10;
        result = func_8006DF78(mask, repeat, mode);
        if (result != -1) return result;
    }
    if (other->held & 0x10) func_8007409C(1);
    return -1;
}
