#include "common.h"
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

/* Reads the record's child pointer; integrated at its call site. */
static inline RecordChild *recordChild(RecordObject *record) {
    return record->child1C;
}

/* Returns the record object while it has a child, else NULL. */
RecordObject *func_800C9E10(void) {
    if (recordChild(&D_80147680)) return &D_80147680;
    return 0;
}
