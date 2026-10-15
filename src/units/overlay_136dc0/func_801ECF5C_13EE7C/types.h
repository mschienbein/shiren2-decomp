/* s09 private overlay_136dc0 views (OW1). Widths/offsets from ROM accesses in this slice. */
#ifndef S09_TYPES_H
#define S09_TYPES_H
#include "common.h"
#include "record_views.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* Partial field view of static D_801F5288_1471A8, installed through D_801F51E8_147108 at
 * 801EECD0 and cleared for 0xA04 bytes at 801EECD8. Preserve that full extent. */
typedef struct D_801F51E8_s {
    u8 b000;            /* sb 0x0 = 1: func_801EDBFC_13FB1C */
    u8 b001;            /* sb/lbu 0x1: func_801EDBFC_13FB1C */
    u8 b002;            /* lbu 0x2: func_801EDBFC_13FB1C */
    u8 pad_003;
    s16 h004;           /* sh 0x4: func_801EDBFC_13FB1C */
    s16 h006;           /* sh 0x6: func_801EDBFC_13FB1C */
    s16 h008;           /* sh 0x8: func_801EDBFC_13FB1C */
    s16 h00A;           /* sh 0xA: func_801EDBFC_13FB1C */
    s16 h00C;           /* sh 0xC: func_801EDBFC_13FB1C */
    s16 h00E;           /* sh 0xE: func_801EDBFC_13FB1C */
    s16 h010;           /* sh 0x10: func_801EDBFC_13FB1C */
    s16 h012;           /* sh 0x12: func_801EDBFC_13FB1C */
    s16 h014;           /* sh 0x14: func_801EDBFC_13FB1C */
    u8 pad_016[0x108 - 0x16];
    s32 x108;           /* lw 0x108 / (u8) lbu 0x10B: func_801ED5A0_13F4C0; lw, (u16) lhu 0x10A: func_801EDBFC_13FB1C */
    s32 x10C;           /* lw 0x10C / (u8) lbu 0x10F: func_801ED5A0_13F4C0; lw 0x10C: func_801EC87C_13E79C */
    s32 x110;           /* lw 0x110: func_801EC87C_13E79C, func_801EDBFC_13FB1C; (u16) lhu 0x112: func_801EDBFC_13FB1C */
    s32 x114;           /* lw 0x114: func_801EDBFC_13FB1C */
    u8 pad_118[0x6];
    u16 len_11E;        /* lhu/sh 0x11E: func_801ECF5C_13EE7C, func_801EE7B8_1406D8, func_801EC568_13E488 */
    u8 pad_120[0x6];
    u8 buf_126[0x180];  /* sb 0x126(base+len): func_801ECF5C_13EE7C, func_801EE7B8_1406D8, func_801EC568_13E488; &0x126 passed: func_801EC568_13E488.
                         * Extent ends at the independent stack at 0x2A6. */
    u8 x2A6[10];        /* lbu 0x2A5+b9F4: func_801EC568_13E488; stack ends at the +0x2B0 record region. */
    u8 records_2B0[160][10]; /* Opaque payloads; 801E4F20..801E4F30 computes ten-byte record stride. */
    u8 record_flags_8F0[160]; /* Separate per-record byte flags [0x8F0,0x990), func_801E4EA0_136DC0. */
    u8 b990;            /* lbu 0x990 switch 1..6: func_801EC568_13E488 */
    u8 b991;            /* lbu/sb 0x991: func_801EE8CC_1407EC */
    u8 pad_992[0x5];
    u8 b997;            /* lbu 0x997: func_801EE8CC_1407EC, func_801EC568_13E488 */
    u8 b998;            /* sb 0x998 = 1: func_801EE7B8_1406D8; lbu ==1, sb 0: func_801EC568_13E488 */
    u8 b999;            /* lbu 0x999 == 2: func_801EE7B8_1406D8, func_801EC568_13E488 */
    u8 pad_99A[0x3];
    u8 b99D;            /* lbu 0x99D == 1, sb 0: func_801EC568_13E488 */
    u8 pad_99E[0x53];
    u8 b9F1;            /* sb 0x9F1: func_801EC54C_13E46C */
    u8 b9F2;            /* sb 0x9F2: func_801EC54C_13E46C */
    u8 b9F3;            /* lbu 0x9F3: func_801EC568_13E488 */
    u8 b9F4;            /* lbu 0x9F4: func_801EC568_13E488 */
    u8 pad_9F5;
    u8 b9F6;            /* lbu 0x9F6: func_801ECC10_13EB30 */
    u8 pad_9F7[0x2];
    u8 b9F9;            /* lbu 0x9F9: func_801ECC10_13EB30 */
    u8 pad_9FA[0xA04 - 0x9FA];
} D_801F51E8_s;

