#include "common.h"

typedef struct {
    unsigned char unk00[9];
    unsigned char unk09;
    unsigned char unk0A[0x12];
    unsigned short unk1C;
    unsigned char unk1E;
    unsigned char unk1F[0x6A];
    unsigned char unk89;
} Object;
extern u32 func_800B1C6C(Object *);
extern s32 func_800A58B8(Object *);
extern char *func_800A3B20(Object *);
extern s32 func_800E0F40(Object *);
extern s32 func_800E91E4(Object *);
extern s32 func_800E9144(Object *);
extern s32 func_800E8C64(Object *);
extern s32 func_80049CB4(s32 command, ...);
extern void func_80049AE8(s32 message, ...);
extern void func_800497F0(s32 id, ...);
extern s32 func_800A08D8(s32, s32, s32);
extern void func_800A7B18(Object *, Object *, s32, s32);
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

s32 func_8010551C(Object *object, Object *target) {
    s32 invalid;
    s32 message;
    if ((object->unk09 & 0xF) != 1) {
        return 1;
    }
    invalid = 0;
    if (target == 0 || (target->unk1C & 1) || (func_800B1C6C(target) & 0x4000)) {
        invalid = 1;
    }
    if (invalid != 0) {
        func_80049CB4(0x6C, object);
        return 1;
    }
    if (func_800A58B8(target) == 1) {
        return 0;
    }
    message = func_80049CB4(0x6C, object);
    func_80049AE8(0x14C, message, func_800A3B20(object));
    if (target->unk1E & 0xC) {
        switch (func_800E0F40(object) & 0xFF) {
        case 1:
            if (func_800E91E4(target) != 0) {
                func_800497F0(0x14D, message);
                func_800A08D8(1, message, 0);
            }
            break;
        case 2:
            if (func_800E9144(target) != 0) {
                func_800497F0(0xF4, message);
                func_800A08D8(1, message, 0);
            }
            break;
        case 3:
            func_800E8C64(target);
            func_800A08D8(1, message, 0);
            break;
        }
    }
    if (D_80147680.valueA == 2) {
        target->unk1C |= 0x100;
    }
    func_800A7B18(target, object, object->unk89, 2);
    target->unk1C &= 0xFEFF;
    return 1;
}
