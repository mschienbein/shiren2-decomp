#include "common.h"

/* Joint unit [0x80132E00, 0x80134000): block reference fetch, coefficient
 * decoder, cell interpolation and YUV-to-RGBA5551 conversion, in address order. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u32 mask;
    u32 bits;
    u32 *src;
} BitReader;

/* Interpolation context: two source rows, edge values, mode and previous value. */
typedef struct {
    u8 *row0;
    u8 pad04[4];
    u8 *row8;
    u8 field_0C;
    u8 field_0D;
    u8 center;
    u8 mode;
    u8 prev;
} Ctx80133858;

extern BitReader D_801CF600[];
extern u8 *D_801CF64C[];
/* Whole serialized tree: root followed by 512 two-halfword nodes. */
extern s16 D_801D0680[];
extern s32 D_801D1710[];
extern u8 D_801D1F10[0x300];
extern u8 D_801D2210[0x300];
extern s32 D_801D2514;
extern u8 *D_801D253C;
extern s32 D_801D2540;
extern u8 D_801D2548;

/*
 * Fetch one 4x4 block from the reference image (position and x/y step taken
 * from the channel's next 16-bit code), remove its rounded mean, and scale
 * *out by the factor for the largest remaining absolute coefficient.
 */
void func_80132E00(s16 *block, s32 *out, s32 channel) {
    u16 *src;
    u16 code;
    s32 xstep;
    s32 ystep;
    s32 stride;
    u8 *row;
    u8 *p;
    s32 sum;
    s32 first;
    s32 v;
    s32 m;
    src = (u16 *)D_801CF64C[channel];
    code = *src;
    D_801CF64C[channel] = (u8 *)(src + 1);
    xstep = (code & 1) + 1;
    ystep = ((code >> 1) & 1) + 1;
    if (D_801D2548 == 8) {
        row = D_801D253C + (((code >> 2) & 0x3F) + ((code >> 8) & 0x1F) * D_801D2540);
    } else {
        row = D_801D253C + (((code >> 2) & 0x1F) + ((code >> 7) & 0x3F) * D_801D2540);
    }
    stride = ystep * D_801D2540;
    p = row;
    first = block[0] = *p;
    p += xstep;
    sum = block[1] = *p;
    sum += first;
    p += xstep;
    sum += block[2] = *p;
    p += xstep;
    sum += block[3] = *p;
    row += stride;
    p = row;
    sum += block[4] = *p;
    p += xstep;
    sum += block[5] = *p;
    p += xstep;
    sum += block[6] = *p;
    p += xstep;
    sum += block[7] = *p;
    row += stride;
    p = row;
    sum += block[8] = *p;
    p += xstep;
    sum += block[9] = *p;
    p += xstep;
    sum += block[10] = *p;
    p += xstep;
    sum += block[11] = *p;
    row += stride;
    p = row;
    sum += block[12] = *p;
    p += xstep;
    sum += block[13] = *p;
    p += xstep;
    sum += block[14] = *p;
    p += xstep;
    sum += block[15] = *p;
    sum = (sum + 8) >> 4;
    v = *block -= sum;
    block++;
    m = v >= 0 ? v : -v;
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *block -= sum;
    v = *block++;
    v = v >= 0 ? v : -v;
    if (m < v) {
        m = v;
    }
    *out = D_801D1710[m] * (*out + (code >> 13));
}

/* ODD_C: decode one signed-halfword leaf with a cursor that starts at the root
 * and then selects children. This also preserves GCC 2.7.2's allocation:
 * a separate tree[0] root read swaps child/entry registers (4 differing words).
 * Keep the leaf address separate from the branch-node address computation. */
static inline s16 decode_scale(BitReader *reader, s16 *tree) {
    s16 *child = tree;
    u32 bit;
    s16 node;

    for (node = *child; node >= 0x100; node = *child) {
        /* local-arithmetic-qualification: this is tree + node * 2, wholly
         * inside the serialized tree. Typed &tree[node * 2] commutes the
         * inner addu at 0x80133528 (1 differing word under the KMC profile).
         * Only this local address expression uses an integer carrier. */
        s16 *entry = (s16 *)((u32)(node * 4) + (u32)tree);
        child = entry + 1;
        if (reader->mask == 0) {
            reader->bits = *reader->src++;
            reader->mask = 0x80000000;
        }
        bit = reader->bits & reader->mask;
        reader->mask >>= 1;
        if (bit) {
            child = entry + 2;
        }
    }
    {
        s16 *leaf = tree + node * 2;

        return leaf[1];
    }
}

/* The block decoder is manually unrolled over its 16 coefficients. */
#define COPY_RAW() *out++ = *D_801CF64C[channel]++
#define FILL() *p++ = fill
#define ACCUMULATE(k) *p += (scale * coef[k] + 0x200) >> 10; p++

void func_801332A4(s16 *out, s32 count, u8 fill, s32 channel) {
    s16 *p;
    s32 i;
    s16 coef[16];
    s32 scale;

    if (count == 8) {
        COPY_RAW();
        COPY_RAW();
        COPY_RAW();
        COPY_RAW();
        COPY_RAW();
        COPY_RAW();
        COPY_RAW();
        COPY_RAW();
        COPY_RAW();
        COPY_RAW();
        COPY_RAW();
        COPY_RAW();
        COPY_RAW();
        COPY_RAW();
        COPY_RAW();
        *out = *D_801CF64C[channel]++;
        return;
    }
    p = out;
    FILL();
    FILL();
    FILL();
    FILL();
    FILL();
    FILL();
    FILL();
    FILL();
    FILL();
    FILL();
    FILL();
    FILL();
    FILL();
    FILL();
    FILL();
    *p = fill;
    for (i = 0; i < count; i++) {
        scale = decode_scale(&D_801CF600[channel], D_801D0680);
        func_80132E00(coef, &scale, channel);
        p = out;
        ACCUMULATE(0);
        ACCUMULATE(1);
        ACCUMULATE(2);
        ACCUMULATE(3);
        ACCUMULATE(4);
        ACCUMULATE(5);
        ACCUMULATE(6);
        ACCUMULATE(7);
        ACCUMULATE(8);
        ACCUMULATE(9);
        ACCUMULATE(10);
        ACCUMULATE(11);
        ACCUMULATE(12);
        ACCUMULATE(13);
        ACCUMULATE(14);
        *p += (scale * coef[15] + 0x200) >> 10;
    }
}

