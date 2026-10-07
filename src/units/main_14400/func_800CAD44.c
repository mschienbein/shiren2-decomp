#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;

typedef struct {
    s16 delta;
    s16 index;
    void *method;
} VTableEntry;

/* Shared prefix views: CADBC reads word 0, CADFC reads word 0x14, and
 * both status operations dispatch through the table at 0x18. */
typedef struct {
    s32 value0;
    u8 pad4[0x10];
    const char *value14;
    VTableEntry *vtable18;
} RecordChild;

typedef struct {
    u8 pad0[9];
    u8 flags9;
    s8 valueA;
    u8 padB[0x11];
    RecordChild *child1C;
} RecordObject;

/* The whole eight-byte single-bit mask table, rodata 0x8015488C..0x80154893
 * = 1,2,4,...,0x80 (func_800B0164 indexes it with i & 7). */
extern u8 D_8015488C[8];
extern void func_800CA0A8(RecordChild *child, s32 value);
extern void func_800CA4A4(RecordChild *child, const char *name);
extern void func_800CA4E8(RecordChild *child, const char *name);
extern void func_800CA2C0(RecordChild *child);
extern void func_800CB050(RecordObject *obj);
extern void func_800CB0C0(RecordObject *obj);
extern void func_800CB288(RecordObject *obj, s32 flag);
extern void func_800CB2D4(RecordObject *obj, s32 flag);
extern void func_800499C0(s32 value);

/* Both original calls use ROM 0x127280 / VRAM 0x80154240, not duplicate
 * strings. Own exactly the eight-byte text object, including its terminator. */
const char D_80154240[] = "RecStat";

void func_800CAD44(RecordObject *self) {
    if (self->child1C != 0) {
        VTableEntry *entry;
        func_800CA0A8(self->child1C, 8);
        func_800CA4A4(self->child1C, D_80154240);
        entry = &self->child1C->vtable18[3];
        ((void (*)(void *, s32, void *))entry->method)(
            (char *)self->child1C + entry->delta, 0x18, self);
        func_800CA2C0(self->child1C);
        func_800CB050(self);
    }
}

void func_800CADBC(RecordObject *self) {
    s32 value = self->child1C->value0;
    func_800CAD44(self);
    func_800CA0A8(self->child1C, value);
}

/* Tests bit `bit` (0..7) of one flag byte: D_8015488C[bit] == 1 << bit.
 * Integrated at both CADFC call sites; no out-of-line copy is emitted. */
static inline s32 testMask(u8 flags, s32 bit) {
    return (flags & D_8015488C[bit]) != 0;
}

void func_800CADFC(RecordObject *obj) {
    RecordChild *child;
    VTableEntry *entry;

    if (obj->child1C == 0) {
        return;
    }
    func_800CA0A8(obj->child1C, 8);
    func_800CA4E8(obj->child1C, D_80154240);
    child = obj->child1C;
    if (child->value14 != 0) {
        return;
    }
    entry = &child->vtable18[5];
    ((void (*)(void *, s32, void *))entry->method)(
        (u8 *)child + entry->delta, 0x18, obj);
    func_800CB0C0(obj);
    func_800CB288(obj, testMask(obj->flags9, 0));
    func_800CB2D4(obj, !testMask(obj->flags9, 1));
    func_800499C0(obj->valueA);
}
