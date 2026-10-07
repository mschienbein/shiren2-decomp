#ifndef SHIREN2_ROW_VIEWS_H
#define SHIREN2_ROW_VIEWS_H

#include "common.h"

typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * One 0x2C-byte record of the 10-record table D_801A9080..D_801A9238
 * (func_80081C18 bounds its loop with D_801A9238 = D_801A9080 + 10 * 0x2C).
 * splat's halfword labels D_801A9082..D_801A909A and D_801A90A0 are the
 * fields of record 0 below, not separate objects. Every observed access is
 * an lhu/sh halfword; field_02 is nonzero while the record is in use.
 * Bytes 0x1C-0x1F and 0x22-0x2B have no access in these TUs.
 */
typedef struct Row_801A9080 {
    u16 field_00;
    u16 field_02;
    u16 field_04;
    u16 field_06;
    u16 field_08;
    u16 field_0A;
    u16 field_0C;
    u16 field_0E;
    u16 field_10;
    u16 field_12;
    u16 field_14;
    u16 field_16;
    u16 field_18;
    u16 field_1A;
    u8 unknown_1C[4];
    u16 field_20;
    u8 unknown_22[10];
} Row_801A9080;

extern Row_801A9080 D_801A9080[10];
extern Row_801A9080 *D_8013E81C;
extern s16 D_8013E820;
extern u16 D_8013E834;
extern u32 D_8013E838;
extern u16 D_801A9F58;
extern u16 D_801A9F5A;
extern u16 D_801A9F5C;
extern u16 D_801A9F5E;

/* Observed minimum argument/effect view; historical return type is unknown. */
extern void func_800827C8(s32 value);

#endif
