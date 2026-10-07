#include "common.h"
extern unsigned char D_80160AF8[8];
extern char D_00194FC0[];
extern char D_2000C60[];
void func_8006AC30(void *dst, void *a, void *b, s32 c, s32 d, s32 e);
/* The object argument is not read; frame selects the 1-based record. */
void *func_80044D1C(void *unusedObject, unsigned char frame) {
    func_8006AC30(D_80160AF8, D_00194FC0, D_2000C60, 8, frame - 1, 1);
    return D_80160AF8;
}