/* Second difference of two neighbours around the centre value. */
static inline s32 curvature(s32 x, s32 y, u8 m) {
    return x + y - m * 2;
}

/* Expand one cell into a smoothed 4x4 block of values. */
void func_80133858(s16 *out, Ctx80133858 *ctx, s32 stream) {
    u8 mode = ctx->mode;
    u8 d = ctx->center;

    if (mode == 0) {
        /* A flagged neighbour falls back to the centre value. */
        u8 raw_a = ctx->field_0D ? d : ctx->field_0C;
        u8 raw_b = ctx->row0[1] ? d : ctx->row0[0];
        u8 raw_c = ctx->row8[1] ? d : ctx->row8[0];
        s32 b = raw_b;
        s32 c = raw_c;
        s32 a = raw_a;
        s16 bc = b - c;
        s32 e = ctx->prev;
        s16 ba = curvature(b, a, d);
        u32 base = d * 8;
        s16 ea = e - a;
        u32 h = bc - ea;
        s16 ec = curvature(e, c, d);
        s16 ca = curvature(c, a, d);
        s16 be = curvature(b, e, d);
        u32 g = bc + ea;
        /* ODD_C: retain computed pixel values until the output row is committed. */
        s16 p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14;

        /* ODD_C: the rounding bias joins the scaled centre after the corner terms
         * (the ROM's sll precedes the first corner sum, its ori follows it). */
        base |= 4;
        p0 = (base + g + be) >> 3;
        p1 = (base + g + (b - e)) >> 3;
        p2 = (base + h + (b - a)) >> 3;
        p3 = (base + h + ba) >> 3;
        p4 = (base + g - (b - e)) >> 3;
        p5 = (base - ca) >> 3;
        p6 = (base - ec) >> 3;
        p7 = (base + h - (b - a)) >> 3;
        p8 = (base - h - (c - e)) >> 3;
        p9 = (base - ba) >> 3;
        p10 = (base - be) >> 3;
        p11 = (base - g - (c - a)) >> 3;
        p12 = (base - h + ec) >> 3;
        p13 = (base - h + (c - e)) >> 3;
        p14 = (base - g + (c - a)) >> 3;
        out[0] = p0;
        out[1] = p1;
        out[2] = p2;
        out[3] = p3;
        out[4] = p4;
        out[5] = p5;
        out[6] = p6;
        out[7] = p7;
        out[8] = p8;
        out[9] = p9;
        out[10] = p10;
        out[11] = p11;
        out[12] = p12;
        out[13] = p13;
        out[14] = p14;
        /* The last pixel is rounded with a signed shift (sra in the ROM). */
        out[15] = (s32)(base - g + ca) >> 3;
        ctx->prev = d;
    } else {
        func_801332A4(out, mode, d, stream);
        ctx->prev = ctx->field_0C;
    }
    ctx->row0 += 2;
    ctx->row8 += 2;
}

/* Each chroma sample supplies two luma samples. Bias 0x4020 includes
 * the clamp-table origin and the rounding term for six fractional bits.
 * ODD_C: pair expansions reuse the live sample and coefficient variables. */
#define CONVERT_PAIR(LUMA) { \
    cr = (s16)(*v++ - 128); \
    y0 = (s16)(*LUMA++ << 6); \
    red = (s16)(90 * cr + 0x4020); \
    cb = (s16)(*u++ - 128); \
    cb_green = 22 * cb; \
    cr_green = 46 * cr; \
    green = (s16)(-cr_green - cb_green + 0x4020); \
    blue = (s16)(113 * cb + 0x4020); \
    *pixel++ = ((D_801D1F10[(y0 + red) >> 6] << 10) | \
        (D_801D1F10[(y0 + green) >> 6] << 5)) | D_801D2210[(y0 + blue) >> 6]; \
    y0 = (s16)(*LUMA++ << 6); \
    *pixel++ = ((D_801D1F10[(y0 + red) >> 6] << 10) | \
        (D_801D1F10[(y0 + green) >> 6] << 5)) | D_801D2210[(y0 + blue) >> 6]; \
}
/* Converts four rows of eight RGBA5551 pixels. Each row takes four u/v
 * pairs and four luma samples from each half of the 32-sample luma block;
 * rows are D_801D2514 pixels apart. */
void func_80133A68(u16 *output, u16 *luma, u16 *u, u16 *v)
{
    u16 *first = luma;
    u16 *second = luma + 16;
    s32 rows = 4;
    s16 cr, cb, y0, red, green, blue;
    s32 cb_green, cr_green;
    u16 *pixel;
    do {
        pixel = output;
        CONVERT_PAIR(first);
        CONVERT_PAIR(first);
        CONVERT_PAIR(second);
        CONVERT_PAIR(second);
        output += D_801D2514;
    } while (--rows > 0);
}
#undef CONVERT_PAIR
