#include "common.h"

typedef unsigned char u8;

extern u8 D_80160B10[];
extern u8 D_00194FC0[];
extern u8 D_2000F78[];
void func_8006AC30(void *, void *, void *, s32, s32, s32);
/* The object argument is not read; frame selects the 1-based record. */
void *func_80044E24(void *unusedObject, u8 frame) { func_8006AC30(D_80160B10, D_00194FC0, D_2000F78, 8, frame - 1, 1); return D_80160B10; }
