#include "common.h"

typedef unsigned char u8;

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct {
    s32 m[4][4];
} Mtx;

/* Pair of render-mode words selected for the two render cycles. */
typedef struct {
    s32 field_0;
    u32 mode1;
    u32 mode2;
    s32 field_C;
    s32 field_10;
} RenderModes;

typedef struct Model Model;

typedef struct {
    u32 count;
    s32 field_4;
    Model **items;
} ModelList;

/* Double-buffered layer: per-frame main and secondary display-list buffers. */
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

#define G_DL_CMD 0xDE000000
#define G_ENDDL_CMD 0xDF000000
#define G_SETOTHERMODE_L_RENDERMODE 0xE200001C
#define G_MTX_CMD 0xDA380003

extern s32 D_8013D454;
extern Gfx D_8013D458[];
extern Mtx D_801A7100;
extern Gfx *D_801A7140;
extern Gfx *D_801A7144;
extern Gfx *D_801A7148;
extern Gfx *D_801A714C;
extern RenderModes D_801A7168;
extern RenderModes D_801A717C;

void func_8006EE64(ModelList *models);
void func_8006F250(Model *model);

/* GBI-style two-word command write (statement macro, like libultra's gDma1p family). */
#define gfx_set(pkt, c0, c1) { Gfx *_g = (pkt); _g->w0 = (c0); _g->w1 = (c1); }

/* Build the layer's display lists and link them from `gfx` (bounded by `gfxEnd` when non-NULL). */
Gfx *func_8006EA64(Gfx *gfx, Gfx *gfxEnd, Layer *layer)
{
    u32 i;

    if (D_8013D454 == 0) {
        return gfx;
    }
    if (layer->enabled == 0) {
        return gfx;
    }
    if (layer->length != 0) {
        D_801A7140 = layer->lists[layer->frame];
        D_801A7148 = layer->lists2[layer->frame];
        D_801A7144 = D_801A7140 + layer->length;
        D_801A714C = D_801A7148 + layer->length2;
    } else {
        if (gfx == 0) {
            return 0;
        }
        D_801A7140 = gfx;
        D_801A7144 = gfxEnd;
        D_801A7148 = 0;
        D_801A714C = 0;
    }
    func_8006EE64(&layer->models);
    gfx_set(D_801A7140++, G_DL_CMD, (u32)D_8013D458);
    gfx_set(D_801A7140++, G_SETOTHERMODE_L_RENDERMODE, 0x553078);
    if (D_801A7148 != 0) {
        gfx_set(D_801A7148++, G_DL_CMD, (u32)D_8013D458);
        gfx_set(D_801A7148++, G_SETOTHERMODE_L_RENDERMODE, 0x4049D8);
    }
    D_801A7168.field_0 = 0;
    D_801A7168.mode1 = 0x443078;
    D_801A7168.mode2 = 0x113078;
    D_801A7168.field_C = 0;
    D_801A7168.field_10 = 0;
    D_801A717C.field_0 = 0;
    D_801A717C.mode1 = 0x4049D8;
    D_801A717C.mode2 = 0x1049D8;
    D_801A717C.field_C = 0;
    D_801A717C.field_10 = 0;
    gfx_set(D_801A7140++, G_MTX_CMD, (u32)&D_801A7100);
    if (D_801A7148 != 0) {
        gfx_set(D_801A7148++, G_MTX_CMD, (u32)&D_801A7100);
    }
    for (i = 0; i < layer->models.count; i++) {
        func_8006F250(layer->models.items[i]);
    }
    if (layer->length != 0) {
        gfx_set(D_801A7140++, G_ENDDL_CMD, 0);
        if (D_801A7148 != 0) {
            gfx_set(D_801A7148++, G_ENDDL_CMD, 0);
        }
    }
    if (layer->length != 0) {
        if (gfx != 0) {
            if (gfxEnd == 0 || gfxEnd - gfx >= 3) {
                gfx_set(gfx++, G_DL_CMD, (u32)layer->lists[layer->frame]);
            }
            if (D_801A7148 != 0 && (gfxEnd == 0 || gfxEnd - gfx >= 3)) {
                gfx_set(gfx++, G_DL_CMD, (u32)layer->lists2[layer->frame]);
            }
        }
    } else {
        gfx = D_801A7140;
    }
    return gfx;
}
