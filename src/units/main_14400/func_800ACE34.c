#include "common.h"

typedef unsigned char u8;

extern void func_800AD188(u8 id, u8 *out0, u8 *out1);
extern void func_800AD308(u8 id);
extern void func_800ACF34(u8 *data);
extern s32 func_800AD468(u32 id);
extern void func_800AD3F4(u8 id);

void func_800ACE34(u8 *data) {
    u8 out[2];
    s32 missing;

    func_800AD188(data[0], &out[0], &out[1]);
    func_800AD308(data[1]);
    if (out[1] != 0) {
        func_800ACF34(data);
    }
    missing = 0;
    if (data[2] & 2) {
        missing = func_800AD468(data[1]) == 0;
    }
    if (missing) {
        func_800AD3F4(data[1]);
    }
}
