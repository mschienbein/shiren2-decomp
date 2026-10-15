/* OW1 slice s05 (overlay_136dc0, 0x801E71E0..0x801E8394) object views.
 * Started from compiler-id/ovl_types.h. Every field lists the ROM accesses
 * (VRAM of the instruction) that fix its offset and width. Objects are named by
 * address; widths only, meanings unproven unless stated. */
#ifndef OW1_S05_TYPES_H
#define OW1_S05_TYPES_H
#include "common.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef float f32;
typedef double f64;

/* 0xB0-byte unit record returned by main func_8007946C (canonical Unit, 0xB0).
 * Only the fields this slice touches. */
typedef struct Unit {
    u8 pad0[0x7];
    u8 x7;        /* 0x07 sb: 801E74B0 801E7600 801E7530 801E78E8 */
    u8 pad8[0x4];
    /* 0x0C/0x0E/0x10 are signed halves (canonical Sprite8008A474 has s16 x/z here):
     * 801E73B0..73CC stores the s8 value -1 with addiu -1 (u16 would emit li 0xFFFF) and
     * 801E739C/73AC store trunc.w.s directly (u16 would add the unsigned fixup);
     * 801E7CD8..7CE4 read (s16) >> 2 as lhu/sll 16/sra 18. */
    s16 xC;       /* 0x0C lhu/sh: 801E7234 801E739C 801E7548/7558 801E767C/76A0 801E7774 801E780C 801E7CD8 */
    s16 xE;       /* 0x0E lhu/sh: 801E7250 801E73AC 801E78EC/78F8 801E7D00 */
    s16 x10;      /* 0x10 lhu/sh: 801E7248 801E73CC 801E7580/7590 801E7684/76A8 801E7824 801E7D2C */
    u8 x12;       /* 0x12 sb: 801E73D8 */
    u8 x13;       /* 0x13 sb: 801E73DC */
    u8 pad14[0x8];
    f32 x1C;      /* 0x1C swc1: 801E73E0 */
    f32 x20;      /* 0x20 swc1: 801E73E4 */
    u8 pad24[0x10];
    s8 x34;       /* 0x34 sb (stores -1/1): 801E73B4 801E73C4 */
    u8 x35;       /* 0x35 sb: 801E73E8 */
    u8 pad36;
    u8 x37;       /* 0x37 sb: 801E73D4 */
    s32 x38;      /* 0x38 sw: 801E73EC */
    u8 pad3C[0x2];
    u8 x3E;       /* 0x3E sb: 801E738C */
    u8 pad3F[0xB];
    s16 x4A;      /* 0x4A lh (table index): 801E7534 801E755C */
    u8 pad4C[0xB0 - 0x4C];
} Unit;

/* B BSS 0x801F6764: 30 records of 0x3E bytes (ends 0x801F6EA8).
 * Stride: sll5/subu/sll1 = 62 (801E71E0, 801E72DC, 801E7630); loops of 30 from
 * the end (801E71B8 count 0x1D, 801E7414, 801E74C0, 801E76CC, 801E7850, 801E7878);
 * bzero(&D_801F6764_148684[i], 0x3E) at 801E7A7C. */
typedef struct Rec801F6764 {
    u8 pad0[0x8];
    u8 x8;        /* 0x08 sb/lbu: 801E7670 801E7754 801E77C8 */
    u8 x9;        /* 0x09 sb/lbu: 801E7674 801E7764 801E77E0 */
    u8 xA;        /* 0x0A sb/lbu: 801E76AC 801E7690 801E76F0 801E7714 801E786C */
    u8 padB;
    u16 xC;       /* 0x0C sh/lhu (u16 counter, andi 0xFFFF): 801E7678 801E7710/771C */
    u16 xE;       /* 0x0E sh/lhu: 801E7680 801E769C 801E7768 801E77F0 */
    u16 x10;      /* 0x10 sh/lhu: 801E768C 801E76A4 801E7778 801E7810 */
    s8 x12;       /* 0x12 lb (signed): 801E71F4; sb 801E71D4 */
    u8 pad13;
    u8 x14;       /* 0x14 lbu/sb: 801E7300 801E731C 801E733C 801E743C 801E7454 801E7A50 */
    u8 pad15;
    u8 x16;       /* 0x16 sb/lbu: 801E74A0 801E74F8 801E7528 801E759C */
    u8 x17;       /* 0x17 sb/lbu: 801E75F0 801E78B0 801E78E0 801E7904 */
    u8 pad18[0x3E - 0x18];
} Rec801F6764;

