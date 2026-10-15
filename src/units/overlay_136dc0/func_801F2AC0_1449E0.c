/* func_801F2AC0_1449E0 (overlay_136dc0, ROM 0x1449E0, 96 bytes): constructor of the
 * 0x34-byte Obj801F63A8 record (vtable D_801F63A8_1482C8 at +0x30).
 * Author: opus-09 (Anthropic, claude-opus-5-5, xhigh), wave39 private candidate.
 * Record views copied from canonical src/units/overlay_136dc0/func_801F2B20_144A40/types.h
 * (Byte801F2D68, VtEntry, Save801F2D68, Obj801F63A8; EventRect from include/record_views.h). */
typedef int s32;
typedef short s16;
typedef unsigned char u8;
typedef unsigned short u16;

/* One-byte value type at +0x2D (canonical func_801F2D68/func_801F2D74 pass/return it
 * by value). */
typedef struct {
    u8 value;
} Byte801F2D68;

typedef struct EventRect {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} EventRect;

typedef struct {
    s16 delta;
    s16 index;
    void (*pfn)(void *self, s32 size, void *buffer);
} VtEntry;

typedef struct {
    u16 field_00;          /* +0x18: sh $zero at ROM 0x144A1C */
    u8 packed;             /* +0x1A */
    u8 pad_03;
    EventRect rect;        /* +0x1C..+0x2B: sw at ROM 0x144A14/0x144A18 (x0/y0), 0x1449FC/0x144A00 (x1/y1) */
    u8 field_14;           /* +0x2C: sb $zero at ROM 0x144A24 */
    Byte801F2D68 field_15; /* +0x2D: sb at ROM 0x144A34 */
} Save801F2D68;

typedef struct Obj801F63A8 {
    s32 field_00;          /* sw ROM 0x144A20 */
    s32 field_04;          /* sw ROM 0x144A08 */
    s32 field_08;          /* sw ROM 0x144A0C */
    s32 field_0C;          /* sw ROM 0x144A10 */
    s32 field_10;          /* sw ROM 0x144A2C */
    s32 field_14;          /* sw ROM 0x144A28 */
    Save801F2D68 save;     /* +0x18..+0x2F */
    VtEntry *vptr;         /* +0x30: sw ROM 0x1449F4 */
} Obj801F63A8;
extern VtEntry D_801F63A8_1482C8[];

/* The inlined call below owns the 8-byte frame slot written by sb 0($sp)
 * (ROM 0x144A30); +0x2D is stored from the daddu copy of that value. Measured: an
 * s32 parameter or a by-pointer setter removes the frame (80 bytes). */
static inline Byte801F2D68 make_byte(u8 value) {
    Byte801F2D68 byte;

    byte.value = value;
    return byte;
}

/* Returns the receiver (daddu v0,a0 at ROM 0x1449E4). All stores are to distinct
 * members; this source order is the measured one that reproduces the ROM register
 * assignment (constant 1 in a0 shared by +0x08, +0x0C and the frame slot; +0x2D
 * stored from the v1 copy). */
Obj801F63A8 *func_801F2AC0_1449E0(Obj801F63A8 *obj) {
    obj->vptr = D_801F63A8_1482C8;
    obj->field_04 = 0;
    obj->field_08 = 1;
    obj->save.rect.x1 = 0xFF;
    obj->save.rect.y1 = 0xFF;
    obj->field_0C = 1;
    obj->save.rect.x0 = 0;
    obj->save.rect.y0 = 0;
    obj->save.field_00 = 0;
    obj->field_00 = 0;
    obj->save.field_14 = 0;
    obj->field_14 = 0;
    obj->field_10 = 0;
    obj->save.field_15 = make_byte(1);
    return obj;
}
