#ifndef OW1_S10_TYPES_H
#define OW1_S10_TYPES_H
#include "common.h"
#include "record_views.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* Partial receiver view: ROM 0x14168C (801EF76C) loads a word at +0x64;
 * ROM 0x1416A8 (801EF788) stores the base-menu vtable pointer at +0x4C. */
typedef struct S10Menu {
    u8 pad00[0x4C];
    const void *vtable4C;
    u8 pad50[0x14];
    s32 value64;
} S10Menu;

/* Script operands are accessed as words and subwords. ROM 0x1410B0
 * stores +0x108; 0x1410B8 stores +0x10C; 0x142078 reads lhu +0x10E.
 * ROM 0x13FFEC/0x13FFF4 reads bytes +0x10B/+0x10F; 0x13CA00/04
 * reads words +0x110/+0x114. Only this proven 16-byte operand prefix is
 * typed: +0x118 contains a separately used pointer (ROM 0x140564).
 * These union views describe actual overlapping accesses, not layout steering. */
typedef union S10Operands {
    s32 words[4];
    u16 halves[8];
    u8 bytes[16];
} S10Operands;

/* Complete 0xA04-byte D_801F5288_1471A8 storage, bzero at ROM 0x140BF4..0x140BF8.
 * +0x120: lhu/sh at 0x1408D8/0x1410BC.
 * +0x990/+0x991: sb at 0x141080/0x14108C; +0x993: lbu/sb 0x140D60/6C.
 * +0x996/+0x997: sb 0x140C94/0x140CA8; +0x9F0: lbu 0x140EFC.
 * +0x9F5: lbu 0x1410B4; +0x9F8: sb 0x1412C4; +0x9F9: lbu 0x14124C;
 * +0x9FB: sb/lbu 0x140C0C/0x140D64; +0x9FC: sw/lw 0x140938/0x141354.
 * Unexamined regions stay opaque and do not assert scalar field widths. */
typedef struct S10EventState {
    u8 pad00[0x108];
    S10Operands operands108;
    u8 pad118[8];
    u16 field120;
    u8 pad122[0x86E];
    u8 field990;
    u8 field991;
    u8 pad992;
    u8 field993;
    u8 pad994[2];
    u8 field996;
    u8 field997;
    u8 pad998[0x58];
    u8 field9F0;
    u8 pad9F1[4];
    u8 field9F5;
    u8 pad9F6[2];
    u8 field9F8;
    u8 flag9F9;
    u8 pad9FA;
    u8 field9FB;
    s32 field9FC;
    u8 padA00[4];
} S10EventState;

/* D_801F5C8C_147BAC is an array of 8-byte frames: indexed by depth*8 at ROM
 * 0x13D500..0x13D538; id uses sh +0, cursor uses sw +4. The initialized
 * interval extends to D_801F5CCC_147BEC (8 frames), not just the first cleared frame. */
typedef struct S10ScriptFrame {
    u16 id;
    u8 pad02[2];
    s32 cursor;
} S10ScriptFrame;


/* Actor accesses: ROM 0x140988..0x1409F0 reads the +0x24 vtable,
 * signed adjustment +0x10, callable +0x14, and flags byte +0x1E.
 * ROM 0x140B7C reads the actor id byte at +8. The vtable slot's
 * canonical contract is s32 (void *), e.g. func_800A00C4. */
typedef struct S10ActorVTable {
    u8 pad00[0x10];
    s16 delta10;
    s16 index12;
    s32 (*call14)(void *);
} S10ActorVTable;
/* ROM 0x140AA8/0x140AB4 copies the two position words at actor +0/+4. */
typedef struct S10Position {
    s32 x;
    s32 y;
} S10Position;
typedef struct S10Actor {
    S10Position position;
    u8 field08; /* lbu at ROM 0x140B7C; also byte input of func_800A665C */
    u8 pad09;
    u8 kind0A; /* lbu at ROM 0x143648 */
    u8 pad0B[0x13];
    u8 flags1E;
    u8 pad1F[5];
    S10ActorVTable *vtable24;
    u8 pad28[0x50];
} S10Actor;

