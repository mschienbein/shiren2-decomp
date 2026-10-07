#include "common.h"

typedef unsigned char u8;

extern u8 D_80160B18[];
extern u8 D_00194FC0[];
extern u8 D_20015A8[];
extern void func_8006AC30(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5);

/* The object argument is not read; the single record is always loaded. */
void *func_80044E7C(void *unusedObject) {
    func_8006AC30(D_80160B18, D_00194FC0, D_20015A8, 8, 0, 1);
    return D_80160B18;
}
