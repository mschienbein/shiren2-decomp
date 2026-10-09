#include "common.h"
extern const char D_8014C7F0[], D_8014C7FC[], D_8014C808[], D_8014C814[];
extern void *D_801A7150[2], *D_801A715C[2];
extern s32 D_8013D454;
extern void *func_8006A8D8(char *, u32);
extern void func_80071088(void);
void func_8006E7E0(void) { D_801A7150[0] = func_8006A8D8((char *)D_8014C7F0, 0x5DC0); D_801A7150[1] = func_8006A8D8((char *)D_8014C7FC, 0x5DC0); D_801A715C[0] = func_8006A8D8((char *)D_8014C808, 0x6400); D_801A715C[1] = func_8006A8D8((char *)D_8014C814, 0x6400); func_80071088(); if (D_801A7150[0] && D_801A7150[1] && D_801A715C[0] && D_801A715C[1]) D_8013D454 = 1; }
