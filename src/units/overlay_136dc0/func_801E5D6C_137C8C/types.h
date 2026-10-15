/* OW1 slice s03 (overlay_136dc0, VRAM 0x801E4EA0..0x801E6000) object views.
 * Started from scratch/omp/overlays/compiler-id/ovl_types.h. Every field below cites the ROM accesses
 * (overlay_136dc0 VRAM of the instruction) that fix its offset and width. Unknown spans are kept as
 * byte arrays so no width is asserted that the ROM does not show. */
#ifndef S03_TYPES_H
#define S03_TYPES_H
#include "common.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef float f32;

/* *D_801F51E8_147108 is static D_801F5288_1471A8 (0xA04 bytes), installed and cleared by
 * func_801EEC9C_140BBC at 801EECD0..801EECD8; this is a partial view, not an allocation.
 * Text ends at +0x2A6; the independent 10-byte stack ends at +0x2B0.
 * The 160 ten-byte ring records end at +0x8F0; their 160 flags end at +0x990.
 * Unaccessed stack/state spans below are opaque, not extra text or queue capacity. */
typedef struct {
    u8 unk0[0x11E];
    u16 x11E;          /* lhu 801E5044, sh 801E5100 */
    u8 unk120[6];
    char x126[0x180];  /* sb x126[x11E] 801E504C; &x126 passed as src 801E50AC */
    u8 unk2A6[0xA];    /* independent stack: lbu 801EBC5C, sb 801EBCA4; unaccessed here */
    char x2B0[160][10]; /* lbu/sb [x9F2][0] 801E4F30/801E4F48; sb [x9F1][0] 801E514C; &x2B0[i] 801E4F90/801E50A4 */
    u8 x8F0[0xA0];     /* lbu [x9F2] 801E4ECC/801E4FA4; sb [x9F1] 801E5038/801E512C */
    u8 unk990[8];      /* independent state: lbu +0x990 at 801EC5E4, +0x992 at 801EC4E0 */
    u8 x998;           /* lbu 801E50B8, sb 801E50EC */
    u8 x999;           /* lbu 801E50C4 */
    u8 unk99A;
    u8 x99B;           /* sb 801E5024 */
    u8 unk99C;
    u8 x99D;           /* lbu 801E5058, sb 801E5068 */
    u8 unk99E[0x53];
    u8 x9F1;           /* lbu 801E4EB8/801E5030/801E5124, sb 801E5108/801E5164 */
    u8 x9F2;           /* lbu 801E4EC4/801E4EEC/801E4F1C, sb 801E4EF8/801E4F44/801E4FCC */
} D_801F51E8_view;
extern D_801F51E8_view *D_801F51E8_147108;

/* D_801F66E0_148600[3]: 16-byte streaming cursors; D_801F6710_148630 points at the active one.
 * 801E5B5C uses &D_801F66E0_148600[i + 1] (= 0x801F66F0 + 16*i); 801E5478/801E53D8 use &D_801F66E0_148600[0]. */
typedef struct {
    u8 x0;     /* sb 801E5B8C; lbu 801E59FC */
    u8 unk1;
    u16 x2;    /* sh 801E5488/801E5B80, lhu 801E5324 (and (u8) lbu 3 at 801E58CC) */
    u16 x4;    /* sh 801E5490/801E5B84/801E5400/801E53F0/801E5C48, lhu 801E533C/801E53F4/801E5A6C/801E5C24 */
    u16 x6;    /* sh 801E549C/801E5414, lhu 801E5450 */
    u16 x8;    /* sh 801E59A0/801E5A1C, lhu 801E5B98 */
    u16 xA;    /* sh 801E59AC/801E5A24, lhu 801E5B9C */
    u32 xC;    /* sw 801E5920, lw 801E5A68 */
} D_801F66E0_view;
extern D_801F66E0_view D_801F66E0_148600[3];
extern D_801F66E0_view *D_801F6710_148630; /* lw/sw throughout 801E52EC..801E5C80 */

/* D_801F6550_148470: 0x190-byte ROM staging buffer (DMA size 0x190 at 801E5358/801E5918/801E5A5C). */
extern u8 D_801F6550_148470[0x190];

/* D_801F6764_148684[30]: 0x3E-byte records (bzero 0x744 = 30 * 0x3E at 801E5258; loop 29..0 at 801E5D74). */
typedef struct {
    u8 x0;     /* lbu 801E5EA0/801E5D90/801E5DA8/801E5E1C, sb 801E5D2C/801E5DB0/801E5DDC */
    u8 x1;     /* sb 801E5D38, lbu 801E5DC0/801E5DD4/801E5E10 */
    u8 x2;     /* sb 801E5D34, lbu 801E5E14 */
    u8 x3;     /* sb 801E5D30, lbu 801E5E18 */
    u8 x4;     /* sb 801E5D48, lbu 801E5DC8 */
    u8 unk5[0x39];
} D_801F6764_view;
extern D_801F6764_view D_801F6764_148684[30];

