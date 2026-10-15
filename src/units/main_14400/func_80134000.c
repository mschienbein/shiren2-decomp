#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
extern u8 D_801D1F10[0x300];
extern u8 D_801D2210[0x300];
extern s32 D_801D2514;
/* Bias 0x4020 includes the clamp-table origin and the rounding term for
 * six fractional bits. */
#define pack_color(y, red, green, blue) \
    (((D_801D1F10[((y) + (red)) >> 6] << 10) | \
      (D_801D1F10[((y) + (green)) >> 6] << 5)) | \
      D_801D2210[((y) + (blue)) >> 6])
/* One luma sample, one output pixel. */
#define PIXEL(LUMA, red, green, blue) \
    y = *LUMA++ << 6; \
    *pixel++ = pack_color(y, red, green, blue)
/* One u/v sample pair: its colour terms and the first two pixels of the
 * 2x2 block they cover. */
#define CHROMA_PAIR(LUMA, red, green, blue) \
    v = *cr++ - 128; \
    y = *LUMA++ << 6; \
    red = v * 90 + 0x4020; \
    u = *cb++ - 128; \
    { \
        s32 cb_green = 22 * u; \
        s32 cr_green = 46 * v; \
        green = -cr_green - cb_green + 0x4020; \
    } \
    blue = u * 113 + 0x4020; \
    *pixel++ = pack_color(y, red, green, blue); \
    PIXEL(LUMA, red, green, blue)
/* Four rows of eight pixels from the left and right 4x4 luma quadrants,
 * two rows per iteration sharing the chroma terms. */
#define CONVERT_ROWS() \
    remaining = 4; \
    do { \
        pixel = output; \
        CHROMA_PAIR(left, red0, green0, blue0); \
        CHROMA_PAIR(left, red1, green1, blue1); \
        CHROMA_PAIR(right, red2, green2, blue2); \
        CHROMA_PAIR(right, red3, green3, blue3); \
        output += D_801D2514; \
        pixel = output; \
        PIXEL(left, red0, green0, blue0); \
        PIXEL(left, red0, green0, blue0); \
        PIXEL(left, red1, green1, blue1); \
        PIXEL(left, red1, green1, blue1); \
        PIXEL(right, red2, green2, blue2); \
        PIXEL(right, red2, green2, blue2); \
        PIXEL(right, red3, green3, blue3); \
        PIXEL(right, red3, green3, blue3); \
        output += D_801D2514; \
        remaining -= 2; \
    } while (remaining > 0)
/* Converts an 8x8 block: luminance holds four 4x4 quadrants (top-left,
 * top-right, bottom-left, bottom-right), cb/cr one sample per 2x2 block. */
void func_80134000(u16 *output, u16 *luminance, u16 *cb, u16 *cr)
{
    u16 *left;
    u16 *right;
    u16 *quadrant;
    u16 *pixel;
    s32 remaining;
    s16 u, v, y;
    s16 red0, green0, blue0, red1, green1, blue1;
    s16 red2, green2, blue2, red3, green3, blue3;

    left = luminance;
    right = left + 16;
    quadrant = right;
    CONVERT_ROWS();
    quadrant += 16;
    left = quadrant;
    right = quadrant + 16;
    CONVERT_ROWS();
}
#undef CONVERT_ROWS
#undef CHROMA_PAIR
#undef PIXEL
#undef pack_color

/* Decoder channel state shared with func_80135670 and read by
 * func_80133858: sample stream cursors and the held samples. */
typedef struct { u8 first, second; } Sample;
typedef struct {
    Sample *previous;
    Sample *current;
    Sample *next;
    Sample samples[6];
} Channel;
typedef void (*ColorConverter)(u16 *, u16 *, u16 *, u16 *);
extern ColorConverter D_801D2510;
extern s32 D_801D2518, D_801D2520, D_801D2530;
extern Channel D_801D16A8;
extern Channel D_801D16C0;
extern Channel D_801D16D8;
extern Channel D_801D16F0;
extern void func_80133858(u16 *out, Channel *channel, s32 plane);

#define START_CHANNEL(channel) \
    (channel)->samples[0] = *(channel)->current; \
    (channel)->samples[2].first = (channel)->samples[0].first
/* Shift the held sample, fetch the next one, decode one 4x4 block. */
#define DECODE_BLOCK(out, channel, plane) { \
    Sample *older = &(channel)->samples[1]; \
    *older = (channel)->samples[0]; \
    (channel)->samples[0] = *++(channel)->current; \
    func_80133858(out, channel, plane); \
}
/* Last block of a row: shift and advance without fetching. */
#define DECODE_LAST(out, channel, plane) { \
    Sample *older = &(channel)->samples[1]; \
    *older = (channel)->samples[0]; \
    (channel)->current++; \
    func_80133858(out, channel, plane); \
}

/* Decodes one macroblock row: luma blocks from D_801D16A8 (and
 * D_801D16C0 when D_801D2520 is set), chroma from D_801D16D8/D_801D16F0,
 * converting each macroblock through D_801D2510. */
void func_8013517C(u16 *output)
{
    u16 luma[64];
    u16 chroma_u[16];
    u16 chroma_v[16];
    s32 count;

    START_CHANNEL(&D_801D16D8);
    START_CHANNEL(&D_801D16F0);
    START_CHANNEL(&D_801D16A8);
    if (D_801D2520) {
        START_CHANNEL(&D_801D16C0);
    }
    for (count = D_801D2530 - 1; count > 0; count--) {
        DECODE_BLOCK(&luma[0], &D_801D16A8, 0);
        DECODE_BLOCK(&luma[16], &D_801D16A8, 0);
        if (D_801D2520) {
            DECODE_BLOCK(&luma[32], &D_801D16C0, 0);
            DECODE_BLOCK(&luma[48], &D_801D16C0, 0);
        }
        DECODE_BLOCK(chroma_u, &D_801D16D8, 1);
        DECODE_BLOCK(chroma_v, &D_801D16F0, 2);
        D_801D2510(output, luma, chroma_u, chroma_v);
        output += D_801D2518;
    }
    DECODE_BLOCK(&luma[0], &D_801D16A8, 0);
    DECODE_LAST(&luma[16], &D_801D16A8, 0);
    if (D_801D2520) {
        DECODE_BLOCK(&luma[32], &D_801D16C0, 0);
        DECODE_LAST(&luma[48], &D_801D16C0, 0);
    }
    DECODE_LAST(chroma_u, &D_801D16D8, 1);
    DECODE_LAST(chroma_v, &D_801D16F0, 2);
    D_801D2510(output, luma, chroma_u, chroma_v);
}
#undef START_CHANNEL
#undef DECODE_BLOCK
#undef DECODE_LAST
