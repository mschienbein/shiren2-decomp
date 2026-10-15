#ifndef OW1_S08_TYPES_H
#define OW1_S08_TYPES_H
#include "common.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* ODD_C: the interpreter accesses operand storage as words (ROM 0x13D974),
 * halfwords (0x13D824/0x13D82C), and bytes (0x13E230/0x13E238).
 * The union records these observed widths rather than conflicting field views. */
typedef union OvlOperand {
    s32 word;
    u16 half[2];
    u8 byte[4];
} OvlOperand;

/* D_801F51E8_147108 points at D_801F5288_1471A8. ROM 0x140BE4..0x140BF8
 * installs that address and clears 0xA04 bytes, ending at D_801F5C8C_147BAC.
 * Unused bytes stay opaque; this is a complete-sized, partial field view. */
typedef struct OvlState {
    u8 pad000[0x108];
    /* Four slots: ROM 0x13D884..0x13D89C extracts a two-bit index;
     * 0x13DB28..0x13DB3C stores a word at 0x108 + 4*index. */
    OvlOperand field108[4];
    u8 pad118[8];
    /* ROM 0x13D698/0x13D6B8: sh 0x120. */
    u16 field120;
    /* ROM 0x13D5CC..0x13D5D0: byte tags for those same four slots. */
    u8 field122[4];
    u8 pad126[0x2A6 - 0x126];
    /* ROM 0x13DB7C/0x13DBC4: byte stack; next record starts at +0x2B0
     * (ROM 0x137068..0x13706C). */
    u8 field2A6[10];
    u8 pad2B0[0x992 - 0x2B0];
    /* ROM 0x13E400/0x13E420, 0x13E440/0x13E45C: lbu/sb. */
    u8 field992;
    u8 pad993;
    /* ROM 0x13D388/0x13D394/0x13D3CC: lbu/sb. */
    u8 field994;
    u8 pad995[0x9F4 - 0x995];
    /* ROM 0x13DB6C/0x13DB94/0x13DBB0: byte stack depth. */
    u8 field9F4;
    /* ROM 0x13D6D0: sb saved script stack depth. */
    u8 field9F5;
    u8 pad9F6[0xA04 - 0x9F6];
} OvlState;
extern OvlState *D_801F51E8_147108;

/* ROM 0x13D50C..0x13D524 selects an 8-byte record; 0x13D528..0x13D540
 * writes its u16 id at +0 and s32 stream position at +4. The clear is 8 bytes.
 * 0x13D4C4 checks depth < 8; D_801F5C8C_147BAC..D_801F5CCC_147BEC is exactly 8 records. */
typedef struct OvlScriptRecord {
    u16 field0;
    u8 pad2[2];
    s32 field4;
} OvlScriptRecord;
extern OvlScriptRecord D_801F5C8C_147BAC[8];
extern OvlScriptRecord *D_801F51EC_14710C;
/* ROM 0x13D510/0x13D558: lb; 0x13D598: sb (signed depth). */
extern s8 D_801F5CCC_147BEC;
/* ROM 0x13DB80..0x13DB84: lbu command byte. */
extern u8 D_801F5CCD_147BED;

typedef struct OvlItem OvlItem;
/* Inventory objects and their tables below are pointer-only prefix views:
 * no allocation, sizeof-based clearing, or whole-object indexing uses them. */
/* Partial table views: each visible delta is an lh, each method an lw/jalr.
 * Collection +0x18/+0x1C: ROM 0x13C7D0..0x13C7DC, setter(self,s32).
 * +0x20/+0x24: 0x13C6B0..0x13C6BC, s32 count(self).
 * +0x38/+0x3C: 0x13C6DC..0x13C6E8, pointer get(self,u32). */
typedef struct OvlCollectionTable {
    u8 pad00[0x18];
    s16 delta18;
    u8 pad1A[2];
    void (*method1C)(void *, s32);
    s16 delta20;
    u8 pad22[2];
    s32 (*method24)(void *);
    u8 pad28[0x10];
    s16 delta38;
    u8 pad3A[2];
    OvlItem *(*method3C)(void *, u32);
} OvlCollectionTable;
typedef struct OvlCollection {
    void *field00;
    OvlCollectionTable *field04; /* ROM 0x13C6AC/0x13C7C8: lw. */
    u8 pad08[0x10];
    void *field18; /* Canonical func_8011422C: owner at collection +0x18. */
} OvlCollection;
/* Item table: destructor +8/+C at ROM 0x13C884..0x13C894;
 * message +0x38/+0x3C at 0x13C7A0..0x13C7B0. */
typedef struct OvlItemTable {
    u8 pad00[8];
    s16 delta08;
    u8 pad0A[2];
    void (*method0C)(void *, s32);
    u8 pad10[0x28];
    s16 delta38;
    u8 pad3A[2];
    s32 (*method3C)(void *, void *);
} OvlItemTable;
struct OvlItem {
    u8 field00; /* ROM 0x13C744/0x13C904: lbu kind. */
    u8 field01; /* ROM 0x13C6F0/0x13C968: lbu id. */
    u8 pad02[2];
    u8 field04; /* ROM 0x13C824: sb quantity. */
    u8 pad05[3];
    OvlItemTable *field08; /* ROM 0x13C884: lw. */
    /* ODD_C: kind-specific derived payloads: byte (0x13C7C0),
     * word (0x13C800), embedded collection (0x13C7C4..0x13C7DC).
     * No simultaneous overlapping scalar fields are asserted. */
    union {
        u8 byte;
        s32 word;
        OvlCollection collection;
    } field0C;
};
/* Same 0x20-byte message view as canonical func_800AB3F4.
 * ROM 0x13C794 writes the s32 tag, 0x13C79C writes the s32 +0x18 flag. */
typedef struct OvlMessage {
    s32 field00;
    u8 pad04[0x14];
    s32 field18;
    u8 pad1C[4];
} OvlMessage;

/* Canonical func_800C9E10 proves the 0x30-byte record extent.
 * ROM 0x13CF6C..0x13CFB4 accesses flag bytes starting at +7; the next
 * non-flag member is the signed byte at +0xA (canonical func_800CB268). */
typedef struct OvlProfile {
    u8 pad00[7];
    u8 flags07[3];
    u8 pad0A[0x30 - 0xA];
} OvlProfile;

extern void func_800265E0(void *dst, s32 size);
extern s32 func_801E5470_137390(u16 id);
extern u8 func_801E53A4_1372C4(s32 position);
extern OvlScriptRecord *func_801EB5D4_13D4F4(u16 id);

#endif
