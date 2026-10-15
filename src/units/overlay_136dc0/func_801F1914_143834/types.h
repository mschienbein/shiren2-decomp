/* OW1 slice s11 (overlay_136dc0, VRAM 0x801F17C0..0x801F2DC4): object views.
 * Every field lists the ROM accesses (VRAM of the instruction) that prove it.
 * C and C++ units both include this header (C++ inside extern "C"). */
#ifndef S11_TYPES_H
#define S11_TYPES_H
#include "common.h"
#include "record_views.h"
#ifdef __cplusplus
#include "ovlb_shared_views.h"
#endif
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* ---- Overlay class with vtable D_801F63A8_1482C8 (actor component at actor+0x78) ---- */

/* One-byte value type at Obj801F63A8+0x2D: passed by value left-justified in a1
 * (sra 24 at 0x801F2D68), returned through the hidden pointer a0 (0x801F2D74), and
 * built in a stack temporary by the constructor (sb 0($sp) at 0x801F2B10). */
typedef struct {
    u8 value;
} Byte801F2D68;

/* GCC 2.x vtable entry {s16 delta; s16 index; pfn}: delta lh at 0x801F18D4,
 * 0x801F2A50, 0x801F2A9C, 0x801F2C94, 0x801F2CEC; pfn lw at +4. */
typedef struct {
    s16 delta;
    s16 index;
    void (*pfn)(void *self, s32 size, void *buffer);
} VtEntry;

/* Serialisation stream (canonical func_800CA4A4/func_800CA4E8 view: vptr at +0x18).
 * Slot 3 (+0x18/+0x1C) writes, slot 5 (+0x28/+0x2C) reads. */
typedef struct {
    u8 pad_00[0x18];
    VtEntry *vptr; /* lw 0x18 at 0x801F2A30, 0x801F2A44, 0x801F2A90, 0x801F2C8C, 0x801F2CE4 */
} Stream;
extern void func_800CA4A4(Stream *stream, const char *tag);
extern void func_800CA4E8(Stream *stream, const char *tag);

/* Saved block at Obj801F63A8+0x18, 0x18 bytes: slot 3/5 transfer exactly 0x18 bytes
 * starting at +0x18 (a1 = 0x18, a2 = this + 0x18 at 0x801F2C88/0x801F2CE0). */
typedef struct {
    u16 field_00;          /* +0x18: sh $zero at 0x801F2AFC; main func_800EFAB4 reads kind18 */
    u8 packed;             /* +0x1A: sb at 0x801F2C84; lbu at 0x801F2CFC..0x801F2D3C */
    u8 pad_03;
    EventRect rect;        /* +0x1C..+0x2B: full rectangle, address at 0x801F2BC8 */
    u8 field_14;           /* +0x2C: sb 0x801F2B04, 0x801F2DA0; lbu 0x801F2D98 */
    Byte801F2D68 field_15; /* +0x2D: sb 0x801F2B14, 0x801F2D70; lbu 0x801F2D74 */
} Save801F2D68;

/* 0x34-byte class object (main func_800EFB74/func_800EFAB4: 0x34-byte base at actor+0x78,
 * vtable word at +0x30). vptr = D_801F63A8_1482C8 (sw at 0x801F2AD4); the vtable holds
 * 0x801F2E0C, 0x801F2C1C and 0x801F2CB8 (ROM words 0x801F63B4..0x801F63C4). */
typedef struct Obj801F63A8 {
    s32 field_00;         /* sw 0x801F2B00, 0x801F2D0C; lw 0x801F2B5C; lbu low byte 0x801F2C44 */
    s32 field_04;         /* sw 0x801F2AE8, 0x801F2D18; lbu low byte 0x801F2C40 */
    s32 field_08;         /* sw 0x801F2AEC, 0x801F2D50, 0x801F2DC0; lw 0x801F2DB8; lbu 0x801F2C68 */
    s32 field_0C;         /* sw 0x801F2AF0, 0x801F2D4C, 0x801F2DA8; lw 0x801F2DB0; lbu 0x801F2C4C */
    s32 field_10;         /* sw 0x801F2B0C, 0x801F2D2C, 0x801F2D88; lw 0x801F2D90; lbu 0x801F2C48 */
    s32 field_14;         /* sw 0x801F2B08, 0x801F2D38; lw 0x801F2BF0/0x801F2C04; lbu 0x801F2C64 */
    Save801F2D68 save;    /* +0x18..+0x2F */
    VtEntry *vptr;        /* +0x30 */
} Obj801F63A8;
extern VtEntry D_801F63A8_1482C8[];

/* ---- B .data scalars (ROM-zero, 0x801F5D00..0x801F5D11; .data ends 0x801F5D10) ---- */
extern s32 D_801F5D00_147C20;   /* sw 0x801F2260, 0x801F241C */
extern void *D_801F5D08_147C28; /* object pointer (main func_800EFAB4 passes the actor to
                          * func_801F216C_14408C, stored at 0x801F21CC); sw 0x801F2160,
                          * 0x801F22E4, 0x801F2414 */
