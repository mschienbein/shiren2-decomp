#include "common.h"

/* Whole 0x30-byte record object at D_80147680 (next object D_801476B0): func_800CB268
 * stores +0xA, func_800CA76C stores +0x20 and the child pointer +0x1C, func_800CB288
 * stores +0x24, func_800CB2D4 +0x28 and func_800CA9A8 clears the word at +0x2C. */
typedef struct RecordObject {
    unsigned char pad0[9];      /* +0x00..+0x08 */
    unsigned char flags9;       /* +0x09 */
    signed char valueA;         /* +0x0A */
    unsigned char padB[0x11];   /* +0x0B..+0x1B */
    void *child1C;              /* +0x1C */
    unsigned char value20;      /* +0x20 */
    unsigned char pad21[3];
    s32 value24;                /* +0x24 */
    s32 value28;                /* +0x28 */
    unsigned char pad2C[4];     /* +0x2C..+0x2F */
} RecordObject;
extern RecordObject D_80147680;
void func_80049C90(s32 mode, s32 handle);
s32 func_80049CB4(s32 id, ...);
s32 func_800A08D8(s32 mode, s32 key, s32 sel) {
    s32 se;
    switch (D_80147680.valueA) {
    case 0:
        return 0;
    case 1:
        if (key == -2) return 0;
        se = 0;
        switch (sel) {
        case 0: se = 6; break;
        case 1: se = 2; break;
        case 2: se = 5; break;
        }
        func_80049CB4(0x129, se);
        return 0;
    case 2:
        if (sel == 0) {
            func_80049C90(mode, key);
            return 1;
        }
        if (key == -2) return 0;
        se = 0;
        switch (sel) {
        case 1: se = 0xE; break;
        case 2: se = 0xC; break;
        }
        func_80049CB4(0x129, se);
        return 0;
    }
    return 0;
}
