#ifndef OW2_A1_TYPES_H
#define OW2_A1_TYPES_H
#include "common.h"
#include "record_views.h"
#include "item_records.h"
extern "C++" {
#include "ovlb_shared_views.h"
}
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* Address comments use overlay_136dc0 VRAM. For loaded bytes,
 * ROM = VRAM - 0x801E4EA0 + 0x136DC0; BSS objects have no ROM offset.
 * Pointed-to opaque views are partial. OvlPairCollection includes the live
 * +0xBC..+0xD0 state used by 800D8514/8618/84C0 (full size 0xD4). */
typedef struct { u8 x, y; } BytePair;
/* Items, their method tables, item tables, handles and the item cursor are the
 * shared item_records.h contracts of the main-image providers. */
typedef Item OvlItem;
typedef ItemMethods OvlItemTable;

/* The interpreter and all migrated consumers use the same shared object view. */
extern Obj801F51E8 *D_801F51E8_147108;
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
/* Receiver-only prefix; ItemCollection from item_records.h stays incomplete. */
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

extern OvlBSelectionSave D_80142F24;
/* Independent byte arrays: 801EFBF4..801EFC1C clears 16/4/4 bytes;
 * s06 proves the complete 32-byte D_801F5228_147148 object.
 * D_801F5204_147124 in case 17 is a folded base D_801F5214_147134 - 0x10. */
typedef struct { u8 mask, row, column, active; } OvlSelection;
extern u8 D_801F5210_147130[4], D_801F5214_147134[4], D_801F5228_147148[32];
extern OvlSelection D_801F5218_147138[4];

/* Selected item wrapper: canonical A0EF4 +0 pointer/+4 item;
 * 801F050C lw +4 and 801F0514 lbu +B. A0DA8 stores a word at +8.
 * ODD_C: real word/byte union for the selected sub-index. */
typedef ItemCursor OvlItemCursor;
/* Canonical func_800A202C S/Obj views: pointer/+4 word/+8 selector,
 * pointed-to value at +C. This preserves full 12-byte cursor storage. */
typedef struct { u8 pad00[0xC]; s32 value0C; } OvlValueObject;
typedef struct { OvlValueObject *object; s32 value; s32 field08; } OvlValueCursor;
extern void func_800D03C4(u8 *member, s32 flags);
extern void *func_800D0180(void *pair);
extern const u8 D_801545E0[];
/* 800D0180 zeroes the two non-owning links. 800D01B8 stores and validates
 * them; neither operation acquires or releases their pointed-to objects.
 * The ROM's 801EF7C0..801EF7DC cleanup and the analogous 800D97EC/9A38
 * destructors traverse 21 records with no per-record side effects.
 * OvlPair's user-provided empty destructor is non-trivial in C++.
 * The ROM helper is g++'s __tcf_0 for the interpreter's local static
 * (cross-family review F1; Main's wave32 message). */
extern "C++" {
typedef struct OvlPair {
    void *container;
    OvlItem *item;
    OvlPair() { func_800D0180(this); }
    ~OvlPair() {}
} OvlPair;
/* 800D03B4 stores items/capacity/count at member+8/+C/+10.
 * 800D03C4 restores the two list vtables and clears count. */
typedef struct OvlPairMember {
    void *collectionA8;
    const void *tableAC;
    OvlPair *itemsB0;
    s32 capacityB4;
    s32 countB8;
    OvlPairMember() { tableAC = D_801545E0; }
} OvlPairMember;
typedef struct OvlPairCollection {
    OvlPair records[21];
    OvlPairMember memberA8;
    s32 nextBC;
    void *groupC0;
    OvlItem *itemC4;
    s32 entryC8;
    OvlCollection *ownerCC;
    s32 stateD0;
    /* The out-of-line list destructor receives g++ 2.x __in_chrg = 2.
     * Native reverse member destruction then emits the observed pair loop. */
    ~OvlPairCollection() {
        func_800D03C4((u8 *)&memberA8, 2);
    }
} OvlPairCollection;
}
/* 801F029C..02CC writes these fields in a 0x16-byte zeroed record. */
typedef struct {
    u8 id00, field01;
    EventCoordinates coordinates;
    u8 pad04[8];
    s16 value0C;
    u8 pad0E[8];
} OvlSpawnRecord;
/* 800D561C pool view; two pointers, temporary table transitions in cases 59..5C. */
typedef Handle OvlPool;
/* Canonical save-record accesses: lhu +0 and lw +18. */
typedef struct { u16 id00; u8 pad02[0x16]; u32 seconds18; } OvlSaveRecord;
/* 801F1114..1130: stride-three descriptor and byte
 * kind/variant accesses. The total host-table extent remains unknown. */
typedef struct { u8 kind, field01, variant; } OvlItemDescriptor;
extern OvlItemDescriptor D_80148780[];
extern s32 D_801F5D00_147C20;
extern u8 D_801480CE;
extern const MenuPos D_80138F80[3];
/* Address-only host objects: unknown full extents remain incomplete. */
extern u8 D_80140100[], D_80140130[], D_80140160[];
extern u8 D_80141AD0[], D_80142930[], D_80147620[];
/* Canonical 80092E98's complete 0x40C MenuPanel; this TU needs no second completion. */
struct MenuPanel;
extern MenuPanel D_801404E0;
extern u8 D_80147F90[], D_80147FB8[];
extern const char D_801F60F4_148014[], D_801F6100_148020[];
typedef struct { u8 pad00[0x408]; u16 message408; } OvlMenuView;

/* Function-local holder: provider A1888 stores+0; A18AC also writes+4. */
typedef struct { void *list; void *field04; } OvlListHolder;

#endif