/* B BSS 0x801F6EAA: 12-byte records indexed by func_801E6C44_138B64's result
 * (stride sll1/addu/sll2 = 12, 801E7D60..7D74). Signedness of x6/x8/xA is
 * unproven (stores only). */
typedef struct Rec801F6EAA {
    u8 pad0;
    u8 x1;        /* 0x01 sb: 801E7D7C */
    u8 pad2[0x2];
    u8 x4;        /* 0x04 sb: 801E7D80 */
    u8 pad5;
    s16 x6;       /* 0x06 sh: 801E7D94 */
    s16 x8;       /* 0x08 sh: 801E7DA8 */
    s16 xA;       /* 0x0A sh: 801E7DBC */
} Rec801F6EAA;

/* B BSS 0x801F6540: six u8 (801E7FD0 stores 0..5; 801E7FF8 reads them). */
typedef struct Obj801F6540 {
    u8 x0;        /* sb 801E7FDC, lbu/sb 801E802C 801E8038 */
    u8 x1;        /* sb 801E7FE0, lbu/sb 801E8020 801E8148 801E8150 */
    u8 x2;        /* sb 801E7FEC */
    u8 x3;        /* sb 801E7FE8, lbu 801E8070 801E8084 801E809C */
    u8 x4;        /* sb 801E7FE4, lbu 801E8040 801E8054 801E806C */
    u8 x5;        /* sb 801E7FF4, lbu 801E80A0 801E80B4 801E80F0 */
} Obj801F6540;

/* B BSS 0x801F71F0: 14 bytes (bzero(0x801F71F0, 0xE) per OVL1 REPORT). */
typedef struct Obj801F71F0 {
    u16 x0[4];    /* sh 801E8334 (copied from arg0[0..3], descending) */
    u8 x8;        /* sb 801E8360 */
    u8 x9;        /* sb 801E8368 */
    u8 xA;        /* sb 801E836C */
    u8 xB;        /* sb 801E8374 */
    u8 xC;        /* sb 801E8384 801E8388 */
    u8 xD;        /* sb 801E837C 801E8390 */
} Obj801F71F0;

/* func_8005DB10 result (main returns &D_80165960 as u8 *): bytes the switch in
 * func_801E7E48_139D68 touches. */
typedef struct Obj80165960 {
    s16 x0;       /* sh 801E7EE0, lh 801E7EF8 */
    s16 x2;       /* sh 801E7EE4, lh 801E7EFC */
    s16 x4;       /* sh 801E7ED0, lh 801E7EF0 */
    s16 x6;       /* sh 801E7ED4, lh 801E7EF4 */
    u8 pad8[0x6];
    u8 xE;        /* sb 801E7F50 801E7F60 */
    u8 xF;        /* sb 801E7F48 */
    u8 x10;       /* sb 801E7F78 801E7F88 */
    u8 x11;       /* sb 801E7F70 */
    u8 pad12[0x2];
    u8 x14;       /* lbu/sb flags 801E7E9C..801E7F90 */
    u8 pad15[0x3];
    u8 x18;       /* sb 801E7EE8 */
    u8 x19;       /* sb 801E7F00, lbu 801E7F0C */
    u8 x1A;       /* sb 801E7EDC */
    u8 pad1B[0x20 - 0x1B];  /* bzero(p, 0x20) at 801E7EC8 */
} Obj80165960;

#endif
