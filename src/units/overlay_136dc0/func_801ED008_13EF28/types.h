/* Astra correction of the Opus OW2 o3 overlay_136dc0 views.
 * The ten-byte stack agrees with wave1/s08 and wave2/a1/o2; wave1/s09's
 * competing extent is a separate integration issue, not adopted here.
 * Every named field lists its ROM access evidence; unused spans stay opaque. */
#ifndef O3_TYPES_H
#define O3_TYPES_H
#include "common.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* Object behind the B pointer D_801F51E8_147108 (partial view, through 0xA04). */
typedef struct D_801F51E8_s {
    u8 pad_000;
    u8 b001;            /* lbu 0x1, compared with 0xFF: func_801ED720_13F640 */
    u8 b002;            /* sb 0x2: func_801ED720_13F640 */
    u8 pad_003[0xD];
    u16 h010;           /* sh 0x10: func_801ED720_13F640 */
    u16 h012;           /* sh 0x12: func_801ED720_13F640 */
    u8 pad_014[0xF4];
    s32 arg_108;        /* lw/sw 0x108; low byte lbu 0x10B: func_801ED008_13EF28, func_801ED720_13F640 */
    s32 arg_10C;        /* low byte lbu 0x10F: func_801ED008_13EF28, func_801ED5A0_13F4C0, func_801ED720_13F640 */
    s32 arg_110;        /* lw 0x110; low byte lbu 0x113: func_801ED008_13EF28 */
    s32 arg_114;        /* lw/sw 0x114; low byte lbu 0x117: func_801ED008_13EF28, func_801ED720_13F640 */
    u8 pad_118[0x6];
    u16 len_11E;        /* lhu/sh 0x11E: func_801ECF5C_13EE7C, func_801ED008_13EF28, func_801ED720_13F640 */
    u8 pad_120[0x5];
    u8 b125;            /* sb 0x125: func_801ED720_13F640 */
    char buf_126[0x180]; /* message text; sb/swl 0x126+len, strlen(base+0x126): func_801ED008_13EF28; ends at the 0x2A6 array */
    /* ROM 0x13DB7C/0x13DBC4: byte stack, ending where the independent
     * ten-byte ring records start (ROM 0x13705C..0x13706C). */
    u8 codes_2A6[10];
    /* Ring-record storage [0x2B0,0x8F0); payload unused by these dispatchers. */
    u8 pad_2B0[0x8F0 - 0x2B0];
    /* ROM 0x137044..0x13704C: byte flags indexed independently of the
     * records. End at the separate +0x990 state byte, not +0x998. */
    u8 queue_flags_8F0[0xA0];
    u8 b990;            /* lbu/sb 0x990: func_801ED008_13EF28 */
    u8 b991;            /* lbu/sb 0x991: func_801EE8CC_1407EC (OW1s09) */
    u8 pad_992[0x5];
    u8 b997;            /* sb 0x997: func_801ED008_13EF28; lbu: func_801EE8CC_1407EC */
    u8 pad_998;
    u8 b999;            /* sb 0x999: func_801ED008_13EF28 */
    u8 b99A;            /* lbu/sb 0x99A: func_801ED008_13EF28, func_801ED720_13F640 */
    u8 pad_99B;
    u8 b99C;            /* sb 0x99C: func_801ED720_13F640 */
    u8 b99D;            /* sb 0x99D: func_801ED008_13EF28, func_801ED720_13F640 */
    u8 b99E;            /* lbu/sb 0x99E, index of the two s32[10] arrays: func_801ED720_13F640 */
    u8 b99F;            /* sb 0x99F: func_801ED720_13F640; lbu: func_801E9D34_13BC54 */
    s32 w9A0[10];       /* sw 0x9A0+4*b99E, func_800265E0(&w9A0, 10): func_801ED720_13F640 */
    s32 w9C8[10];       /* sw 0x9C8+4*b99E: func_801ED720_13F640 */
    u8 b9F0;
    u8 b9F1;            /* lbu 0x9F1: func_801ED008_13EF28; sb: func_801EC54C_13E46C (OW1s09) */
    u8 b9F2;            /* sb 0x9F2: func_801EC54C_13E46C (OW1s09) */
    u8 b9F3;            /* lbu/sb 0x9F3: func_801ED008_13EF28, func_801ED720_13F640 */
    u8 b9F4;            /* lbu 0x9F4: func_801EC568_13E488 (OW1s09), func_801ED008_13EF28 */
    u8 pad_9F5[0x5];
    u8 b9FA;            /* sb 0x9FA: func_801ED720_13F640 */
    u8 pad_9FB[0x5];
    s32 wA00;           /* sw 0xA00: func_801ED720_13F640 */
} D_801F51E8_s;

extern D_801F51E8_s *D_801F51E8_147108;

/* Shared eight-byte script record: sh id @801EB618; sw cursor @801EB61C.
 * These dispatchers access only count_4; bytes +2..+3 remain opaque. */
typedef struct D_801F51EC_s { u16 id; u8 opaque_2[2]; s32 count_4; } D_801F51EC_s;
extern D_801F51EC_s *D_801F51EC_14710C;

/* Script command byte (lbu D_801F5CCD_147BED, switch index: func_801ED008_13EF28, func_801ED5A0_13F4C0, func_801ED720_13F640). */
extern u8 D_801F5CCD_147BED;

/* Message colour codes: rodata string literals of this TU, read with lwl/lwr (byte alignment). */
extern const char D_801F5ED0_147DF0[4];   /* "#3c" */
extern const char D_801F5F28_147E48[7];   /* "#0l#1l" */
extern const char D_801F5F98_147EB8[4];   /* "#5c" */
extern const char D_801F5F9C_147EBC[4];   /* "#6c" */
extern const char D_801F5FA0_147EC0[4];   /* "#4c" */
extern const char D_801F5FA4_147EC4[5];   /* "#99c" */

#endif