extern u8 D_801F5D0C_147C2C;    /* sb 0x801F2148, 0x801F2158, 0x801F21C4 */
extern u16 D_801F5D0E_147C2E;   /* lhu 0x801F2130, 0x801F2184, 0x801F21FC, 0x801F2A14; sh 0x801F2140,
                          * 0x801F21BC, 0x801F240C; main func_80042014 reads it as u16 */
extern u16 D_801F5D10_147C30;   /* lhu 0x801F21E8, 0x801F2234, 0x801F234C, 0x801F2A24; sh 0x801F2244,
                          * 0x801F2258, 0x801F2304; main func_80042014/func_80108C2C: u16 */

/* B .data byte table at 0x801F5228 (ROM-zero): lbu indexed 0x801F19D0; [0] 0x801F2778;
 * [2] 0x801F25F8/0x801F269C/0x801F26C8/0x801F28C4/0x801F2970/0x801F29C4;
 * [3] 0x801F2628; [7] 0x801F263C/0x801F265C; [8] 0x801F2720. */
extern u8 D_801F5228_147148[];

/* ---- Main-image objects (canonical declarations) ---- */
extern signed char D_80140160[]; /* [4] lb 0x801F2198/0x801F221C; [6] lb 0x801F220C */

/* Canonical SelectionRecord D_80142F18: flags +3 = 0x80142F1B (lbu 0x801F28D8),
 * mode +8 = 0x80142F20, coordinates[0] +9 = 0x80142F21 (lb 0x801F1A70/0x801F206C).
 * Canonical SelectionSave D_80142F24: index +0 read lbu at 0x801F27E4. */
#ifdef __cplusplus
extern OvlBSelectionRecord D_80142F18;
extern OvlBSelectionSave D_80142F24;
#else
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05; unsigned short field_06; unsigned char mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;
#endif
extern u16 D_8014767C;           /* canonical u16; lhu at 0x801F2818 */
extern const u8 D_8015488C[8];   /* canonical; [0] 0x801F24A0, [6] 0x801F2478, [7] 0x801F245C */

/* Main 0x30-byte record object (canonical RecordObject D_80147680, returned by
 * func_800C9E10); bytes +7 and +8 read with lbu at 0x801F2454/0x801F2498. */
typedef struct {
    u8 pad_00[7];
    u8 flags7;
    u8 flags8;
    u8 pad_09[0x27];
} Record80147680;

/* Position pair: first 8 bytes of the player actor *D_801476B8 (lw 0/4 at 0x801F17F4/
 * 0x801F1800) and of a cell record (lw 0/4 at 0x801F1864/0x801F1868). */
typedef struct Vec801F17C0 {
    s32 x;
    s32 y;
#ifdef __cplusplus
    /* Member-wise copy constructor (lw/sw pairs at 0x801F17F4..0x801F1808, as canonical
     * func_800B7948's Vec2i); assignment stays the implicit bitwise copy (0x801F1864). */
    Vec801F17C0() {}
    Vec801F17C0(const Vec801F17C0 &other) : x(other.x), y(other.y) {}
#endif
} Vec801F17C0;
typedef struct {
    Vec801F17C0 pos;
} Unit801476B8;
extern Unit801476B8 *D_801476B8; /* canonical: pointer (void *, Unit *, Actor *) */

/* Cell vtable entry: slot 1 is called with (this, 3) only (a2 unset at 0x801F18CC). */
typedef struct CellVtEntry {
    s16 delta;
    s16 index;
    void (*pfn)(void *self, s32 event);
} CellVtEntry;

/* 0xE4-byte cell record returned by func_800A8CB0 (canonical Record data[0xE4]). */
typedef struct {
    Vec801F17C0 pos;      /* +0x00: lw at 0x801F1864/0x801F1868 */
    u8 pad_08[0x16];
    u8 flags_1E;          /* +0x1E: lbu at 0x801F1824, bit 4 */
    u8 pad_1F[5];
    struct CellVtEntry *vptr; /* +0x24: lw at 0x801F18D0; slot 1 = +0x8/+0xC */
    u8 pad_28[0x72];
    u16 flags_9A;         /* +0x9A: lhu at 0x801F183C (bits 0x40, 0x100, 0x200) */
    u8 pad_9C[0x48];
} Cell801F17C0;

/* Actor component views (canonical func_800DF40C: overlapping parts at +0x78 and
 * +0x84 of the actor; the +0x78 one is Obj801F63A8 for overlay actors). */
typedef struct {
    u8 pad_00[8];
    s32 state; /* +8: lw at 0x801F1F94 (+0x78 part) and 0x801F1FCC (+0x84 part) */
} Part801F1F64;
typedef struct {
    u8 pad_00[0x1E];
    u8 flags_1E; /* lbu at 0x801F1F74/0x801F1FAC: bit 6 = +0x78 part, bit 3 = +0x84 part */
    u8 pad_1F[0x59];
    union {
        Part801F1F64 part_78;
        struct {
            u8 pad_78[0xC];
            Part801F1F64 part_84;
        } inner;
    } parts;
} Actor801F1F64;

