/* OW2 o4 (overlay_136dc0: func_801F3664_145584, func_801F3B5C_145A7C) object views.
 *
 * Both functions sit in B object 0x801F2E30..0x801F5110, a g++ 2.8.1 translation unit (slice s12's
 * finding: vtable calls, null-checked derived-to-base conversions, Dir(s32) masks). Evidence in these two
 * functions: the deleting virtual destructor call through vtable slot 1 with flag 3 after a null check
 * (@801F3AF0..801F3B04), and a loop the C front end would rotate but g++ 2.8.1 keeps test-first
 * (@801F3A80). So the sources are C++ (gxx281pm-gnu291-O2-unsigned) with extern "C" entry points.
 *
 * Names and shapes follow slice s12's header (scratch/omp/overlays/wave1/s12/types.h) where the same
 * object is viewed; additions are marked. Every field cites the ROM accesses (overlay_136dc0 VRAM). */
#ifndef O4_TYPES_H
#define O4_TYPES_H
#include "common.h"
#include "record_views.h"
#include "ovlb_shared_views.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef float f32;

#define NULL 0

/* OvlBPos, OvlBDir, OvlBUnitHead, Obj801F51E8 and Rec16 have one
 * definition shared with s12. OvlBUnitHead includes both s12's flags at
 * +0x1E and this family's explicit destructor table at +0x24. */

/* Event script behind D_801F51EC_14710C: ID is stored with sh @801EB618 (ROM 0x13D538),
 * cursor with sw @801EB61C. +2..+3 are opaque. Cursor +4 is the next byte offset
 * (lw/sw 0x4 around every func_801E53A4_1372C4 call, e.g. @801F36E0/801F36F0). */
struct EventScript {
    /* 0x0 */ u16 id;
    /* 0x2 */ u8 opaque02[2];
    /* 0x4 */ s32 pos;
};


extern "C" {
extern Obj801F51E8 *D_801F51E8_147108;
extern EventScript *D_801F51EC_14710C;
/* Current event opcode (lbu @801F3668/801F3BD0). */
extern u8 D_801F5CCD_147BED;

s32 func_80049CB4(s32 id, ...);
u8 func_800A8C00(OvlBUnitHead *unit);
OvlBUnitHead *func_800A8CB0(s32 cell);
s32 func_800A8974(OvlBUnitHead *unit);
void func_800A58FC(OvlBUnitHead *unit, OvlBPos *pos);
void func_800A59A4(OvlBUnitHead *unit);
void func_800A665C(OvlBUnitHead *unit, OvlBDir *direction);
void func_800B5260(OvlBPos *pos, u8 arg1);
s32 func_800B5650(OvlBPos *pos);
void func_800E039C(OvlBUnitHead *unit, s32 value);
void func_800E5F38(OvlBUnitHead *unit, OvlBPos *to);
void func_800E5F5C(OvlBUnitHead *unit, OvlBPos *to);
u8 func_801E53A4_1372C4(s32 offset);
/* wave3/s03's selected halfword contract; recovered callers pass 0 or 255.
 * s16 versus u16 is not uniquely recoverable from those constants. */
void func_801E5CC0_137BE0(u8 id, s16 arg1, s16 alpha);
void func_801E61B8_1380D8(s32 slot, s32 arg1, s32 arg2, s32 arg3);
void func_801E6394_1382B4(s32 slot, s32 arg1);
/* Full-word record index; third argument is supplied @801F4200 but unused. */
void func_801E65EC_13850C(s32 id, s32 arg1, s32 unused_kind);
void func_801E6708_138628(s32 slot, s32 arg1, s32 arg2, s32 arg3);
void func_801E6888_1387A8(s32 id, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_801E69E8_138908(u8 value);
void func_801E6A8C_1389AC(u8 id);
void func_801E6B4C_138A6C(s32 slot);
s32 func_801E6C44_138B64(s32 model, s32 dir, s32 arg2, s32 arg3, f32 *at);
void func_801E6D08_138C28(s32 arg0, s32 dir, s32 arg2, s32 arg3, f32 *at, s32 slot);
void func_801E7074_138F94(s32 slot, s32 from, s32 to, s32 speed);
void func_801E70D8_138FF8(s32 id);
void func_801E7200_139120(s32 id, s32 y0, s32 x0, s32 y1, s32 x1);
void func_801E72BC_1391DC(s32 slot, s32 arg1, s32 arg2, s32 dy, s32 dx, s32 arg5, s32 model);
void func_801E747C_13939C(s32 slot);
void func_801E75CC_1394EC(s32 slot);
void func_801E7610_139530(s32 id, s32 arg1, s32 arg2, s32 arg3);
void func_801E7A28_139948(s32 slot);
void func_801E7AA0_1399C0(s32 kind, s32 mode, s32 arg2);
void func_801E7C88_139BA8(s32 model, s32 arg1, s32 id, s32 arg3, s32 arg4, f32 *at);
/* Full-word index return @801E8D78/801E8D94; both inputs masked to u16.
 * Caller byte uses narrow locally, matching fixes/s07's public contract. */
s32 func_801E8D0C_13AC2C(u16 id, u16 selector);
void func_801EB6A0_13D5C0(u8 index);
OvlBUnitHead *func_801F1510_143430(Rec16 *desc, s32 count, s32 arg2);
void func_801F1B58_143A78(OvlBUnitHead *unit, Rec16 *desc);
s32 func_801F2E30_144D50(s32 value);
s32 func_801F2E80_144DA0(s32 value);
void func_801F2E8C_144DAC(OvlBUnitHead *unit);
void func_801F33FC_14531C(void);
void func_801F349C_1453BC(OvlBUnitHead *unit, OvlBUnitHead *target);
void func_801F34D8_1453F8(OvlBUnitHead *unit, s32 unitOnly);
void func_801F360C_14552C(const Operand *args, Rec16 *out);
s32 func_801F49BC_1468DC(OvlBUnitHead *unit, OvlBUnitHead *target);
}

/* Next script byte. A function-like macro, not an inline function: the u8 result must reach each use
 * unextended (an inline function's u8 return value is zero-extended at the call, which reorders the
 * 16-bit reads @801F3714..801F3728, @801F3F6C..801F3F7C and @801F4254..801F4268). */
#define SCRIPT_BYTE() func_801E53A4_1372C4(D_801F51EC_14710C->pos++)

/* Unit at the cell of a 16-bit unit id (s12's unit_of; inlined, @801F3D78..801F3D88). */
static inline OvlBUnitHead *unit_of(u16 id) {
    return func_800A8CB0((u8)func_801E8D0C_13AC2C(id, 0));
}

#endif
