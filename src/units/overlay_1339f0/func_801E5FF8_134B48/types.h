/* OW1 slice s01: overlay_1339f0 (A), functions 0x801E4EA0..0x801E6030.
 * One header for every object view used by this slice. Each field lists the
 * original instructions (VRAM) that establish its offset and width.
 * Objects are named by address; no meaning is claimed beyond the cited accesses.
 */
#ifndef S01_TYPES_H
#define S01_TYPES_H
#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* ---- A .data ---------------------------------------------------------- */

/* D_801E7830_136380: u8 slot map, 3 entries. 0xFF = empty.
 *   lbu table[i]: 0x801E4F40/4F58, 0x801E4FA4, 0x801E5034, 0x801E5074/5094
 *   sb 0xFF to [2],[1],[0]: 0x801E5128 loop; sb i to [v-1] (D_801E782F_13637F+v): 0x801E5194
 *   lbu [0..2] (i < 3): 0x801E5C84, 0x801E5D48 */
extern u8 D_801E7830_136380[3];

/* D_801E7840_136390: u32 status code (PFS error 0..13, 0x65 read, 0x66 write, 0x67 delete).
 *   lw/sltiu 0x65: 0x801E578C; lw/sltiu 0x69 (switch): 0x801E59A8
 *   sw: 0x801E5290, 0x801E5300, 0x801E53CC, 0x801E54BC, 0x801E55F0, 0x801E5684,
 *       0x801E58BC, 0x801E5974/5980; lw 0x801E5990.
 * Local-definition scheduling is measured, but the five separate candidate
 * definitions are alternatives, not five owners. One joint-TU owner is pending. */
extern u32 D_801E7840_136390;

/* ---- A .rodata (string/code constants, contents from ROM) -------------- */

/* 0x801E7EA4..0x801E7EC4: four 8-byte pak extension names, defined (static) in the
 * func_801E4EA0_1339F0 TU: local definition proven by the gas j-delay-slot fill (see results). */
/* 0x801E7EC8..0x801E7EEA: "contpak" (7EC8), "01" (7ED0), "NSIJ" (7ED4)
 * and the 13-character encoded game name (7EDC, 14 bytes with NUL).
 * Local code-literal scheduling is measured; contpak attribution is inferred.
 * An extern game-name source also matches func_801E58EC_13443C, so its ownership is
 * unresolved. Joint-TU integration must provide one definition, not duplicates. */

/* ---- A .bss ----------------------------------------------------------- */

/* D_801E8290_136DE0: 0x12000-byte "contpak" heap block (sw 0x801E524C/5308, lw 0x801E52A0/52C0/537C/5448/5474/566C/56C8) */
extern u8 *D_801E8290_136DE0;
/* D_801E8294_136DE4: current half of that block (sw 0x801E5268/5480/5570/56D4, lw 0x801E52EC/55B0/5744/57A8) */
extern u8 *D_801E8294_136DE4;

/* D_801E8298_136DE8: 16-byte pak file handle; bzero(.., 0x10) at 0x801E527C.
 *   +0x0 slot pointer: written by func_80131570 (canonical Obj80131570.slot)
 *   +0x4 file index:   sw 0x801E5830
 *   +0x8 result:       lw 0x801E5358/542C/551C/5720/5834, lw a0 0x801E538C/545C/554C/5754
 *                      (canonical func_80130EE0 Obj.result, func_80130FE0 field08)
 *   +0xC enabled:      lw 0x801E57F8, 0x801E5960 (canonical func_80130EE0 Obj.enabled) */
typedef struct {
    void *slot;
    s32 index;
    s32 result;
    s32 enabled;
} PakFile801E8298;
extern PakFile801E8298 D_801E8298_136DE8;

/* D_801E82A8_136DF8: 16 x 32-byte file-state records; bzero(.., 0x200) at 0x801E5270.
 *   +0x0 u32 size:          lw 0x801E541C/5450, 0x801E550C/5540
 *   +0x4 u32 game code:     lw -6(s0) 0x801E585C; address 0x801E5624
 *   +0x8 u16 company code:  lhu -2(s0) 0x801E5840; address 0x801E561C
 *   +0xA ext name[4]:       address 0x801E5418/5508/5634; lbu 0x801E5884
 *   +0xE game name[16]:     address 0x801E5414/5504/562C/5878 (ends at +0x1E)
 *   stride 0x20: sll 5 at 0x801E53C4/54B4/55E8; bzero 0x20 at 0x801E589C */