/* Prefix view of func_801F27DC_1446FC's argument: lbu +1 at 0x801F27F4, lb +5 at 0x801F2804. */
typedef struct {
    u8 field_00;
    u8 kind;
    u8 pad_02[3];
    s8 field_05;
} Arg801F27DC;

/* 0xC-byte placement record filled by func_801E5BE4_137B04(index, 0xC, out) (0x801F2054):
 * bytes 0..7 read with lbu/lb at 0x801F205C..0x801F20D4, halfword +8 at 0x801F20B4,
 * byte +0xA at 0x801F20C4. */
typedef struct {
    u8 y;          /* +0 */
    u8 x;          /* +1 */
    u8 field_02;   /* +2 */
    u8 req_id;     /* +3 */
    u8 req_low;    /* +4 */
    u8 req_high;   /* +5 */
    s8 floor;      /* +6: -1 = any (lb at 0x801F205C) */
    u8 pad_07;
    u16 field_08;  /* +8 */
    u8 field_0A;   /* +A */
    u8 pad_0B;
} Placement801F1FF4;

/* Item fields written at 0x801F20B8..0x801F20CC (canonical func_800AD7E0 item). */
typedef struct {
    u8 pad_00[0xC];
    u16 field_0C;
    u8 field_0E;
    u8 pad_0F;
    s32 field_10;
} Item801F1FF4;
typedef struct {
    s32 x;
    s32 y;
} Pos801F1FF4;

/* 0x16-byte event record filled by func_801E5BE4_137B04(index, 0x16, out) (0x801F1A54). */
typedef struct {
    u8 id;          /* +0: lbu 0x801F1AC0, 0x801F1AD0, 0x801F1AE4, 0x801F1B10 */
    s8 floor;       /* +1: lb 0x801F1A5C, -1 = any */
    EventCoordinates coordinates; /* +2: packed halfword and spawn y/x bytes */
    u8 pad_04;
    u8 req0_id;     /* +5..+7: lbu 0x801F1A7C..0x801F1A88 */
    u8 req0_low;
    u8 req0_high;
    u8 req1_id;     /* +8..+A: lbu 0x801F1A98..0x801F1AA4 */
    u8 req1_low;
    u8 req1_high;
    u8 pad_0B[0x0B];
} Event801F1A00;
typedef struct {
    u8 pad_00[0x52];
    u8 field_52;    /* sb at 0x801F1B28 */
} Spawn801F1A00;

/* ---- Main-image callees (canonical signatures where a unit exists) ---- */
extern void func_800A9C5C(s32 unused);
extern s32 func_800E776C(void *arg0, u8 arg1);
extern void func_800E039C(void *object, s32 value);
extern s32 func_800E6170(void *mover, void *map);
extern s32 func_80049CB4(s32 action, ...);
extern Record80147680 *func_800C9E10(void);
extern void func_800C96FC(void);
extern s32 func_800B74C0(s32 index);
extern void func_800B15C8(s32 kind);
extern void func_800B03F4(void);
extern void *func_800AC244(u8 id);
extern s32 func_800AFBFC(void *key);   /* canonical C: exactly 0 or 1 */
extern void func_800AD7E0(void *item, Pos801F1FF4 *pos, s32 notify);
extern void func_80094FA8(void *state);      /* called with D_80140160 (0x801F22F0) */
extern void func_800265E0(void *buffer, s32 size); /* bzero-like clear */
extern void *func_800A8CB0(s32 cell); /* canonical contract; caller narrows locally */
extern s32 func_80083D40(s32 x);
extern void *func_800A83D0(s32 arg, u8 flags);

/* ---- Image-local callees outside this slice (signatures from the call sites) ---- */
/* Canonical main callers establish these contracts (80041EF0/80041F28).
 * fixes/s07 owns the definition repair; unsigned indexing is a local operation. */
extern u8 func_801E9F70_13BE90(s32 flag);
extern void func_801E9F90_13BEB0(s32 flag, u8 value);
extern void func_801EA004_13BF24(void);
extern void func_801E5B50_137A70(s32 kind, u8 mode, u16 *first, u16 *last);
extern void func_801E5BE4_137B04(u32 index, u32 size, u8 *out);
extern u8 func_801E8CF4_13AC14(u8 kind, u8 value);
extern s32 func_801F2E30_144D50(s32 kind);
extern Spawn801F1A00 *func_801F1510_143430(Event801F1A00 *event, s32 arg1, s32 arg2);
extern void func_801F1B58_143A78(Spawn801F1A00 *spawn, Event801F1A00 *event);

/* ---- Slice functions called from other slice functions ---- */
extern void func_801F1914_143834(s32 arg0);
extern s32 func_801F1974_143894(u8 id, u8 low, u8 high);
extern void func_801F1A00_143920(s32 announce, s32 first, s32 last);
extern void func_801F1FF4_143F14(void);
extern void func_801F212C_14404C(u16 id, u8 mode);
extern void func_801F2254_144174(u16 id, s32 value);
extern s32 func_801F23AC_1442CC(u16 id);

#endif