/* ROM 0x143694..0x1436B0: {s16 adjustment, s16 index, callable}
 * at vtable +8. D_801F63A8_1482C8+0xC resolves to func_801F2E0C_144D2C and
 * D_80159130+0xC to func_800EEB50: both void (void *, s32). */
typedef struct S10ComponentMethod {
    s16 delta;
    s16 index;
    void (*call)(void *, s32);
} S10ComponentMethod;
typedef struct S10ComponentTable {
    u8 pad00[8];
    S10ComponentMethod method08;
} S10ComponentTable;

/* Nullable subtype adjustment at ROM 0x140B28..0x140B58 chooses +0x78
 * or +0x84, then reads the byte at component +0x2C. These are alternative
 * actor subclasses selected by flags, not overlapping live members.
 * Component +0: sw at ROM 0x14367C; +0x18: sh at 0x143668;
 * +0x30: lw at 0x14368C. Full 0x34-byte base confirmed by func_801F2AC0_1449E0. */
typedef struct S10ActorComponent {
    s32 field00;
    u8 pad04[0x14];
    u16 field18;
    u8 packed1A;
    u8 pad1B;
    EventRect rect; /* +0x1C..+0x2B; same subobject as s11/s12 */
    u8 field2C;
    u8 pad2D[3];
    S10ComponentTable *vtable30;
} S10ActorComponent;
typedef struct S10ActorA {
    S10Actor base;
    S10ActorComponent component78;
} S10ActorA;
typedef struct S10ActorB {
    S10Actor base;
    u8 pad78[0xC];
    S10ActorComponent component84;
} S10ActorB;

/* Bit-4 actor subtype: ROM 0x140A88 loads a halfword at +0x9A. */
typedef struct S10ActorC {
    S10Actor base;
    u8 pad78[0x22];
    u16 flags9A;
} S10ActorC;

/* Stream receiver: lw +0x18 at ROM 0x14146C/0x141538.
 * The write entry is lh +0x18, lw +0x1C; read is lh +0x28, lw +0x2C.
 * Exact void (void *, s32, void *) slots follow func_80046294/800462E8. */
typedef struct S10StreamVTable {
    u8 pad00[0x18];
    s16 writeDelta18;
    s16 writeIndex1A;
    void (*write1C)(void *, s32, void *);
    u8 pad20[8];
    s16 readDelta28;
    s16 readIndex2A;
    void (*read2C)(void *, s32, void *);
} S10StreamVTable;
typedef struct S10Stream {
    u8 pad00[0x18];
    S10StreamVTable *vtable18;
} S10Stream;

/* Shared 0x16-byte event/spawn record. ROM 0x143478/0x1435B0/0x1435B8/
 * 0x143634 reads +0/+2/+3/+4; s11 reads the overlapping +2 halfword. */
typedef struct S10SpawnCommand {
    u8 kind;
    s8 floor;
    EventCoordinates coordinates;
    u8 direction;
    u8 tail05[0x11];
} S10SpawnCommand;

/* Same 12-byte selection record as canonical func_800C96FC.
 * ROM 0x143464/0x1434BC reads its byte +8 (former D_80142F20). */
typedef struct S10Selection {
    u8 kind, variant, field02, flags, row, field05;
    u16 field06;
    u8 mode;
    s8 coordinates[2], status;
} S10Selection;

/* Canonical func_800A9520 serializes 11 bytes at D_80142F24.
 * ROM 0x140F24..0x140F30 reads index/count, not independent globals. */
typedef struct S10SelectionSave {
    u8 index, count, previous, field03, masks[2], field06, previousCount, field08;
    s8 result;
    u8 field0A;
} S10SelectionSave;

#endif
