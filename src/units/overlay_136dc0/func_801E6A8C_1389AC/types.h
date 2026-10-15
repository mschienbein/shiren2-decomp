#ifndef OW1_S04_TYPES_H
#define OW1_S04_TYPES_H
#include "common.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* Overlay B only. Instruction addresses below identify the ROM evidence
 * in reference.txt: ROM = VRAM - 0x801E4EA0 + 0x136DC0.
 * D_801F6764_148684: 30 records, stride 0x3e (801E65EC, 801E665C).
 * Unused bytes remain opaque, rather than inventing scalar widths. */
typedef struct {
    u8 pad00[5];
    u8 b05;       /* lbu/sb: 801E616C/61A0, 801E634C */
    u8 b06, b07;  /* sb: 801E70C4/70C8; lbu: 801E713C/718C */
    u8 pad08[10];
    s8 b12;       /* lb: 801E7130; sb: 801E70AC/70C0 */
    u8 b13;       /* lbu: 801E715C; sb: 801E70CC */
    u8 pad14;
    u8 b15;       /* lbu/sb: 801E6854/6820 */
    u8 pad16[2];
    u8 b18;       /* lbu/sb: 801E6998/6924 */
    u8 b19[8];    /* lbu/sb with index 0..7: 801E6308/6214 */
    u8 b21[8];    /* lbu/sb with index 0..7: 801E635C/621C */
    u8 pad29;
    u16 h2A;      /* lhu/sh: 801E62B0/62D4 */
    s16 h2C;      /* lh/sh: 801E663C/6600 */
    u16 h2E[8];   /* lhu/sh with index 0..7: 801E6200/6210 */
} OvlRecord;
extern OvlRecord D_801F6764_148684[30];

/* D_801F6EAA_148DCA: eight 12-byte records; bzero size 0x60 at 801E6E5C.
 * b00..b04 lbu/sb: 801E6ED8, 6BFC, 6BD4, 6EB0, 6F48.
 * h06/h08/h0A lh: 801E6F68/6F8C/6FB0. */
typedef struct {
    u8 b00, b01, b02, b03, b04, pad05;
    s16 h06, h08, h0A;
} OvlEffect;
extern OvlEffect D_801F6EAA_148DCA[8];

/* D_801F6714_148634: 10 eight-byte records (801E603C..60A0).
 * b00..b05 sb: 801E606C/6074/6070/607C/608C/6084;
 * h06 lhu/sh: 801E605C/6068. */
typedef struct { u8 b00, b01, b02, b03, b04, b05; u16 h06; } OvlEvent;
extern OvlEvent D_801F6714_148634[10];

/* D_801F7060_148F80: four-byte stride, capacity 100 (801E656C..6574).
 * lbu/lbu/lh: 801E66C8/66CC/66D0; sb/sb/sh: 801E6550/6554/654C. */
typedef struct { u8 x, y; s16 value; } OvlCell;
extern OvlCell D_801F7060_148F80[100];
extern u8 D_801F6F0A_148E2A; /* lbu/sb: 801E64D4/6568 */
extern u8 D_801F5129_147049; /* lbu/sb: 801E643C/6474 */

/* Two byte positions: 801E6078/6080 and 801E60C8/60EC. */
typedef struct { u8 x, y; } BytePair;
extern BytePair D_8014BF98;

/* Whole 0xb0 host unit, matching func_8007946C's table stride.
 * h02 lh at 801E6AAC; b07/b09 lbu at 801E6B7C/6ABC;
 * h0C/h0E/h10 lhu at 801E6F64/6F88/6FAC (signed fixed point);
 * b42/b43 sb at 801E67F4/67FC; b70..b73 sb at 801E63EC..6404.
 * These widths agree with canonical func_80074500.c. */
typedef struct {
    u8 pad00[2]; s16 h02;
    u8 pad04[3]; u8 b07;
    u8 pad08; u8 b09;
    u8 pad0A[2]; u16 h0C, h0E, h10;
    u8 pad12[0x30]; u8 b42, b43;
    u8 pad44[0x2C]; u8 b70, b71, b72, b73;
    u8 pad74[0x3C];
} HostUnit;
/* lwc1/swc1 at 801E6D54/6D58/6DBC..6DC8; 12-byte vectors. */
typedef struct { float x, y, z; } Vec3;
/* Metadata pointer (801E6798); 8-byte entry per canonical func_80074784. */
typedef struct { u16 h00; u8 b02, b03; u8 pad04[4]; } HostMetadata;
typedef struct { HostMetadata *metadata; u8 pad04[4]; } HostEntry;
/* Animation table at real base 0x8014D1B4, not interior pointer label.
 * Eight-byte stride / pointer +4: 801E67A0..67AC; frame stride 4 and
 * signed sentinel byte at 801E67B0/67CC. Canonical func_80074BF4. */
typedef struct { s8 id; u8 next; s16 duration; } HostFrame;
typedef struct { u32 count; HostFrame *frames; } HostAnimation;
extern HostAnimation D_8014D1B4[];

void func_800265E0(void *, s32);
void func_80079560(s32, s32, s32);
void func_80059784(s32);
void func_800597A4(s32);
s32 func_80041FF8(void);
HostUnit *func_8007946C(s32, s32);
void func_80075E5C(s32, s32);
s32 func_80076044(s32, s32, s32, s32, s32);
void func_800625B8(s32, s32, s32);
s32 func_80062554(s32, s32);
void func_8008C194(s32);
s32 func_8008BF14(s32, s32, s32, s32, s32, float *, float);
void func_8008C334(s32);
s32 func_8008C1C8(s32, float);
void func_8008C588(s32, Vec3 *, float);
void func_8004194C(s32, s32 *, s32 *);
s32 func_80075F44(s32, s32, s32);
s32 func_800748F8(s32, s32, s32, s32, s32, s32);
HostEntry *func_80074784(s32, s32);
u8 func_80042498(s32);
s32 func_80059830(s32);
s32 func_8004208C(s32, s32, s32);
void func_800766A8(s32, s32);

void func_801E5FB4_137ED4(void);
void func_801E6140_138060(s32);
void func_801E6394_1382B4(s32, s32);
void func_801E6418_138338(void);
void func_801E6BA0_138AC0(s32);
s32 func_801E6C44_138B64(s32, s32, s32, s32, Vec3 *);
void func_801E6E4C_138D6C(void);
#endif
