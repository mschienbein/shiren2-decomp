#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;

typedef struct Unit800E370C {
    s32 x;
    s32 y;
    u8 pad8;
    u8 field_09;
    u8 padA[0x14];
    u8 field_1E;
    u8 pad1F[0x39];
    struct Unit800E370C *target_58;
    u8 pad5C[0x16];
    u8 field_72;
} Unit800E370C;

typedef struct RecordChild RecordChild;
/* Whole 0x30-byte record object at D_80147680 (next object D_801476B0): func_800CB268
 * stores +0xA, func_800CA76C stores +0x20 and the child pointer +0x1C, func_800CB288
 * stores +0x24, func_800CB2D4 +0x28 and func_800CA9A8 clears the word at +0x2C. */
typedef struct RecordObject {
    unsigned char pad0[9];      /* +0x00..+0x08 */
    unsigned char flags9;       /* +0x09 */
    signed char valueA;         /* +0x0A */
    unsigned char padB[0x11];   /* +0x0B..+0x1B */
    RecordChild *child1C;       /* +0x1C */
    unsigned char value20;      /* +0x20 */
    unsigned char pad21[3];
    s32 value24;                /* +0x24 */
    s32 value28;                /* +0x28 */
    unsigned char pad2C[4];     /* +0x2C..+0x2F */
} RecordObject;

extern RecordObject D_80147680;

void func_800D739C(void *arg0);
s32 func_800E2074(Unit800E370C *unit);
u16 func_800E08B0(void *obj);
s32 func_800E1D14(Unit800E370C *obj, s32 kind);
s32 func_800E1CC4(Unit800E370C *obj, s32 kind);
u32 func_800B1C6C(Unit800E370C *pos);
void *func_800A6CF0(Unit800E370C *unit);
s32 func_800A4520(void *ctx, Unit800E370C *obj);
s32 func_800A6FD0(Unit800E370C *self);
void func_800A6690(Unit800E370C *unit, unsigned char *dir, s32 arg2);
void func_800A665C(Unit800E370C *obj, u8 *value);

void func_800E370C(Unit800E370C *unit, u8 *dir)
{
    s32 blocked;
    s32 skip;
    Unit800E370C *target;

    func_800D739C(unit);
    blocked = 0;
    if (func_800E2074(unit) == 0 || func_800E08B0(unit) == 0 || func_800E1D14(unit, 0x13) != 0
        || func_800E1D14(unit, 0x14) != 0 || func_800E1CC4(unit, 0) != 0 || (unit->field_72 & 2)
        || ((unit->field_09 & 0xF) == 1 && (func_800B1C6C(unit) & 0x80))) {
        blocked = 1;
    }
    if (blocked) {
        return;
    }
    target = func_800A6CF0(unit);
    skip = 0;
    if (func_800A4520(unit, target) && (func_800A6FD0(unit) || target == unit->target_58)
        && (target->field_1E & 0x7C) && func_800E1CC4(target, 1) == 0) {
        skip = 1;
    }
    if (skip) {
        return;
    }
    if (D_80147680.valueA == 2) {
        func_800A6690(unit, dir, 1);
    } else {
        func_800A665C(unit, dir);
    }
}
