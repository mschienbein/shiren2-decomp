#ifndef OW2_A1_TYPES_H
#define OW2_A1_TYPES_H
#include "common.h"
#include "record_views.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* Address comments use overlay_136dc0 VRAM. For loaded bytes,
 * ROM = VRAM - 0x801E4EA0 + 0x136DC0; BSS objects have no ROM offset.
 * Pointed-to opaque views are partial; OvlState retains the complete
 * extent corroborated by wave1/s08, s09 and s10. */
/* Operand slots are actually read at all three widths. Examples in B:
 * 801EE0E0 lw +108, 801EE374 lhu +10A, 801EE158 lbu +10B.
 * This is the same four-slot union as wave1/s08, not overlapping scalars. */
typedef union OvlOperand {
    s32 word;
    u16 half[2];
    u8 byte[4];
} OvlOperand;
/* Vec2i copies use lw/sw at +0/+4; BytePair uses sb/lbu at +0/+1 in
 * func_801EDFBC_13FEDC cases 3E..40 and the adjacent s04 callees. */
typedef struct { s32 x, y; } Vec2i;
typedef struct { u8 x, y; } BytePair;
typedef struct OvlItem OvlItem;
typedef struct {
    u8 pad00[8];
    s16 delta08; /* 801EE654/801EE6AC: lh at table+8. */
    u8 pad0A[2];
    void (*method0C)(void *, s32); /* 801EE658/801EE6B0: lw/jalr destructor. */
} OvlItemTable;
struct OvlItem {
    u8 field00;
    u8 field01; /* 801EE244: lbu +1, compared with 0x8A. */
    u8 pad02[6];
    OvlItemTable *field08; /* 801EE650/801EE6A8: lw +8. */
};

/* D_801F5288_1471A8..D_801F5C8C_147BAC: clear size 0xA04 at 801EECC4..801EECD8.
 * Opaque bytes do not assert scalar widths. s09's +11E/+126/+991/+997,
 * s08's four operands and byte fields retain their observed widths. */
typedef struct OvlState {
    u8 pad000[0x108];
    OvlOperand operand[4];
    OvlItem *item118; /* 801EE200 sw / 801EE644 lw. */
    u8 pad11C[2];
    u16 len11E; /* lhu/sh 801F0648/0670. */
    u16 field120; /* s08 ROM 0x13D698/0x13D6B8: sh +120. */
    u8 field122[4]; /* s08 ROM 0x13D5CC..0x13D5D0: four byte tags. */
    char text126[0x180]; /* 801F0658 add +126; stack begins at +2A6. */
    u8 stack2A6[10]; /* s08 ROM 0x13DB7C/C4; next record at +2B0. */
    u8 pad2B0[0x991 - 0x2B0];
    /* 991: s09 func_801EE8CC_1407EC; 992/994: s08 ROM 0x13E400..0x13E45C. */
    u8 field991, field992, pad993, field994, pad995;
    u8 field996, field997, pad998, field999; /* 801EE5E8 sb +999. */
    u8 pad99A[0x9F0 - 0x99A];
    /* 9F0: sb 801EF968/9A4; 9F1/2: s09 func_801EC54C_13E46C;
     * 9F4/5: s08 ROM 0x13DB6C/0x13D6D0, byte stack depths. */
    u8 field9F0, field9F1, field9F2, pad9F3, field9F4, field9F5;
    u8 field9F6, field9F7, field9F8; /* sb: 801F02F0/0304/0754. */
    u8 field9F9; /* 801EE490/801EE4AC: sb +9F9. */
    u8 pad9FA, field9FB; /* sb 801F0740. */
    s32 field9FC; /* sw 801F1440; agrees with s10's 801EEA18. */
    u8 padA00[4];
} OvlState;
extern OvlState *D_801F51E8_147108;
/* Eight 8-byte records at D_801F5C8C_147BAC, matching s08: u16 id and word cursor.
 * 801EE118/124 and 801EE290/2A0 read/increment cursor by one. */
typedef struct { u16 field0; u8 pad2[2]; s32 field4; } OvlScriptRecord;
extern OvlScriptRecord *D_801F51EC_14710C;
extern s8 D_801F5CCC_147BEC; /* signed depth clamp at 801EE718..801EE744. */
extern u8 D_801F5CCD_147BED; /* lbu 801EDFE4, opcode switch 0x3B..0x4F. */
extern char D_801F51F0_147110[]; /* string destination at 801EE208 and 801EE610. */

/* Player's first pair is position; +24 owns the item-collection getter.
 * 801F01A4..01B8 and 801F1364..1378: lh +98, lw/jalr +9C.
 * +1C lhu/sh in opcode 53; +84 lw in opcode 6F. */
