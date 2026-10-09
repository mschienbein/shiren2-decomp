#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;
typedef double f64;

typedef struct { u32 w0; u32 w1; } Gfx;
typedef struct { s32 m[4][4]; } Mtx;
typedef struct { s16 ob[3]; u16 flag; s16 tc[2]; u8 cn[4]; } Vtx;

/* func_80059624 is canonically typed with Vec3i, but the camera vector it copies
   (D_80165324) holds three floats; this view reads the copied words as floats. */
typedef struct {
    s32 a;
    s32 b;
    s32 c;
} Vec3i;
typedef union {
    Vec3i words;
    struct {
        f32 x;
        f32 y;
        f32 z;
    } f;
} CameraAngles;

typedef struct {
    void *pixels_00;
    void *palette_04;
    u16 width_08;
    u16 height_0A;
    u16 pad0C;
    s16 x_0E;
    s16 y_10;
    u16 w_12;
    u16 h_14;
} Backdrop;

/* One horizontal gradient band edge: screen row and RGB. */
typedef struct {
    u16 y;
    u8 rgb[3];
    u8 pad;
} Band;

typedef struct {
    Backdrop *backdrop_00;
    Band upper_04;
    Band lower_0A;
    u32 flags_10;
} Sky;

extern u32 D_8016DB18;
extern Sky *D_8016DB50;
extern s32 D_801D2BF4;
extern s32 D_801DEAAC;
/* Fog/background colour components (whole s32 objects; the low byte is the value). */
extern s32 D_801DE9AC, D_801DE9B0, D_801E4E78;
extern Mtx D_80169A60;
extern Mtx D_80169AA0;
/* Render-state display list run before the gradient triangles. */
Gfx D_8013B888[10] = {
    { 0xE7000000, 0x00000000 }, /* G_RDPPIPESYNC */
    { 0xD9000000, 0x00000000 }, /* G_GEOMETRYMODE clear all */
    { 0xD9FFFFFF, 0x00200004 }, /* G_GEOMETRYMODE set shade, shade-smooth */
    { 0xD7000000, 0x00000000 }, /* G_TEXTURE off */
    { 0xE3000A01, 0x00000000 }, /* G_SETOTHERMODE_H cycle type 1-cycle */
    { 0xE3000800, 0x00000000 }, /* G_SETOTHERMODE_H pipeline mode */
    { 0xE3001801, 0x00000040 }, /* G_SETOTHERMODE_H color dither */
    { 0xE200001C, 0x0C084000 }, /* G_SETOTHERMODE_L render mode */
    { 0xFCFFFFFF, 0xFFFE793C }, /* G_SETCOMBINE shade */
    { 0xDF000000, 0x00000000 }, /* G_ENDDL */
};

/* Render-state display list run before the backdrop image. */
Gfx D_8013B8D8[4] = {
    { 0xE7000000, 0x00000000 }, /* G_RDPPIPESYNC */
    { 0xFCFFFFFF, 0xFFFCF279 }, /* G_SETCOMBINE texture */
    { 0xE200001C, 0x0F0A4000 }, /* G_SETOTHERMODE_L render mode */
    { 0xDF000000, 0x00000000 }, /* G_ENDDL */
};

/* Two gradient bands across the 320x240 screen; rows 2-5 and colors are patched per frame. */
static Vtx sSkyVertices[8] = {
    { { 0, 0, 0 }, 0, { 0, 0 }, { 0, 0, 0, 0xFF } },
    { { 320, 0, 0 }, 0, { 0, 0 }, { 0, 0, 0, 0xFF } },
    { { 0, 0, 0 }, 0, { 0, 0 }, { 0, 0, 0, 0xFF } },
    { { 320, 0, 0 }, 0, { 0, 0 }, { 0, 0, 0, 0xFF } },
    { { 0, 0, 0 }, 0, { 0, 0 }, { 0, 0, 0, 0xFF } },
    { { 320, 0, 0 }, 0, { 0, 0 }, { 0, 0, 0, 0xFF } },
    { { 0, 240, 0 }, 0, { 0, 0 }, { 0, 0, 0, 0xFF } },
    { { 320, 240, 0 }, 0, { 0, 0 }, { 0, 0, 0, 0xFF } },
};

Gfx *func_8006A2E0(Gfx *gdl);
void func_8002D1D0(Mtx *m, float l, float r, float b, float t, float n, float f, float scale);
void func_80033A20(Mtx *m, float x, float y, float z);
void func_80059624(Vec3i *dst);
Gfx *func_80060048(Gfx *gfx, const void *image, const void *palette, s32 format, s32 width, s32 height, s32 x, s32 y, float scaleX, float scaleY, const Gfx *extra);

#define PI 3.141592654

