/* OW2 o1 object views (overlay_136dc0 func_801F1B58_143A78; overlay_1339f0 func_801E6F68_138E88).
 * Started from scratch/omp/overlays/compiler-id/ovl_types.h. Each field cites the ROM accesses
 * (image VRAM of the instruction) that fix its offset and width. Unknown spans stay byte arrays. */
#ifndef OW2_O1_TYPES_H
#define OW2_O1_TYPES_H
#include "common.h"
#include "record_views.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

#ifdef __cplusplus
#include "ovlb_shared_views.h"
/* ---- overlay_136dc0 ------------------------------------------------------------------- */

/* Component that a unit embeds as a non-first base. The owner converts to it through a
 * null-checked base-pointer adjustment (`p ? p + 0x78 : 0`, `p ? p + 0x84 : 0`), which g++ 2.x
 * emits for every Derived* -> Part* conversion (func_801F1B58_143A78, func_801F1F64_143E84). */
struct Part {
    s32 mode;   /* +0x00: sw 801F1C00/1C34/1C68 (B: 801F1DE4/1E18/1E4C) */
    u8 pad04[4];
    s32 f08;    /* +0x08: sw 801F1D18; lw 801F1F94 (func_801F1F64_143E84) */
    s32 f0C;    /* +0x0C: sw 801F1CE8/1CFC */
    s32 f10;    /* +0x10: sw 801F1D3C */
    s32 f14;    /* +0x14: sw 801F1CA0/1CB8/1CC4 */
    u16 f18;    /* +0x18: sh 801F1BA4 */
    u8 pad1A[2];
    EventRect rect; /* +0x1C..+0x2B: full rectangle written by func_801F2B20_144A40 */
    u8 f2C;     /* +0x2C: sb 801F1BB0 */
    u8 f2D;     /* +0x2D: sb 801F1D58 */

    void setMode(s32 value) { mode = value; }
    void setField08(s32 value) { f08 = value; }
    void setField0C(s32 value) { f0C = value; }
    void setField10(s32 value) { f10 = value; }
    void setField14(s32 value) { f14 = value; }
    void setField18(u16 value) { f18 = value; }
    void setField2C(u8 value) { f2C = value; }
    void setField2D(const u8 &value) { f2D = value; }
};

/* Unit base: kind flags byte at +0x1E (lbu 801F1B74/1D5C; func_801F1F64_143E84 801F1F74/1FAC).
 * Bit 6 marks the variant whose Part sits at +0x78, bit 3 the variant whose Part sits at +0x84. */
struct UnitPart78 : OvlBUnitHead, Part {};
struct UnitBody84 : OvlBUnitHead { u8 pad78[0x84 - 0x78]; };
struct UnitPart84 : UnitBody84, Part {};

/* Part settings record passed to func_801F1B58_143A78 (a1). */
struct PartSpec {
    u8 pad00[4];
    u8 f04;     /* +0x04: lbu 801F1D40 (low 3 bits) */
    u8 pad05[7];
    u16 f0C;    /* +0x0C: lhu 801F1B98 */
    u16 f0E;    /* +0x0E: lhu 801F1BB4 (bits 0-1 mode, 3, 4-5, 6, 7) */
    u8 f10;     /* +0x10..0x13: lbu 801F1C10..1C0C, func_801F2B20_144A40 arguments */
    u8 f11;
    u8 f12;
    u8 f13;
    u8 f14;     /* +0x14: lbu 801F1BAC */
};
#endif

#ifndef __cplusplus
/* ---- overlay_1339f0 (C) --------------------------------------------------------------- */

/* libultra controller / controller-pak records (os_cont.h, os_pfs.h layouts; the resident
 * definitions func_80027910, func_800277B8, func_8002E930 and func_8002E470 use the same). */
typedef struct OSMesgQueue OSMesgQueue;
typedef void *OSMesg;
typedef struct {
    u32 ctrl, width, burst, vSync, hSync, leap, hStart, xScale, vCurrent;
} OSViCommonRegs;
typedef struct {
    u32 origin, yScale, vStart, vBurst, vIntr;
} OSViFieldRegs;
typedef struct { /* 0x50 bytes; resident table D_800374E0 (func_80031340) */
    u8 type;
    OSViCommonRegs comRegs;
    OSViFieldRegs fldRegs[2];
} OSViMode;
typedef struct {
    u16 type;
    u8 status;  /* +0x2: lbu 801E6FE8 (CONT_CARD_ON, bit 0) */
    u8 errno;   /* +0x3: lbu 801E6FDC */
} OSContStatus;
typedef struct {
    u16 button; /* +0x0: lhu 801E7054/7184/7714/7724/7760 */
    s8 stick_x;
    s8 stick_y;
    u8 errno;
} OSContPad;
typedef struct {
    s32 status;
    OSMesgQueue *queue;
    s32 channel;
    u8 id[32];
    u8 label[32];
    s32 version;
    s32 dir_size;
    s32 inode_table;
    s32 minode_table;
    s32 dir_table;
    s32 inode_start_page;
    u8 banks;
    u8 activebank;
} OSPfs;
typedef struct {
    u32 file_size;      /* +0x00: lw 801E7590 */
    u32 game_code;      /* +0x04: lw 801E753C */
    u16 company_code;   /* +0x08: lhu 801E7530 */
    char ext_name[4];   /* +0x0A: address 801E7534 */
    char game_name[16]; /* +0x0E: address 801E7540 */
} OSPfsState;
#endif

#endif
