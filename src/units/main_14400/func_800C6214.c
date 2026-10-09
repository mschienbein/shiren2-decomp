#include "common.h"
typedef unsigned char u8;
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
/* Whole menu-system object; +6 is its quit-request byte, not a standalone object. */
extern u8 D_80140160[];
extern s32 D_801476B0;
extern RecordObject D_80147680;
s32 D_80147670 = 0;
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800CA76C(void *, u8);
extern void func_800C6874(void);
extern s32 func_800C6A78(void);
extern void func_800C6B38(void);
void func_800C6214(void) {
    s32 result;
    func_80049CB4(1);
    func_80049CB4(8);
    func_80049CB4(10);
    result = func_800CA76C(&D_80147680, (u8)(((u8 *)&D_801476B0)[3] + 10));
    if (result == 3) {
        D_80140160[6] = 1;
        D_80147670 = result;
        func_800C6874();
        func_800C6A78();
        func_800C6B38();
        if (++D_801476B0 >= 6) D_801476B0 = 0;
    }
}