Gfx *func_80064444(Gfx *gdl) {
    Backdrop *backdrop;
    CameraAngles angles;
    s32 x;
    s32 y;
    s32 span;
    f32 scaleX;
    f32 scaleY;
    Sky *sky;
    s32 alpha;

    if (D_8016DB18 == 0 || D_8016DB50 == 0) {
        return func_8006A2E0(gdl);
    }
    func_8002D1D0(&D_80169A60, -160.0f, 160.0f, 120.0f, -120.0f, -100.0f, 100.0f, 1.0f);
    func_80033A20(&D_80169AA0, -160.0f, -120.0f, 0.0f);

    sky = D_8016DB50;
    sSkyVertices[0].cn[0] = sSkyVertices[1].cn[0] = sSkyVertices[2].cn[0] = sSkyVertices[3].cn[0] = sky->upper_04.rgb[0];
    sSkyVertices[0].cn[1] = sSkyVertices[1].cn[1] = sSkyVertices[2].cn[1] = sSkyVertices[3].cn[1] = sky->upper_04.rgb[1];
    sSkyVertices[0].cn[2] = sSkyVertices[1].cn[2] = sSkyVertices[2].cn[2] = sSkyVertices[3].cn[2] = sky->upper_04.rgb[2];
    sSkyVertices[4].cn[0] = sSkyVertices[5].cn[0] = sSkyVertices[6].cn[0] = sSkyVertices[7].cn[0] = sky->lower_0A.rgb[0];
    sSkyVertices[4].cn[1] = sSkyVertices[5].cn[1] = sSkyVertices[6].cn[1] = sSkyVertices[7].cn[1] = sky->lower_0A.rgb[1];
    sSkyVertices[4].cn[2] = sSkyVertices[5].cn[2] = sSkyVertices[6].cn[2] = sSkyVertices[7].cn[2] = sky->lower_0A.rgb[2];
    sSkyVertices[2].ob[1] = sSkyVertices[3].ob[1] = sky->upper_04.y;
    sSkyVertices[4].ob[1] = sSkyVertices[5].ob[1] = sky->lower_0A.y;

    /* Matrix words carry RDRAM physical addresses (KSEG0 - 0x80000000). */
    {
        Gfx *g = gdl++;
        g->w0 = 0xDA380007;
        g->w1 = (u32)&D_80169A60 - 0x80000000;
    }
    {
        Gfx *g = gdl++;
        g->w0 = 0xDA380003;
        g->w1 = (u32)&D_80169AA0 - 0x80000000;
    }
    {
        Gfx *g = gdl++;
        g->w0 = 0xDE000000;
        g->w1 = (u32)D_8013B888;
    }
    {
        Gfx *g = gdl++;
        g->w0 = 0x01008010;
        g->w1 = (u32)sSkyVertices;
    }
    {
        Gfx *g = gdl++;
        g->w0 = 0x06000602;
        g->w1 = 0x00000406;
    }
    {
        Gfx *g = gdl++;
        g->w0 = 0x06040A06;
        g->w1 = 0x0004080A;
    }
    {
        Gfx *g = gdl++;
        g->w0 = 0x06080E0A;
        g->w1 = 0x00080C0E;
    }

    if (D_801D2BF4 == 0) {
        return gdl;
    }
    backdrop = D_8016DB50->backdrop_00;
    if (backdrop == 0) {
        return gdl;
    }
    func_80059624(&angles.words);
    if (D_8016DB50->flags_10 & 1) {
        x = backdrop->x_0E * 4 + (s32)(angles.f.y * (f32)(u32)(backdrop->w_12 * 4) / (PI * 2));
    } else {
        x = backdrop->x_0E * 4;
    }
    if (D_8016DB50->flags_10 & 2) {
        s32 top = backdrop->y_10 * 4;
        s32 offset = (angles.f.x <= PI ? angles.f.x : angles.f.x - PI * 2) * 960.0 / (PI / 2);
        y = top + offset;
    } else {
        y = backdrop->y_10 * 4;
    }
    span = backdrop->w_12 << 2;
    scaleX = (f32)backdrop->w_12 / (f32)backdrop->width_08;
    scaleY = (f32)backdrop->h_14 / (f32)backdrop->height_0A;
    {
        Gfx *g = gdl++;
        g->w0 = 0xDE000000;
        g->w1 = (u32)D_8013B8D8;
    }
    alpha = D_801DEAAC;
    if (alpha != 0) {
        {
            Gfx *g = gdl++;
            g->w0 = 0xFC12FE25;
            g->w1 = 0xFFFFF3F9;
        }
        {
            Gfx *g = gdl++;
            g->w0 = 0xFB000000;
            g->w1 = ((u8)D_801DE9AC << 24) | ((u8)D_801E4E78 << 16) | ((u8)D_801DE9B0 << 8) | (alpha & 0xFF);
        }
    }
    gdl = func_80060048(gdl, backdrop->pixels_00, backdrop->palette_04, 0x15, backdrop->width_08,
                        backdrop->height_0A, x, y, scaleX, scaleY, 0);
    if (D_8016DB50->flags_10 & 1) {
        if (x > 0) {
            gdl = func_80060048(gdl, backdrop->pixels_00, backdrop->palette_04, 0x15, backdrop->width_08,
                                backdrop->height_0A, x - span, y, scaleX, scaleY, 0);
        }
        if (x + span < 0x500) {
            gdl = func_80060048(gdl, backdrop->pixels_00, backdrop->palette_04, 0x15, backdrop->width_08,
                                backdrop->height_0A, x + span, y, scaleX, scaleY, 0);
        }
    }
    return gdl;
}
