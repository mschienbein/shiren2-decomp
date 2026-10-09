#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u32 w0; u32 w1; } Gfx;
/* Render task handed to the frame callbacks (func_8005F0B0 registers both with
 * func_8006B184/func_8006B1B0); partial view of the leading fields. */
typedef struct RenderFrame { void *framebuffer; u32 flags; } RenderFrame;
typedef struct RenderContext { u8 type; u8 pad1[3]; RenderFrame *frame; void *depthbuffer; } RenderContext;

typedef struct Model Model;
typedef struct {
    u32 count;
    s32 field_4;
    Model **items;
} ModelList;

/* Double-buffered layer (same layout as func_8006EA64's Layer). */
typedef struct {
    u8 enabled;
    u8 frame;
    u8 pad2[2];
    u32 length;
    Gfx *lists[2];
    u32 length2;
    Gfx *lists2[2];
    ModelList models;
} Layer;

typedef struct ProbeMessageQueue ProbeMessageQueue;
typedef void *ProbeMessage;

#define SHIFTL(v, s, w) ((u32)(((u32)(v)) & ((1 << (w)) - 1)) << (s))

#define gDPWord(pkt, hi, lo) \
    { \
        Gfx *_g = (Gfx *)(pkt); \
        _g->w0 = (hi); \
        _g->w1 = (u32)(lo); \
    }
#define gSPSegment(pkt, seg, base) gDPWord(pkt, 0xDB060000 | ((seg) * 4), base)
#define gSPDisplayList(pkt, dl) gDPWord(pkt, 0xDE000000, dl)
#define gDPFullSync(pkt) gDPWord(pkt, 0xE9000000, 0)
#define gSPEndDisplayList(pkt) gDPWord(pkt, 0xDF000000, 0)
#define gDPSetPrimColor32(pkt, rgba) gDPWord(pkt, 0xFA000000, rgba)
#define gDPSetScissorFrac(pkt, ulx, uly, lrx, lry) \
    gDPWord(pkt, 0xED000000 | SHIFTL((int)(ulx), 12, 12) | SHIFTL((int)(uly), 0, 12), \
            SHIFTL((int)(lrx), 12, 12) | SHIFTL((int)(lry), 0, 12))

extern void *D_8013B748;
/* Current render task, also returned by func_80060038. */
extern RenderContext *D_80165A10;
extern u8 D_8013B740;
extern Gfx D_80165A18[][0x200];
extern s32 D_8013B744;
extern float D_8013B7A8;
extern float D_8013B7AC;
extern float D_8013B7B0;
extern float D_8013B7B4;
extern s32 D_80169A44;
extern s32 D_8013B758;
extern s32 D_8013B75C;
extern u8 D_8013B761;
extern u8 D_8013B762;
extern u8 D_8013B763;
extern u8 D_8013B764;
extern u32 D_80169A48;
extern u32 D_80169A4C;
extern u32 D_80169A50;
/* Saved CPU copy of the last frame (func_8006B580 buffer, released with func_8006B6F4). */
extern u8 *D_80169A54;
extern Layer D_801E4E48;
extern Layer D_801D8FD0;
extern ProbeMessageQueue D_801D8590;
extern Gfx D_8013B728[];
extern Gfx D_010000B8[];
extern Gfx D_01000010[];

u32 func_800340F0(void *addr);
s32 func_8005C880(void);
void func_8005D79C(void);
void func_8006EDCC(void);
Gfx *func_80060048(Gfx *gfx, const void *image, const void *palette, s32 format, s32 width, s32 height, s32 x,
                   s32 y, float scaleX, float scaleY, const Gfx *extra);