typedef struct {
    u32 size;
    u32 game_code;
    u16 company_code;
    char ext_name[4];
    char game_name[16];
} PakState801E82A8;
extern PakState801E82A8 D_801E82A8_136DF8[16];

/* D_801E84A8_136FF8: 17-byte game name buffer; bzero(.., 0x11) at 0x801E5294; 14-byte copy 0x801E58EC */
extern char D_801E84A8_136FF8[17];

/* ---- list windows (receivers of 0x801E5A30..0x801E6030) ------------------ */

/* 12-byte menu entry; ROM rows at D_801E7850_1363A0 read {0x044E0000,0,0}, {0x044F0000,0,1},
 * {0x05070000,0,2}, ... (u16 message id, then the entry value).
 *   copied as three words: 0x801E5C94..5CA8, 0x801E5CEC..5D00, 0x801E5D58..5D6C
 *   +0x0 u16 id: canonical func_800972D0 Entry.id (reached through func_801E5FDC_134B2C)
 *   +0x8 value:  lw 8(v0) at 0x801E6048 */
typedef struct {
    u16 id;
    u8 pad2[2];
    s32 field4;
    s32 value;
} MenuEntry801E5BF4;

/* D_801E7850_1363A0: MenuEntry table; [0..1] read at 0x801E5CEC, [2..4] at 0x801E5C94/0x801E5D58 */
extern MenuEntry801E5BF4 D_801E7850_1363A0[5];

/* 32-byte item record: stride sll 5 at 0x801E5B14, 0x801E5DC0; addiu 0x20 at 0x801E5FBC
 *   +0x0 s32 value:  lw 0x801E5DEC (printed +1), lw 0x801E5F98/5FA8 (flag index)
 *   +0x6 packed text: func_80083F34(.., 4, ..) source at 0x801E5DCC */
typedef struct {
    s32 value;
    u8 pad4[2];
    u8 text[4];
    u8 padA[0x16];
} Item801E5A30;

/* 16-byte layout: four s32 fields, reusing canonical func_8009C440
 * Arg_8009C440 widths; canonical func_8009A930/func_8009CE64 Layout names them
 * columns, width, x, y. Copy at 0x801E5A70..5AB4; D_80138EE0.unk0 store
 * at 0x801E5DA0. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Layout801E5A30;

/* 8-byte selection: canonical func_8009A930/func_8009CE64 Selection widths.
 * D_80138EF0.count store at 0x801E5DA8. */
typedef struct {
    s32 count;
    s32 width;
} Selection801E5BF4;

/* 8-byte cursor at list +0x34: lw 0x801E5B60/5B64 (canonical func_80097418 S.x34 Pair) */
typedef struct {
    s32 offset;
    s32 extra;
} Cursor801E5B54;

/* Method table at +0x4C: g++-2 style {s16 delta; s16 index; fn} slots.
 *   +0x68/+0x6C: lh/lw 0x801E5E84/5E88 (also 5EA8/5EAC, 5ED4/5ED8)
 *   +0x80/+0x84: lh/lw 0x801E5BD0/5BD4 */
typedef struct {
    u8 pad0[0x68];
    s16 delta_68;
    s16 index_6A;
    s32 (*method_6C)(void *self, s32 arg);
    u8 pad70[0x10];
    s16 delta_80;
    s16 index_82;
    void (*method_84)(void *self, Cursor801E5B54 *cursor);
} Methods801E5A30;

/* Menu embedded at list +0x288 (0x5C bytes up to +0x2E4).
 *   +0x50 entries: lw 0x50(a0) at 0x801E6038 (canonical func_80097240 Menu.field_50) */
typedef struct {
    u8 pad0[0x50];
    MenuEntry801E5BF4 *entries;
    u8 pad54[8];
} Menu801E6030;