/* B .data scalars used by this slice (values from ROM 0x147048..0x147060). */
extern u8 D_801F5128_147048;   /* lbu 801E5590, sb 801E55A8; ROM 00 */
extern u8 D_801F5129_147049;   /* sb 801E52E0; ROM 00 */
extern u8 D_801F512A_14704A;   /* sb 801E52B0, lbu 801E56B4; ROM 01 */
extern s16 D_801F512C_14704C;  /* sh 801E52A8/801E5560, lh 801E55EC/801E56D4/801E5720; ROM 0000 */
extern s32 D_801F5130_147050;  /* sw 801E5274/801E5520, lw 801E51D4/801E54F0/801E5508; ROM ffffffff */
extern s32 D_801F5134_147054;  /* sw 801E527C; ROM ffffffff */
extern s32 D_801F5138_147058;  /* sw 801E5284; ROM ffffffff */
extern s32 D_801F513C_14705C;  /* sw 801E528C/801E51F0, lw 801E51E0; ROM ffffffff */

/* B BSS objects cleared by func_801E5244_137164 (bzero sizes at 801E5268/801E52C8). */
extern u8 D_801F6540_148460[6];
extern u8 D_801F71F0_149110[14];
extern u8 D_801F6EA8_148DC8;   /* sb 801E5298 */
extern u8 D_801F6F0A_148E2A;   /* sb 801E52A0 */

/* Argument of func_801E5564_137484 / func_801E5760_137680 (caller-owned record; 0x16 bytes proven). */
typedef struct {
    u8 x0;     /* lbu 801E5580; sb 801E577C */
    u8 unk1;
    u8 x2;     /* lbu 801E56E0/801E5708/801E5724/801E5730 */
    u8 unk3;
    s16 x4;    /* lh 801E55FC/801E5644 */
    s16 x6;    /* lh 801E566C; sh -12345 801E5774 */
    s16 x8;    /* lh 801E5620/801E5658 */
    s16 xA;    /* lh 801E568C; sh -1 801E578C */
    s16 xC;    /* lh 801E55B4 (and lbu 0xD 801E55C8: (u8) of xC); sh -1 801E5780 */
    s16 xE;    /* lh 801E55D0 (and lbu 0xF 801E55E0: (u8) of xE); sh -1 801E5784 */
    u16 x10;   /* lhu 801E55F8 */
    u16 x12;   /* lhu 801E561C */
    u16 x14;   /* lhu 801E56AC; sh (f32 -> unsigned) 801E57B8/801E57D0 */
} D_801E5564_arg;

/* D_801F6714_148634[10]: 8-byte timed sound/event slots (bzero 0x50 at 801E5FC8; loop 9..0 at 801E5EC4/801E5FCC). */
typedef struct {
    u8 x0;     /* lbu 0(s2) 801E5F00 */
    u8 x1;     /* lbu/sb -5(s0) 801E5EF4/801E5EFC, sb 0 801E5F6C */
    u8 x2;     /* lbu -4(s0) 801E5F1C */
    u8 x3;     /* lbu -3(s0) 801E5F30 */
    u8 x4;     /* lbu -2(s0) 801E5F40 */
    u8 x5;     /* lbu/sb -1(s0) 801E5F60/801E5F74 */
    u16 x6;    /* lhu 801E5EE8, lh 801E5F54 (as s16 id), sh 0xFFFF 801E5F80/801E5FE4 */
} D_801F6714_view;
extern D_801F6714_view D_801F6714_148634[10];

/* Sprite returned by func_8007946C (canonical Sprite8008A474: alpha u8 at 0x46). */
typedef struct {
    u8 padA[0xB];
    u8 xB;     /* lbu 801E5D3C */
    u8 padC[0x3A];
    u8 alpha;  /* sb 801E5D1C/801E5DC4/801E5E60 */
} D_8007946C_sprite;

/* Camera view filled by func_80059728 and consumed by func_8005ABBC/func_8005A2E4/func_8005A464
 * (canonical ViewSrc in src/units/main_14400/func_8005A2E4.c: Vec3 eye, Vec3 at, f32 fov, f32 roll). */
typedef struct {
    f32 x, y, z;
} Vec3;
typedef struct {
    Vec3 eye;   /* swc1 sp+0x10/0x14/0x18 801E5618/801E5688/801E5640 */
    Vec3 at;    /* swc1 sp+0x1C/0x20 801E55E4/801E55CC */
    f32 fov;    /* swc1 sp+0x28 801E56CC */
    f32 roll;   /* swc1 sp+0x2C 801E56A8 */
} ViewSrc;

/* Views of the D_801F6550_148470 staging buffer after the ROM reads of func_801E58A0_1377C0. */
typedef struct {
    u32 offset;   /* lw 0(a3) 801E58EC (added to D_0023E210) */
    u32 x4;       /* lw 4(a3) 801E5914 (stored to cursor xC) */
} D_801F6550_index;
typedef struct {
    u8 key;       /* lbu 0(a0) 801E59F0 */
    u8 unk1;
    u16 value;    /* lhu 2(a0) 801E599C/801E59A4/801E5A18; lhu 6(a0) = next record 801E5A20 */
} D_801F6550_record;
/* 0x180-byte block moved down by the unaligned-start skip (inline block move 801E592C..801E59DC). */
typedef struct {
    u8 bytes[0x180];
} D_801F6550_block;

s32 func_801E52EC_13720C(void);

#endif
