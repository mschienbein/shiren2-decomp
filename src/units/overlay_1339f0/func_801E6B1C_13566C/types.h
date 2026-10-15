#ifndef OW1_S02_TYPES_H
#define OW1_S02_TYPES_H
#include "common.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* ROM 0x134D98..0x134DA4: 32-byte stride, lw at +0. */
typedef struct {
    s32 value_00;
    u8 unknown_04[0x1C];
} OvlEntry20;

/* Base-menu accesses: func_800953C0 sw +0xC/+0x10/+0x48/+0x4C,
 * sb +0x46. Overlay constructors pass each actual embedded base address. */
typedef struct {
    u8 unknown_00[0xC];
    s32 value_0C;
    void *data_10;
    u8 unknown_14[0x32];
    u8 flag_46;
    u8 unknown_47;
    s32 value_48;
    const void *table_4C;
    u8 unknown_50[0xC];
} OvlMenu;

/* ROM 0x134E5C..0x134E68 and 0x134E7C..0x134E88:
 * table +0x4C, signed word +0x68, pointer +0x6C.
 * 0x10C member extent follows constructor bases +0x17C and +0x288. */
typedef struct {
    OvlMenu menu;
    u8 unknown_5C[0xC];
    s32 value_68;
    const void *data_6C;
    u8 unknown_70[0x9C];
} OvlDialog;

/* 0x98-byte final list: constructor base +0x288, next panel +0x320. */
typedef struct {
    OvlMenu menu;
    u8 unknown_5C[0x3C];
} OvlChoice;

/* 0x320-byte panel, proven by D_80141DB4's two panels (+0x64/+0x384).
 * ROM 0x134D8C: lw +0x16C; 0x134D78: sw +0x174.
 * Embedded bases at +0x5C/+0x17C/+0x288: func_801E62AC_134DFC. */
typedef struct {
    OvlMenu menu;
    OvlDialog first_05C;
    u8 unknown_168[4];
    OvlEntry20 *entries_16C;
    u8 unknown_170[4];
    s32 value_174;
    u8 unknown_178[4];
    OvlDialog second_17C;
    OvlChoice third_288;
} OvlOwner;

/* ROM 0x134C58/0x134CD8: panel bases +0x64/+0x384, extent 0x6A4
 * agrees with canonical D_80141DB4 in func_8009CE64. */
typedef struct {
    OvlMenu menu;
    u8 unknown_5C[8];
    OvlOwner panels[2];
} OvlGroup;

/* Opaque 24-byte save metadata: ROM 0x134FFC..0x1350B8 copies six
 * unaligned words from save+0xA to the 24-byte D_801E78A4_1363F4 record.
 * No scalar field widths are claimed; canonical Info also copies these bytes. */
typedef struct {
    u8 opaque[24];
} OvlSaveInfo;

/* Partial save-buffer view. ROM 0x1350C4..0x1350E8 reads four bytes
 * at +0x27F8..+0x27FB and reconstructs a big-endian word explicitly. */
typedef struct {
    u8 unknown_0000[0xA];
    OvlSaveInfo info_000A;
    u8 unknown_0022[0x27F8 - 0x22];
    u8 value_27F8[4];
} OvlSaveBuffer;

/* ROM 0x135188/0x135210/0x1352B0: operation +4, destination +0xC
 * are words. lbu +0xB/+0xF are low-byte conversions of slot words. */
typedef struct {
    u8 unknown_00[4];
    s32 operation_04;
    s32 source_08;
    s32 destination_0C;
} OvlSaveRequest;

/* Whole 0x30-byte RecordObject, matching canonical func_800CA008:
 * CA76C writes pointer +0x1C and byte +0x20; CB268 +0xA;
 * CB288/CB2D4/CA9A8 write words +0x24/+0x28/+0x2C. */
typedef struct {
    u8 unknown_00[9];
    u8 flags_09;
    s8 value_0A;
    u8 unknown_0B[0x11];
    void *child_1C;
    u8 kind_20;
    u8 unknown_21[3];
    s32 value_24;
    s32 value_28;
    s32 value_2C;
} OvlRecord;

/* ROM 0x13538C and 0x135398: paired byte start/count, four 2-byte rows.
 * ROM data 0x136990..0x136998 = 0B 1A 01 0A 25 0E 33 3A. */
typedef struct {
    u8 first;
    u8 count;
} OvlFontRun;
extern OvlFontRun D_801E7E40_136990[4];
/* Two 8-byte OR/AND mask groups, each containing four bytes per mask.
 * ROM 0x1354B8/0x1354C4 reads +0/+4; second group starts at +8. */
extern u8 D_801E7E48_136998[16];
/* ROM 0x13537C: four-byte compressed font reads, total table 0x550 bytes. */
extern u32 D_801E78F0_136440[0x154];
/* ROM 0x135600..0x135618: lw pointer, 32-byte glyph stride, lhu rows.
 * func_801E67D0_135320 clears 0x870 halfwords in the allocated font workspace. */
extern u16 *D_801E8284_136DD4;

/* OSPfsState matches resident func_8002E470: u32 size/game code, u16
 * company code, 4/16-byte encoded names. ROM 0x1357F4: 32-byte stride;
 * 0x1358BC lw +0; 0x135874/0x1358A4 lbu +0xA; 0x135858 address +0xE. */
typedef struct {
    u32 file_size;
    u32 game_code;
    u16 company_code;
    char ext_name[4];
    char game_name[16];
} OvlPakState;
extern OvlPakState D_801E84C0_137010[16];
/* ROM 0x135838..0x135844: 16 word statuses, mapping evidence ends at 0x801E8700. */
extern s32 D_801E86C0_137210[16];
/* Three strings (ROM 0x1369B8..0x1369C4), u16 lengths, s16 pixel offsets.
 * ROM 0x1358F8 lw, 0x135908 lhu, 0x135914 lh. */
extern const char *D_801E7E68_1369B8[3];
extern u16 D_801E7E74_1369C4[3];
extern s16 D_801E7E7C_1369CC[3];

/* ROM 0x135700: lw pointer; 0x135708 scales pixel indices by two.
 * Allocated framebuffer extent is 0x96000 bytes (func_801E77A4_1362F4). */
extern u16 *D_801E7E60_1369B0;
/* ROM 0x135728: sw pointer, 0x135764..0x135774: advance by count * 2. */
extern u16 *D_801E8280_136DD0;
#endif