typedef struct OvlCollection OvlCollection;
/* Collection slots agree with s08: signed count at +20/+24, item pointer
 * at +38/+3C. ROM 801F1384..13C0 invokes them with the signed adjustments. */
typedef struct {
    u8 pad00[0x20];
    s16 delta20; u8 pad22[2];
    s32 (*count24)(void *);
    u8 pad28[0x10];
    s16 delta38; u8 pad3A[2];
    OvlItem *(*item3C)(void *, u32);
} OvlCollectionTable;
struct OvlCollection {
    void *field00;
    OvlCollectionTable *table04;
    u8 pad08[0x10];
    void *owner18;
};
typedef struct {
    u8 pad00[0x98];
    s16 delta98; u8 pad9A[2];
    OvlCollection *(*collection9C)(void *);
} OvlActorTable;
typedef struct {
    s32 x, y;
    u8 id08, pad09[0x13];
    u16 flags1C;
    u8 flags1E, pad1F[5];
    OvlActorTable *table24;
    u8 pad28[0x5C];
    u32 value84;
} OvlActor;
extern OvlActor *D_801476B8;

/* Scene object's +8 dispatch: lh +30/+38, lw/jalr +34/+3C,
 * 801F006C..00A8. Methods have no additional live arguments. */
typedef struct {
    u8 pad00[0x30];
    s16 delta30; u8 pad32[2]; void (*call34)(void *);
    s16 delta38; u8 pad3A[2]; void (*call3C)(void *);
} OvlSceneTable;
typedef struct { u8 pad00[8]; OvlSceneTable *table08; } OvlScene;
extern OvlScene *D_80142B10;

/* Record widths from canonical func_800B7340. Avoid D_80142F25 interior label. */
typedef struct {
    u8 index, count, previous, field03, masks[2], field06, previousCount, field08;
    s8 result;
    u8 field0A;
} OvlSelectionSave;
extern OvlSelectionSave D_80142F24;
/* Independent byte arrays: 801EFBF4..801EFC1C clears 16/4/4 bytes;
 * s06 proves the complete 32-byte D_801F5228_147148 object.
 * D_801F5204_147124 in case 17 is a folded base D_801F5214_147134 - 0x10. */
typedef struct { u8 mask, row, column, active; } OvlSelection;
extern u8 D_801F5210_147130[4], D_801F5214_147134[4], D_801F5228_147148[32];
extern OvlSelection D_801F5218_147138[4];

/* Selected item wrapper: canonical A0EF4 +0 pointer/+4 item;
 * 801F050C lw +4 and 801F0514 lbu +B. A0DA8 stores a word at +8.
 * ODD_C: real word/byte union for the selected sub-index. */
typedef struct { OvlCollection *collection; OvlItem *item; OvlOperand index; } OvlItemCursor;
/* Canonical func_800A202C S/Obj views: pointer/+4 word/+8 selector,
 * pointed-to value at +C. This preserves full 12-byte cursor storage. */
typedef struct { u8 pad00[0xC]; s32 value0C; } OvlValueObject;
typedef struct { OvlValueObject *object; s32 value; s32 field08; } OvlValueCursor;
/* 801F029C..02CC writes these fields in a 0x16-byte zeroed record. */
typedef struct {
    u8 id00, field01;
    EventCoordinates coordinates;
    u8 pad04[8];
    s16 value0C;
    u8 pad0E[8];
} OvlSpawnRecord;
/* 800D561C pool view; two pointers, temporary table transitions in cases 59..5C. */
typedef struct { void *data; const void *table; } OvlPool;
/* Canonical save-record accesses: lhu +0 and lw +18. */
typedef struct { u16 id00; u8 pad02[0x16]; u32 seconds18; } OvlSaveRecord;
/* 801F1114..1130: stride-three descriptor and byte
 * kind/variant accesses. The total host-table extent remains unknown. */
typedef struct { u8 kind, field01, variant; } OvlItemDescriptor;
extern OvlItemDescriptor D_80148780[];
extern void *D_80147FD0[];
extern const u8 D_80149DA8[], D_80149DB8[], D_801545E0[];
extern u8 D_801480CE;
/* Address-only host objects: unknown full extents remain incomplete. */
extern u8 D_80140100[], D_80140130[], D_80140160[];
extern u8 D_801404E0[], D_80141AD0[], D_80142930[], D_80147620[];
extern u8 D_80147F90[], D_80147FB8[];
extern const char D_801F60F4_148014[], D_801F6100_148020[];
typedef struct { u8 pad00[0x408]; u16 message408; } OvlMenuView;

#endif
