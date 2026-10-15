#ifndef OW1_S06_TYPES_H
#define OW1_S06_TYPES_H
#include "common.h"
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

/* +34/+38 lw pair at 801E8BA8/AC; canonical widget position is two s32. */
typedef struct { s32 x, y; } S06Point;
/* Widget table D_801F6060_147F80: +68 lh adjustment and +6C lw function,
 * called at 801E8C70/78. +80 lh/+84 lw at 801E8C3C/40 invokes the
 * position setter (canonical func_80095D64). */
typedef struct {
    u8 pad00[0x68];
    s16 delta68;
    s16 index6A;
    s32 (*call6C)(void *self, s32 index);
    u8 pad70[0x10];
    s16 delta80;
    s16 index82;
    void (*call84)(void *self, S06Point *pos);
    u8 pad88[0xC]; /* D_801F6060_147F80 ends at 801F60F4. */
} S06WidgetTable;
/* Partial receiver view, not an allocation declaration. +4C lw at 801E8C6C;
 * +20 lw at 801E8BD4; +34/+38 at 801E8BA8/AC; +5C lw at 801E8CD0;
 * +60 lw/sw at 801E8C1C/801E8B74; +64 sw at 801E8CBC/8CE8. */
typedef struct {
    u8 pad00[0x20];
    s32 field20;
    u8 pad24[0x10];
    S06Point point34;
    u8 pad3C[0x10];
    const S06WidgetTable *table4C;
    u8 pad50[0xC];
    s32 field5C;
    s32 field60;
    s32 field64;
} S06Widget;
/* Partial pointed-to event image; never defines its allocation. +120 lhu
 * at 801E8EE4, +995 sb at 801E8F84, +9F7 lbu at 801E8F64.
 * +9C8 lw indexed by 4 at 801E8C94; ten word offsets end before the
 * byte fields at +9F0 (801EEFDC lbu). Offsets are relative to +126. */
typedef struct {
    u8 pad000[0x120];
    u16 field120;
    u8 pad122[0x873];
    u8 field995;
    u8 pad996[0x32];
    s32 offsets9C8[10];
    u8 pad9F0[7];
    u8 field9F7;
} S06Event;
extern S06Event *D_801F51E8_147108;
/* Canonical func_800D1488/func_80118F10 establish the 32-byte table. */
extern u8 D_801F5228_147148[32];
/* Independent four-byte ramp records: bases materialized at 801E8764,
 * 801E8834 and 801E8840; lbu/sb +0/+1/+2/+3 in 801E8758/801E8834. */
typedef struct { u8 field0, field1, field2, field3; } S06Ramp;
extern S06Ramp D_801F6F0B_148E2B, D_801F6F0F_148E2F;
extern s32 D_801F5140_147060;
/* Partial creature view: byte id +A at 801E8D68; flags +1E at 801E8D7C;
 * byte selector +32 at 801E8D8C. Canonical func_800F27A4 agrees on +A/+1E.
 * Subobjects at +78/+84 have pointer +8, lw at 801E8E98/64. */
typedef struct { u8 pad0[8]; void *field8; } S06Subobject;
typedef struct {
    u8 pad00[0xA];
    u8 id0A;
    u8 pad0B[0x13];
    u8 flags1E;
    u8 pad1F[0x13];
    u8 field32;
    u8 pad33[0x45];
    S06Subobject sub78;
    S06Subobject sub84;
} S06Creature;
/* ODD_C: real word/byte alias. Canonical func_800554C4 initializes both
 * colors with word stores; 801E87FC..801E8818 stores their eight bytes.
 * Neither scalar-only width is a complete view of these colors. */
typedef union { s32 word; u8 bytes[4]; } S06Color;
/* Canonical D_801D40DC is 32 records of 0x60 bytes. +34/+38 swc1 at
 * 801E881C/20; the color bytes above are the only other fields used here. */
typedef struct {
    u8 pad00[0x14];
    S06Color color14, color18;
    u8 pad1C[0x18];
    float scale34, scale38;
    u8 pad3C[0x24];
} S06Task;
extern S06Task D_801D40DC[32];
extern u8 D_8013DF70;
/* D_801F71F0_149110 is cleared with size 14 by the overlay initializer. This
 * updater uses lh at +2*i (801E83C8), lbu +8+i (801E83D8), lbu/sb +C
 * (801E83F4/8574), and lbu/sb +D (801E839C/8584). */
typedef struct {
    s16 targets[4];
    u8 starts[4];
    u8 frame, duration;
} S06Transition;
extern S06Transition D_801F71F0_149110;
/* Four-byte records in 801E9064/92C0: lbu/sb at +0/+1/+2/+3,
 * pointer stride 4, cleared with func_800265E0(record, 4). */
typedef struct { u8 mask, row, column, active; } S06Selection;
#endif
