/* OW2 o2 private overlay_136dc0 views (func_801EAAA8_13C9C8, func_801EBCA8_13DBC8; slice s08 range).
 * Shared objects use slice s08's names and layout (wave1/s08/types.h: OvlOperand,
 * OvlState, OvlScriptRecord, D_801F51EC_14710C, D_801F5CCD_147BED). The fields s08 leaves opaque
 * are added below with their ROM evidence (instruction VRAM). Main-image views follow
 * canonical precedent: func_800DF61C (links at +0x78/+0x84), func_800B7680 (component
 * vtable at +0x30), func_80041C44 and others (SelectionRecord), func_80041F68
 * (D_801F5228_147148). */
#ifndef OW2_O2_TYPES_H
#define OW2_O2_TYPES_H
#include "common.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* s08 OvlOperand. Here: word reads lw 0x108..0x114 (801EAADC..801EAAE4, 801EBD9C,
 * 801EBF64..801EBF74); u16 read of slot 0 lhu 0x10A (801EAAD8, 801EBCF4); u8 reads
 * of slots 0/1 lbu 0x10B/0x10F (801EBE3C 801EBE54 801EC310/801EC318). */
typedef union OvlOperand {
    s32 word;
    u16 half[2];
    u8 byte[4];
} OvlOperand;

/* s08 OvlState (0xA04 bytes), plus the fields func_801EBCA8_13DBC8 touches. */
typedef struct OvlState {
    u8 pad000[0x108];
    OvlOperand field108[4];
    u8 pad118[6];
    /* Text length: lhu/sh 0x11E at 801EBFC4/801EBFD0, 801EC004/801EC014,
     * 801EC03C/801EC048, 801EC0B0/801EC0BC; u16 (andi 0xFFFF at 801EBFCC). */
    u16 field11E;
    u16 field120;       /* s08: sh 0x120 */
    u8 field122[4];     /* s08: operand tags */
    /* Text buffer indexed by field11E: sb 0x126(base+len) at 801EBFD8;
     * swl/swr 0x126/0x129 at 801EC02C/801EC030; strcpy destination at 801EC08C.
     * The extent stops at s08's field2A6. */
    u8 field126[0x2A6 - 0x126];
    u8 field2A6[10];    /* s08 */
    u8 pad2B0[0x992 - 0x2B0];
    u8 field992;        /* s08 */
    u8 pad993;
    u8 field994;        /* s08 */
    u8 pad995[0x99E - 0x995];
    /* Count for the two word arrays below: lbu 0x99E at 801EBFB0/801EBFE4,
     * sb at 801EBFF4. */
    u8 field99E;
    u8 pad99F;
    /* Word arrays indexed by field99E: sw 0x9A0(base + 4*n) at 801EBFC0,
     * sw 0x9C8(base + 4*n) at 801EC010. They are adjacent, so the first holds
     * 10 words; the second ends before 0x9F0. */
    s32 field9A0[10];
    s32 field9C8[10];
    u8 pad9F0[4];
    u8 field9F4;        /* s08; here sb zero 0x9F4 at 801EBD88 801EC354 */
    u8 field9F5;        /* s08 */
    u8 pad9F6[0xA04 - 0x9F6];
} OvlState;
extern OvlState *D_801F51E8_147108;

/* s08 OvlScriptRecord. Here: lw/sw field4 at 801EBEC4/801EBED4 (x4 pattern),
 * 801EBF88/801EBF98, 801EC340/801EC350. */
typedef struct OvlScriptRecord {
    u16 field0;
    u8 pad2[2];
    s32 field4;
} OvlScriptRecord;
extern OvlScriptRecord *D_801F51EC_14710C;   /* lw at 801EBD58; sw at 801EBD68 */
extern u8 D_801F5CCD_147BED;                 /* lbu 801EBCAC (s08) */
extern u8 D_801F5228_147148[];               /* lbu D_801F5228_147148[(u8)id] at 801EBE48 (canonical func_80041F68) */

/* Canonical SelectionRecord (func_80041C44 and others). Struct copy lwl/lwr x3 at
 * 801EC120..801EC14C; mode lbu sp+0x28 at 801EC150; coordinates lb sp+0x29/0x2A at
 * 801EC188/801EC1B0. */
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05; unsigned short field_06; unsigned char mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

/* Unit vtable (unit +0x24). */
typedef struct UnitVtbl {
    u8 pad_00[0x8];
    /* Deleting destructor slot: lh 0x8, lw 0xC, called with 3 at 801EC0C8/801EC0CC. */
    s16 delta08; s16 index08; void (*fn08)(void *self, s32 flags);
    u8 pad_10[0x58];
    s16 delta68; s16 index68; u32 (*fn68)(void *self);            /* lh/lw 0x68/0x6C: 801EAC48 */
    s16 delta70; s16 index70; u32 (*fn70)(void *self);            /* lh/lw 0x70/0x74: 801EAC68 801EACC8 */
    s16 delta78; s16 index78; void (*fn78)(void *self, s16 v);    /* lh/lw 0x78/0x7C: 801EAD24 */
    u8 pad_80[0x10];
    s16 delta90; s16 index90;
    s32 (*fn90)(void *self, s32 a, s32 b, u8 c, s32 d);           /* lh/lw 0x90/0x94: 801EAD8C.. */
} UnitVtbl;

/* Component vtable slot (canonical func_800B7680 Entry). */
typedef struct ComponentSlot {
    s16 delta;
    s16 index;
    void (*fn)(void *self, s32 arg);
} ComponentSlot;

/* Component vtable (component +0x30). */
typedef struct ComponentVtbl {
    u8 pad_00[0x8];
    ComponentSlot slot08;   /* lh 0x8, lw 0xC: 801EAEB0/801EAEB4 801EAF9C/801EAFA0 */
} ComponentVtbl;

/* Component sub-object of a unit (+0x78 or +0x84). The +0x84 head matches
 * canonical func_800DF61C Link {field_0; active} and s09's CellLink. */
typedef struct Component {
    s32 x0;             /* sw 0x0: 801EAE8C 801EAF90 */
    s32 x4;             /* lw 0x4 (via +0x84): 801EBD48 */
    u8 pad_08[0x28];
    ComponentVtbl *vtable;  /* lw 0x30: 801EAE9C 801EAF4C; 0xB4 = 0x84 + 0x30: 801EAF94 */
} Component;

/* Unit record returned by main func_800A8CB0 (0xE4-byte records; partial view).
 * The +0x78 and +0x84 components belong to different unit kinds (flags1E bit 6
 * or bit 3) and overlap. */
typedef struct Unit {
    u8 pad_00[0x1E];
    u8 flags1E;         /* lbu 0x1E: 801EAE68 801EAF2C 801EBD1C */
    u8 pad_1F[0x5];
    UnitVtbl *vtable;   /* lw 0x24: 801EAC40.. 801EC0C0 */
    u8 pad_28[0x50];
    union {
        Component c78;  /* +0x78; vtable at 0xA8 */
        struct {
            u8 pad_78[0xC];
            Component c84;  /* +0x84; vtable at 0xB4 */
        } p;
    } u78;
    u8 pad_B8[0xE4 - 0xB8];
} Unit;

#endif
