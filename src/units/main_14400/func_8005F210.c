#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u32 w0; u32 w1; } Gfx;
/* Render task handed to the frame callbacks (func_8005F0B0 registers both with
 * func_8006B184/func_8006B1B0); partial view of the leading fields. */
typedef struct RenderFrame { void *framebuffer; u32 flags; } RenderFrame;
typedef struct RenderContext { u8 type; u8 pad1[3]; RenderFrame *frame; void *depthbuffer; } RenderContext;

#define SHIFTL(v, s, w) ((u32)(((u32)(v)) & ((1 << (w)) - 1)) << (s))

#define gDPWord(pkt, hi, lo) \
    { \
        Gfx *_g = (Gfx *)(pkt); \
        _g->w0 = (hi); \
        _g->w1 = (u32)(lo); \
    }
#define gSPSegment(pkt, seg, base) gDPWord(pkt, 0xDB060000 | ((seg) * 4), base)
#define gSPDisplayList(pkt, dl) gDPWord(pkt, 0xDE000000, dl)
#define gDPPipeSync(pkt) gDPWord(pkt, 0xE7000000, 0)
#define gDPFullSync(pkt) gDPWord(pkt, 0xE9000000, 0)
#define gSPEndDisplayList(pkt) gDPWord(pkt, 0xDF000000, 0)
#define gDPSetDepthImage(pkt, img) gDPWord(pkt, 0xFE000000, img)
#define gDPSetColorImage16(pkt, img) gDPWord(pkt, 0xFF10013F, img)
#define gDPSetScissorFrac(pkt, ulx, uly, lrx, lry) \
    gDPWord(pkt, 0xED000000 | SHIFTL((int)(ulx), 12, 12) | SHIFTL((int)(uly), 0, 12), \
            SHIFTL((int)(lrx), 12, 12) | SHIFTL((int)(lry), 0, 12))
#define gDPFillRectangle(pkt, ulx, uly, lrx, lry) \
    gDPWord(pkt, 0xF6000000 | SHIFTL(lrx, 14, 10) | SHIFTL(lry, 2, 10), \
            SHIFTL(ulx, 14, 10) | SHIFTL(uly, 2, 10))

extern void *D_8013B748;
/* Current render task, also returned by func_80060038. */
extern RenderContext *D_80165A10;
extern u8 D_8013B740;
extern Gfx D_80167A18[][0x200];
extern s32 D_8013B744;
extern float D_8013B7A8;
extern float D_8013B7AC;
extern float D_8013B7B0;
extern float D_8013B7B4;
extern s32 D_8013B75C;
extern u32 D_80169A48;
extern u32 D_80169A4C;
extern u32 D_80169A50;
/* Saved CPU copy of the last frame (func_8006B580 buffer, released with func_8006B6F4). */
extern u8 *D_80169A54;
extern Gfx D_8013B6C0[];
extern Gfx D_010000B8[];
extern Gfx D_01000010[];
extern Gfx D_01000120[];
extern Gfx D_01000140[];
u32 func_800340F0(void *addr);
void func_8005B4AC(void);
s32 func_8007D890(void);
Gfx *func_80055078(Gfx *gfx);
Gfx *func_80060048(Gfx *gfx, const void *image, const void *palette, s32 format, s32 width, s32 height, s32 x,
                   s32 y, float scaleX, float scaleY, const Gfx *extra);
void func_8006B7F8(Gfx *dl, s32 size, s32 ucode, s32 kind, void *framebuffer, void *depthbuffer,
                   void *completion_queue, void *completion_message);

s32 func_8005F210(RenderContext *task) {
    Gfx *gfx;

    D_80165A10 = task;
    if (D_8013B748 == 0) {
        return -1;
    }
    if (task->type >= 3) {
        return -1;
    }
    gfx = D_80167A18[D_8013B740];
    gSPSegment(gfx++, 0, 0);
    gSPSegment(gfx++, 1, func_800340F0(D_8013B748));
    gSPDisplayList(gfx++, D_010000B8);
    gSPDisplayList(gfx++, D_01000010);
    gDPWord(gfx++, 0xED000000, 0x005003C0);
    gDPSetDepthImage(gfx++, func_800340F0(D_80165A10->depthbuffer));
    gDPSetColorImage16(gfx++, func_800340F0(D_80165A10->depthbuffer));
    gSPDisplayList(gfx++, D_8013B6C0);
    gDPPipeSync(gfx++);
    gDPSetColorImage16(gfx++, func_800340F0(D_80165A10->frame->framebuffer));
    if (D_80165A10->frame->flags & 2) {
        D_80165A10->frame->flags &= ~2;
        gSPDisplayList(gfx++, D_01000120);
        gDPWord(gfx++, 0xF64FC3BC, 0);
    }
    gSPDisplayList(gfx++, D_01000010);
    if (D_8013B744 != 0) {
        gDPSetScissorFrac(gfx++, D_8013B7A8 * 4.0f, D_8013B7AC * 4.0f, D_8013B7B0 * 4.0f, D_8013B7B4 * 4.0f);
    }
    func_8005B4AC();
    gSPDisplayList(gfx++, D_010000B8);
    if (func_8007D890() != 0) {
        gSPDisplayList(gfx++, D_01000120);
        gDPFillRectangle(gfx++, D_8013B7A8, D_8013B7AC, D_8013B7B0 - 1.0f, D_8013B7B4 - 1.0f);
    } else if (D_8013B75C == 0) {
        gfx = func_80055078(gfx);
    } else {
        gSPDisplayList(gfx++, D_01000140);
        gfx = func_80060048(gfx, D_80169A54, 0, 0x22, D_80169A4C, D_80169A50, 0, 0, D_80169A48, D_80169A48, 0);
    }
    gDPFullSync(gfx++);
    gSPEndDisplayList(gfx++);
    func_8006B7F8(D_80167A18[D_8013B740], (s32)((gfx - D_80167A18[D_8013B740]) * sizeof(Gfx)), 0, 4,
                  D_80165A10->frame->framebuffer, D_80165A10->depthbuffer, 0, 0);
    return 0;
}
