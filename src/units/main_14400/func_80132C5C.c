#include "common.h"

typedef unsigned char u8;

extern s32 D_801D2524; /* source width (source pixels are two bytes apart) */
extern s32 D_801D2528; /* source height */
extern u8 *D_801D253C; /* destination buffer */
extern s32 D_801D2540; /* destination width */
extern s32 D_801D2544; /* destination height */

/*
 * Copy one byte channel of the source image into the destination buffer,
 * filling extra width/height first by mirroring the edge and then with zeros.
 */
void func_80132C5C(u8 *src)
{
    u8 *dst = D_801D253C;
    u8 *in;
    u8 *row;
    s32 copy_w, mirror_w, pad_w;
    s32 copy_h, mirror_h, pad_h;
    s32 x, y;

    if (D_801D2524 < D_801D2540) {
        copy_w = D_801D2524;
        mirror_w = D_801D2540 - copy_w;
        if (copy_w < mirror_w) {
            mirror_w = copy_w;
        }
        pad_w = D_801D2540 - (copy_w + mirror_w);
    } else {
        copy_w = D_801D2540;
        mirror_w = 0;
        pad_w = 0;
    }

    copy_h = D_801D2528;
    mirror_h = D_801D2544 - copy_h;
    if (copy_h < D_801D2544) {
        if (copy_h < mirror_h) {
            mirror_h = copy_h;
        }
        pad_h = D_801D2544 - (copy_h + mirror_h);
    } else {
        copy_h = D_801D2544;
        mirror_h = 0;
        pad_h = 0;
    }

    for (y = copy_h; y > 0; y--) {
        in = src;
        for (x = copy_w; x > 0; x--) {
            *dst++ = *in;
            in += 2;
        }
        for (x = mirror_w; x > 0; x--) {
            in -= 2;
            *dst++ = *in;
        }
        for (x = pad_w; x > 0; x--) {
            *dst++ = 0;
        }
        src += D_801D2524 * 2;
    }

    row = dst - D_801D2540;
    for (y = mirror_h; y > 0; y--) {
        u8 *line = row;
        for (x = D_801D2540; x > 0; x--) {
            *dst++ = *line++;
        }
        row -= D_801D2540;
    }

    for (y = pad_h; y > 0; y--) {
        for (x = D_801D2540; x > 0; x--) {
            *dst++ = 0;
        }
    }
}
