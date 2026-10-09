#include "common.h"
typedef struct { short adjustment; short index; void (*transfer)(void *, s32, void *); } Slot;
typedef struct { unsigned char pad0[0x18]; Slot *vtable18; } Serializer;
/* Whole 0x30-byte record object at D_80147680 (next object D_801476B0): func_800CB268
 * stores +0xA, func_800CA76C stores +0x20 and the child pointer +0x1C, func_800CB288
 * stores +0x24, func_800CB2D4 +0x28 and func_800CA9A8 clears the word at +0x2C. */
typedef struct RecordObject {
    unsigned char pad0[9];      /* +0x00..+0x08 */
    unsigned char flags9;       /* +0x09 */
    signed char valueA;         /* +0x0A */
    unsigned char padB[0x11];   /* +0x0B..+0x1B */
    Serializer *child1C;        /* +0x1C */
    unsigned char value20;      /* +0x20 */
    unsigned char pad21[3];
    s32 value24;                /* +0x24 */
    s32 value28;                /* +0x28 */
    unsigned char pad2C[4];     /* +0x2C..+0x2F */
} RecordObject;
extern RecordObject D_80147680;
extern const char D_801541C0[];
extern unsigned char D_801476C3;
extern unsigned char D_80147620[];
extern unsigned char D_80140160[];
extern void func_800CA250(void *object, s32 value);
extern void func_800CA0A8(void *object, s32 value);
extern void func_800CA4A4(void *object, const void *name);
extern void func_800C5B8C(void *object, void *serializer);
extern void func_800C56D4(void *object);
extern void func_800C9A54(void *serializer);
extern void func_800C573C(void *object);
extern void func_800C9C00(void *serializer);
extern void func_80094BC8(unsigned char *object);
void func_800C9870(void) {
    Serializer *serializer = D_80147680.child1C;
    if (serializer) {
        Slot *slot;
        unsigned char *object;
        func_800CA250(serializer, 0x22);
        func_800CA0A8(serializer, 0x22);
        func_800CA4A4(serializer, D_801541C0);
        slot = &serializer->vtable18[3];
        slot->transfer((unsigned char *)serializer + slot->adjustment, 1, &D_801476C3);
        object = D_80147620;
        func_800C5B8C(object, serializer);
        func_800C56D4(object);
        func_800C9A54(serializer);
        func_800C573C(object);
        func_800C56D4(object);
        func_800C9C00(serializer);
        func_800C573C(object);
        func_80094BC8(D_80140160);
    }
}