Gfx *func_8005C790(Gfx *gfx, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
u8 func_8006C508(u8 value);
void func_8006B6F4(u8 *buf);
Gfx *func_8005BB94(Gfx *gdl);
Gfx *func_80069D78(Gfx *gfx);
Gfx *func_80069F14(Gfx *cursor);
void func_800610CC(void);
void func_800629AC(void);
void func_8006EA40(unsigned char *arg0);
void func_8007BD30(void);
void func_8008C2E0(void);
Gfx *func_8006EA64(Gfx *gfx, Gfx *gfxEnd, Layer *layer);
void func_800611F8(s32 slot, void *display_list);
Gfx *func_800610F4(Gfx *out);
void func_8006B7F8(Gfx *dl, s32 size, s32 ucode, s32 kind, void *framebuffer, void *depthbuffer,
                   void *completion_queue, void *completion_message);
long func_8002FEA0(ProbeMessageQueue *queue, ProbeMessage *message, long flags); /* osRecvMesg */
void func_8006CD14(void);
u8 *func_8006B580(s32 offset);
void *func_80032D94(void *dst, const void *src, u32 size);
void func_80060BC8(u16 *dst, u16 *src, u32 *w, u32 *h, u8 sx, u8 sy);
Gfx *func_8005CA84(Gfx *display);
Gfx *func_8007E0BC(Gfx *gfx);
Gfx *func_8007C590(Gfx *gfx);
void func_80055F08(void);
void func_80055A68(void);
Gfx *func_8007D5B8(Gfx *gfx);
void func_800565F0(void);
void *func_80056FFC(void *arg0);
void func_80072E00(void);
void func_8005D700(void **p);
Gfx *func_8005C6FC(Gfx *gfx);
Gfx *func_8007F350(Gfx *gfx);
void func_80055758(void *object);
void func_8006EE28(void);

s32 func_8005F7E0(RenderContext *task) {
    Gfx *gfx;
    u8 mode;

    D_80165A10 = task;
    if (D_8013B748 == 0) {
        return -1;
    }
    if (task->type >= 3) {
        return -1;
    }
    gfx = D_80165A18[D_8013B740];
    mode = func_8005C880();
    gSPSegment(gfx++, 0, 0);
    gSPSegment(gfx++, 1, func_800340F0(D_8013B748));
    gSPDisplayList(gfx++, D_010000B8);
    gSPDisplayList(gfx++, D_01000010);
    func_8005D79C();
    if (D_8013B744 != 0) {
        gDPSetScissorFrac(gfx++, D_8013B7A8 * 4.0f, D_8013B7AC * 4.0f, D_8013B7B0 * 4.0f, D_8013B7B4 * 4.0f);
    }
    func_8006EDCC();
    switch (D_80169A44) {
    case 3:
    case 4:
    case 5:
        gSPDisplayList(gfx++, D_010000B8);
        if (D_8013B75C != 0) {
            gDPSetPrimColor32(gfx++, 0xFFFFFF80);
            if (D_8013B763 != 0 || D_8013B764 != 0) {
                gSPDisplayList(gfx++, D_8013B728);
                gfx = func_80060048(gfx, D_80169A54, 0, 0x22, D_80169A4C, D_80169A50, D_8013B763,
                                    D_8013B764, D_80169A48, D_80169A48, 0);
            }
            if (D_8013B761 != 0) {
                gfx = func_8005C790(gfx, 0, 0, 0, D_8013B761);
            }
            if (D_8013B758 == 0) {
                D_8013B75C = 0;
                func_8006C508(2);
                func_8006B6F4(D_80169A54);
                D_80169A54 = 0;
            }
        } else {
            gfx = func_8005BB94(gfx);
            gfx = func_80069D78(gfx);
            gfx = func_80069F14(gfx);
            func_800610CC();
            func_800629AC();
            func_8006EA40((unsigned char *)&D_801E4E48);
            func_8007BD30();
            func_8008C2E0();
            func_8006EA64(0, 0, &D_801E4E48);
            func_800611F8(2, D_801E4E48.lists[D_801E4E48.frame]);
            func_800611F8(7, D_801E4E48.lists2[D_801E4E48.frame]);
            gfx = func_800610F4(gfx);
            if (D_8013B758 != 0) {
                D_8013B75C = 1;
                gDPFullSync(gfx++);
                gSPEndDisplayList(gfx++);
                func_8006B7F8(D_80165A18[D_8013B740], (s32)((gfx - D_80165A18[D_8013B740]) * sizeof(Gfx)), 0, 0,
                              D_80165A10->frame->framebuffer, D_80165A10->depthbuffer, &D_801D8590, 0);
                func_8002FEA0(&D_801D8590, 0, 1);
                func_8006CD14();
                D_80169A54 = func_8006B580(1);
                D_80169A4C = 320;
                D_80169A50 = 240;
                D_80169A48 = D_8013B762;
                func_80032D94(D_80169A54, D_80165A10->frame->framebuffer, 320 * 240 * 2);
                if (D_80169A48 != 1) {
                    func_80060BC8((u16 *)D_80169A54, (u16 *)D_80169A54, &D_80169A4C, &D_80169A50, D_80169A48,
                                  D_80169A48);
                }
                func_8006C508(1);
                return -1;
            }
        }
        if (D_80169A44 == 3) {
            gfx = func_8005CA84(gfx);
            gfx = func_8007E0BC(gfx);
            gfx = func_8007C590(gfx);
        }
        switch (D_80169A44) {
        case 3:
            func_80055F08();
            func_80055A68();
            break;
        case 4:
            gfx = func_8007D5B8(gfx);
            func_800565F0();
            break;
        }
        break;
    case 6:
    case 7:
    case 8:
        gfx = func_80056FFC(gfx);
        break;
    case 9:
        func_800610CC();
        func_8006EA40((unsigned char *)&D_801E4E48);
        func_80072E00();
        func_8006EA64(0, 0, &D_801E4E48);
        func_800611F8(2, D_801E4E48.lists[D_801E4E48.frame]);
        func_800611F8(7, D_801E4E48.lists2[D_801E4E48.frame]);
        gfx = func_800610F4(gfx);
        func_80055A68();
        break;
    case 10:
        gSPDisplayList(gfx++, D_010000B8);
        gfx = func_8005BB94(gfx);
        gfx = func_80069D78(gfx);
        gfx = func_80069F14(gfx);
        func_800610CC();
        func_800629AC();
        gfx = func_800610F4(gfx);
        break;
    }
    func_8005D700((void **)&gfx);
    if (mode == 3) {
        gfx = func_8005C6FC(gfx);
    }
    gfx = func_8007F350(gfx);
    if (mode == 2) {
        gfx = func_8005C6FC(gfx);
    }
    if (D_80169A44 == 3 || D_80169A44 == 9 || D_80169A44 == 4) {
        func_8006EA40((unsigned char *)&D_801D8FD0);
        func_80055758(&D_801D8FD0);
        gfx = func_8006EA64(gfx, 0, &D_801D8FD0);
    }
    func_8006EE28();
    if (mode == 1) {
        gfx = func_8005C6FC(gfx);
    }
    gDPFullSync(gfx++);
    gSPEndDisplayList(gfx++);
    func_8006B7F8(D_80165A18[D_8013B740], (s32)((gfx - D_80165A18[D_8013B740]) * sizeof(Gfx)), 0, 1,
                  D_80165A10->frame->framebuffer, D_80165A10->depthbuffer, 0, 0);
    D_8013B740 = (D_8013B740 + 1) & 1;
    return 0;
}
