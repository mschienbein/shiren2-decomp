/* Slice s12 (overlay_136dc0, 0x801F2DC4..0x801F5054) object views.
 *
 * Both B objects this slice touches are g++ 2.8.1 translation units:
 *   0x801F2AC0..0x801F2E30: vtable call through {s16 delta, s16 index, pfn}
 *     entries @801F2C8C..801F2C9C, struct return of a Dir via hidden pointer
 *     (func_801F2D74_144C94), Dir passed left-justified in a1 (func_801F2D68_144C88 `sra 24`);
 *   0x801F2E30..0x801F5110: vtable call @801F318C..801F3198, null-checked
 *     derived-to-base conversions (beqz unit / addiu 0x78|0x84), Dir(s32)
 *     constructor masks (andi 7).
 * So every source in this slice is C++ (gxx281pm-gnu291-O2-unsigned) with
 * extern "C" entry points. Base typedefs as in compiler-id/ovl_types.h. */
#ifndef S12_TYPES_H
#define S12_TYPES_H
#include "common.h"
#include "record_views.h"
#include "ovlb_shared_views.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

#define NULL 0

/* OvlBPos and OvlBDir share the same copy/masking helpers with o4;
 * ovlb_shared_views.h also owns the common unit head, state and event record. */

/* Record class of B object 0x801F2AC0 (constructor func_801F2AC0_1449E0) and base
 * subobject of the unit classes below. Field evidence:
 *   0x00 s32: sw 0x0 @801F2B00, sw @801F2D0C, lw @801F2B5C, sw a1 @801F2E18 (func_801F2E14_144D34)
 *   0x04 s32: sw @801F2AE8/801F2D18, lw @801F2E08 (func_801F2E04_144D24), sw @801F2E10 (func_801F2E0C_144D2C),
 *             lw base84+4 @801F33A0/801F4B2C
 *   0x08 s32: sw @801F2AEC/801F2D50, lw @801F2DB8, sw @801F2DC0
 *   0x0C s32: sw @801F2AF0/801F2D4C, sw @801F2DA8, lw @801F2DB0
 *   0x10 s32: sw @801F2B0C/801F2D2C, sw @801F2D88, lw @801F2D90, lw base78+0x10 @801F4C10
 *   0x14 s32: sw @801F2B08/801F2D38, lw @801F2BF0/801F2C04 (func_801F2BE8_144B08), sw @801F2E00 (func_801F2DFC_144D1C)
 *   0x18 u16: sh @801F2AFC, lhu @801F2E20 (func_801F2E1C_144D3C), sh @801F2E28 (func_801F2E24_144D44)
 *   0x1A u8:  sb @801F2C84, lbu @801F2CFC/801F2D10/801F2D1C/801F2D30/801F2D3C
 *   0x1C s32: sw @801F2AF4/801F2B2C, address taken @801F2BC8
 *   0x20 s32: sw @801F2AF8/801F2B28
 *   0x24 s32: sw @801F2ADC/801F2B44
 *   0x28 s32: sw @801F2AE0/801F2B38
 *   0x2C u8:  sb @801F2B04/801F2DA0, lbu @801F2D98
 *   0x2D Dir: sb @801F2B14/801F2D70, lbu @801F2D74 (returned by hidden pointer),
 *             lbu base78+0x2D @801F4C84/801F4CAC, base84+0x2D @801F4DE8
 *   0x30 ptr: sw &D_801F63A8_1482C8 @801F2AD4 */
struct Rec801F2AC0 {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u16 unk18;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 pad1B;
    /* 0x1C..0x2B */ EventRect rect;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ OvlBDir unk2D;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ void *unk30;

    /* func_801F2D74_144C94 is this accessor out of line (struct return via a0). */
    OvlBDir home() const { return unk2D; }
};


/* Unit kind flagged by bit 6 of 0x1E: Rec801F2AC0 base at +0x78
 * (beqz unit / addiu 0x78 @801F4C04..801F4C0C, @801F4D80..801F4D88). */
struct UnitX : OvlBUnitHead, Rec801F2AC0 {
};

/* Unit kind flagged by bit 3 of 0x1E: Rec801F2AC0 base at +0x84
 * (beqz unit / addiu 0x84 @801F3394..801F339C, @801F4B20..801F4B28,
 * @801F4DBC..801F4DC4). The 0xC bytes between are not accessed here. */
struct UnitHead84 : OvlBUnitHead {
    /* 0x78 */ u8 pad78[0x0C];
};

struct UnitY : UnitHead84, Rec801F2AC0 {
};


/* Item object returned by main func_800AC244: g++ vptr at +8 (lw 8(item)
 * @801F318C), deleting virtual destructor in vtable slot 1 called with 3
 * (lh 8 / lw 0xC of the vtable @801F3190..801F3198). Canonical C views use
 * the same layout (src/units/main_14400/func_800DF9F0.c Item { pad0[8]; vt; }). */
struct OvlBVirtualItemHead {
    /* 0x00 */ u8 pad00[8];
};

struct OvlBVirtualItem : OvlBVirtualItemHead {
    virtual ~OvlBVirtualItem();
};

/* Object behind the .data pointer D_801F51EC_14710C.
 *   0x04 s32 counter: lw/sw +1 @801F4644/801F4658, @801F4864/801F4878 */
struct Obj801F51EC {
    /* 0x00 */ u16 id; /* sh @801EB618 */
    /* 0x02 */ u8 opaque02[2];
    /* 0x04 */ s32 unk04;
};


/* Canonical accessor shape (src/units/main_14400/func_800AA944.c); the
 * pointer parameter keeps the mode read on its own lui (@801F4ED8). */
static inline u8 selection_mode(const OvlBSelectionRecord *record) { return record->mode; }

extern "C" {
extern OvlBSelectionRecord D_80142F18;
extern u16 D_801F5D0E_147C2E;
extern Obj801F51E8 *D_801F51E8_147108;
extern OvlBUnitHead *D_801476B8;
void func_800265E0(void *dst, s32 length);
s32 func_80049CB4(s32 id, ...);
OvlBUnitHead *func_800A8CB0(s32 cell);
s32 func_800A8974(OvlBUnitHead *unit);
void func_800A665C(OvlBUnitHead *unit, OvlBDir *direction);
/* s06 producer returns a word; byte consumers narrow locally (ROM andi 0xFF). */
s32 func_801E8D0C_13AC2C(u16 id, u16 selector);
/* Both producers return s32, restricted to 0/1: canonical 800A99D0 returns
 * 1 or a logical expression; s11 801F2BE8 returns only literal 0/1
 * (ROM 801F2BE8..801F2C18). This supports integer XOR negation, not bool ABI. */
s32 func_800A99D0(void);
s32 func_801F2BE8_144B08(Rec801F2AC0 *rec, s32 arg1);
s32 func_801F49BC_1468DC(OvlBUnitHead *unit, OvlBUnitHead *target);
}

/* Unit lookup by 16-bit id: func_801E8D0C_13AC2C maps the id to a cell, then main's
 * func_800A8CB0 returns the record. Inlined; its return value lands in a0
 * before the caller copies it (801F48CC/801F48D4). */
static inline OvlBUnitHead *unit_of(u16 id)
{
    return func_800A8CB0((u8)func_801E8D0C_13AC2C(id, 0));
}

#endif