/* 0x320-byte list window (embedded at owner +0x64 and +0x384: 0x384 - 0x64 = 0x320).
 *   +0x34 cursor:      lw 0x801E5B60/5B64
 *   +0x4C methods:     lw 0x801E5BC8
 *   +0x16C items:      sw 0x801E5AB8; lw 0x801E5B20, 0x801E5DBC/5DE0
 *   +0x170 count:      sw 0x801E5ABC; lw 0x801E5B08, 0x801E5B88/5BB8
 *   +0x174 mode:       lw 0x801E5AF8, 0x801E5C20; sw 0x801E5EE8
 *   +0x178 u8 flags[2]: lbu 0x801E5CE0 (i < 2); sb 0x801E5F74/5F78, 0x801E5FA4/5FB0
 *   +0x17C text box:   func_8009D610/func_8009D6E4 receiver 0x801E5E20
 *   +0x288 menu:       func_80097240 receiver 0x801E5CC4/5D84
 *   +0x2E4 entries[5]: sw 0x2E4/2E8/2EC(+12n) 0x801E5CA0, 0x801E5CF8, 0x801E5D64; n <= 2 + 3 */
typedef struct {
    u8 pad0[0x34];
    Cursor801E5B54 cursor;
    u8 pad3C[0x10];
    Methods801E5A30 *methods;
    u8 pad50[0x11C];
    Item801E5A30 *items;
    s32 count;
    s32 mode;
    u8 flags[2];
    u8 pad17A[2];
    u8 text_box[0x10C];
    Menu801E6030 menu;
    MenuEntry801E5BF4 entries[5];
} List801E5A30;

/* Owner of two lists (receiver of 0x801E5E68 / 0x801E5F0C).
 *   +0x4C methods: lw 0x801E5E80/5EA4/5ED0
 *   +0x64 list:    addiu 0x801E5EBC, 0x801E5F48; flags 0x1DC = 0x64 + 0x178
 *   +0x384 list:   addiu 0x801E5EE4, 0x801E5F38; flags 0x4FC = 0x384 + 0x178 */
typedef struct {
    u8 pad0[0x4C];
    Methods801E5A30 *methods;
    u8 pad50[0x14];
    List801E5A30 list64;
    List801E5A30 list384;
} Owner801E5E68;

/* ---- main-image data used by this slice ----------------------------------- */
extern s32 D_80138E88;                  /* lw 0x801E5F58 (canonical func_8009C440: extern s32) */
extern s32 D_80138E9C[2];              /* address 0x801E5E30; map extent 8 bytes {9, 0xB} */
extern s32 D_80138EA4[4];              /* address 0x801E5E28; map extent 16 bytes {4, 0x16, 0xE, 8} */
/* Accepted main_data_10bca0 range [0x80138ECC, 0x80138EF8), ROM [0x10BF0C, 0x10BF38),
 * as four separate objects with disjoint consumers (no canonical C declaration):
 * - D_80138ECC [ECC,EDC): 16-byte rectangle {8, 0x1C, 0xE, 6}; main func_8009C61C
 *   (0x8009C7F4) passes &D_80138ECC to func_8009D610, which copies it as a Rect of
 *   two s32 pairs.
 * - D_80138EDC [EDC,EE0): s32 style 0x1D; lw 0x801E5F34, passed as s32 to func_801E5A30_134580.
 * - D_80138EE0 [EE0,EF0): Layout {5, 0x12, 0xB, 0xE}; address 0x801E5D90/5D94, sw unk0 0x801E5DA0.
 * - D_80138EF0 [EF0,EF8): Selection {5, 0}; address 0x801E5D98/5D9C, sw count 0x801E5DA8. */
typedef struct {
    s32 x;
    s32 y;
} Pair80138ECC;
typedef struct {
    Pair80138ECC origin;
    Pair80138ECC end;
} Rect80138ECC;
extern Rect80138ECC D_80138ECC;
extern s32 D_80138EDC;
extern Layout801E5A30 D_80138EE0;
extern Selection801E5BF4 D_80138EF0;

#endif
