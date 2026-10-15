/* Overlay B C++ views shared by the s07, s11, s12, o1 and o4 families.
 * Unrelated main-image partial views keep their existing names; these names
 * distinguish native C++ views without changing extern "C" catalogue symbols.
 * Source reconstruction: OpenAI gpt-6-astra. */
#ifndef OVLB_SHARED_VIEWS_H
#define OVLB_SHARED_VIEWS_H
#include "common.h"
#include "record_views.h"
#include "script_operand.h"
struct Item;

/* D_801C9BD8, returned by func_800C9E00: grids at 0/0x14 (800D1BB0),
 * signed slots at 0xBB (800A1DAC; lb at overlay 801E9858). */
struct Game800C9E00 {
    unsigned char id[5][4];
    unsigned char extra[5][4];
    unsigned char pad_28[0xBB - 0x28];
    signed char slots[12];
};

struct MenuPos { s32 x; s32 y; };

/* The explicit g++ 2.x deleting-destructor slot at +8/+C is used by s07.
 * The native virtual-destructor view in s12 is a distinct C++ type. */
struct DestroyTable {
    unsigned char pad_00[8];
    short delta_08;
    short index_0A;
    void (*destroy_0C)(void *self, s32 flags);
};
struct OvlBItemDispatchView {
    unsigned char pad_00[8];
    DestroyTable *table_08;
};

/* Map position's member-wise copy preserves the observed lw/sw pairs.
 * Dir's mask and helpers are the existing s12 definitions, shared with o4. */
struct OvlBPos {
    s32 x;
    s32 y;
    OvlBPos() {}
    OvlBPos(const OvlBPos &o) : x(o.x), y(o.y) {}
};
struct OvlBDir {
    unsigned char v;
    OvlBDir() {}
    OvlBDir(s32 d) : v(d & 7) {}
    OvlBDir turned(s32 steps) const { return OvlBDir(v + steps); }
    bool operator==(const OvlBDir &o) const { return v == o.v; }
};
struct OvlBUnitVEntry {
    short delta;
    short index;
    void (*fn)(void *self, s32 flags);
};

/* The same 0x78-byte unit head in s12/o4 and func_801F1B58:
 * position 0, facing 8, kind A, flags 1E (801F1B74/801F3380),
 * table 24 (801F3AF8), state 52. The +78/+84 component bases remain
 * outside this head. s07's inventory-bearing +84/+8C view is distinct. */
struct OvlBUnitHead {
    /* 0x00 */ OvlBPos pos;
    /* 0x08 */ OvlBDir facing;
    /* 0x09 */ unsigned char pad09;
    /* 0x0A */ unsigned char unk0A;
    /* 0x0B */ unsigned char pad0B[0x1E - 0x0B];
    /* 0x1E */ unsigned char unk1E;
    /* 0x1F */ unsigned char pad1F[0x24 - 0x1F];
    /* 0x24 */ OvlBUnitVEntry *vt;
    /* 0x28 */ unsigned char pad28[0x52 - 0x28];
    /* 0x52 */ unsigned char unk52;
    /* 0x53 */ unsigned char pad53[0x78 - 0x53];
};
struct UnitPairs {
    OvlBUnitHead *units[30];
    OvlBUnitHead *targets[30];
};

/* One object behind D_801F51E8_147108, cleared as 0xA04 bytes at
 * 801EECD0..801EECD8. All three prior views describe disjoint regions:
 * pairs 18..108 (801F3550..801F3558), event args 108..118 (801F4680),
 * text 126..2A6, stack 2A6..2B0, ring 2B0..8F0, flags 8F0..990,
 * o4 flag 993 (801F36BC/801F36CC), then s07's flags and text offsets. */
struct Obj801F51E8 {
    unsigned char pad000[0x18];
    UnitPairs pairs;
    /* lw/lhu/lbu at 801EE0E0/801EE374/801EE158 read the same operand slots. */
    Operand args[4];
    Item *item118;             /* sw 801F1234; lw 801F1240. */
    unsigned char opaque_11C[2];
    unsigned short len11E;    /* lhu/sh 801F0648/801F0670. */
    unsigned char opaque_120[6];
    unsigned char text_126[0x180];
    unsigned char stack_2A6[10];
    unsigned char ring_2B0[160][10];
    unsigned char ring_flags_8F0[160];
    unsigned char opaque_990[3];
    unsigned char unk993;
    unsigned char opaque_994;
    unsigned char unk_995;
    unsigned char unk_996;
    unsigned char opaque_997[4];
    unsigned char unk_99B;
    unsigned char unk_99C;
    unsigned char opaque_99D;
    unsigned char unk_99E;
    unsigned char unk_99F;
    unsigned char opaque_9A0[0x28];
    s32 unk_9C8[10];
    unsigned char field9F0;   /* sb 801EF968/801EF9A4. */
    unsigned char opaque_9F1;
    unsigned char unk_9F2;
    unsigned char opaque_9F3[3];
    unsigned char field9F6;   /* sb 801F02F0. */
    unsigned char field9F7;   /* sb 801F0304. */
    unsigned char unk_9F8;
    unsigned char opaque_9F9;
    unsigned char unk_9FA;
    unsigned char field9FB;   /* sb 801F0740. */
    s32 field9FC;             /* sw 801F1440. */
    s32 unk_A00;
};

/* Shared 0x16-byte event/spawn description: floor is signed (801F1A5C),
 * +2 is the existing packed/y-x union; +4 is a raw byte (801F364C),
 * not a masked Dir construction. o4 writes facing.v at 801F45C0.
 * +C/+E and +10..14 are the settings consumed by func_801F1B58. */
struct Rec16 {
    unsigned char unk00;
    signed char floor;
    EventCoordinates coordinates;
    unsigned char unk04;
    unsigned char pad05[0x0C - 0x05];
    unsigned short unk0C;
    unsigned short unk0E;
    unsigned char unk10[5];
    unsigned char pad15;
};

/* Main selection state: 12 bytes, with a real unsigned halfword at +6
 * (lhu 800C73D0/800C7414). Overlay consumers read the other byte fields;
 * 801EC120..801EC14C copies the complete record, not separate +6/+7 values.
 * These overlay names remain distinct from the main C++ classes. */
struct OvlBSelectionRecord {
    unsigned char kind, variant, field_02, flags, row, field_05;
    unsigned short field_06;
    unsigned char mode;
    signed char coordinates[2], status;
};
struct OvlBSelectionSave {
    unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08;
    signed char result;
    unsigned char field_0A;
};
#endif