extern D_801F51E8_s *D_801F51E8_147108;

/* Eight-byte cursor record: func_801EB5D4_13D4F4 clears eight bytes at 801EB604/608,
 * then stores the u16 id at 801EB618 and the s32 cursor at 801EB61C. */
typedef struct D_801F51EC_s {
    u16 id;
    u8 pad_002[2];
    s32 x4;             /* lw 0x4: func_801EE8CC_1407EC; lw/addiu/sw 0x4 (x4++): func_801EE7B8_1406D8, func_801EDBFC_13FB1C */
} D_801F51EC_s;

/* Link pair: canonical func_800DF61C.c Link {s32 field_0; s32 active;} at 0x84 (linkA). */
typedef struct CellLink {
    s32 field_0;
    s32 active;         /* lw 0x4 of &cell->link_84: func_801ECECC_13EDEC */
} CellLink;

/* Record returned by func_800A8CB0 (0xE4 bytes; canonical func_800A8CB0.c views it as u8[0xE4]).
 * In func_801EDBFC_13FB1C the same record is read as a Vec2i (lw 0x0/0x4 struct copy). */
typedef struct Cell801ECECC {
    u8 pad_00[0x1E];
    u8 flags_1E;        /* lbu 0x1E, bit 3: func_801ECECC_13EDEC (canonical func_801217DC: u8 flags_1E) */
    u8 pad_1F[0x84 - 0x1F];
    CellLink link_84;   /* addiu 0x84 then lw 0x4: func_801ECECC_13EDEC */
} Cell801ECECC;

/* Map position (canonical func_800B50D4.c Vec2i): lw/sw 0x0 and 0x4 in func_801EDEF8_13FE18;
 * (u8) low bytes lbu 0x3/0x7 in func_801ECE10_13ED30; sw zero 0x0/0x4 in func_801ECC10_13EB30. */
typedef struct { s32 x; s32 y; } Vec2i;

/* Main-image SelectionRecord D_80142F18 (canonical func_800DF9F0.c view, verbatim). */
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05; unsigned short field_06; unsigned char mode; signed char coordinates[2], status; } SelectionRecord;

/* 0x16-byte request cleared by func_800265E0(&spec, 0x16) in func_801ECE10_13ED30 (frame slot 0x18 bytes). */
typedef struct Spec801ECE10 {
    u8 kind_00;         /* sb 0x0 = 0x5F */
    u8 pad_01;
    EventCoordinates coordinates; /* +2 packed halfword / y,x bytes */
    u8 pad_04[0x8];
    s16 h0C;            /* sh 0xC = 0x297 */
    s16 h0E;            /* sh 0xE = 0x91 */
    u8 pad_10[0x6];
} Spec801ECE10;

/* Object returned by func_801F1510_143430 (partial). */
typedef struct Popup801F1510 {
    u8 pad_00[0x52];
    u8 b52;             /* sb 0x52 = 1: func_801ECE10_13ED30 */
} Popup801F1510;

/* Main-image area D_801429C0 (canonical func_800A324C.c Rect view); only its address is used here. */
typedef struct {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} Rect;


/* D_80142B10 object (canonical func_800C6B70.c Obj/VTable view, vtable pointer at +0x8). */
typedef struct VTable801ECC10 {
    u8 pad_00[0x20];
    s16 delta20;        /* lh 0x20: func_801ECC10_13EB30 */
    s16 pad22;
    void (*fn20)(void *); /* lw 0x24, jalr: func_801ECC10_13EB30 */
} VTable801ECC10;

typedef struct World801ECC10 {
    u8 pad_00[0x8];
    VTable801ECC10 *vtable; /* lw 0x8: func_801ECC10_13EB30 */
} World801ECC10;

/* Main-image actor D_801476B8: position first (passed as Vec2i * to func_801ECE10_13ED30), flag at 0x52. */
typedef struct Actor801476B8 {
    Vec2i pos;
    u8 pad_08[0x52 - 0x8];
    u8 b52;             /* sb 0x52 = 1: func_801ECC10_13EB30 */
} Actor801476B8;

/* Iterator handed to func_800A8F6C/func_800A910C (canonical func_800C799C.c Iterator800C799C). */
typedef struct { s32 unk0; s32 unk4; } Iterator801EC87C;

#endif
