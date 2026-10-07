#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u16 size;
    u8 pixels[0x86];
} Glyph;

extern u8 D_00177150[];
extern Glyph D_80165980;

/* devAddr is a PI ROM offset, not a CPU pointer. */
void func_8006AAF0(void *dst, u32 devAddr, s32 size);

void func_8005DE08(s32 index, u8 **pixels, s32 *width, s32 *height) {
    Glyph *glyph = &D_80165980;

    func_8006AAF0(glyph, (u32)D_00177150 + index * 0x82, 0x88);
    *width = glyph->size >> 8;
    *height = glyph->size & 0xFF;
    *pixels = glyph->pixels;
}
