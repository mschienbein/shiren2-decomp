#include "common.h"
/* Glyph buffer (0x88 bytes are DMA'd into it); bytes 0 and 1 hold width and height. */
extern unsigned short D_80165980[];
/* Linker symbol for the glyph table's ROM/PI device offset 0x00157B80 (0x72-byte
 * glyph records). Only its numeric value is used; it is never dereferenced. */
extern unsigned char D_00157B80[];
extern s32 func_8005DCB0(s32 value);
extern void func_8006AAF0(void *destination, u32 deviceOffset, s32 size);
/* The character code is an int: every original caller masks it to 16 bits before
 * the call (andi a0,0xFFFF at 0x8005DF38, 0x8005E03C and 0x8005E0E8) and this
 * function forwards a0 to func_8005DCB0 unmasked, so a 16-bit parameter (which
 * would add a callee-side andi) is not the contract. */
void func_8005E134(s32 value, s32 *width, s32 *height) {
    func_8006AAF0(D_80165980, (u32)D_00157B80 + (u32)func_8005DCB0(value) * 0x72U, 0x88);
    *width = D_80165980[0] >> 8;
    *height = ((unsigned char *)D_80165980)[1];
}
