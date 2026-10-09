#include "common.h"

typedef unsigned char u8;
typedef struct { u32 w0, w1; } Gfx;
typedef struct Mtx Mtx;
/* One animation frame of a texture strip: image row and palette row. */
typedef struct { u32 header; s32 image_index; s32 palette_index; } TextureFrame;
typedef struct {
    s32 field_00;
    TextureFrame *texture;  /* 0x04 */
    void *uv;               /* 0x08 */
    void *transform;        /* 0x0C */
    u32 format;             /* 0x10: bits 0-3 image format, bits 8-11 texel size */
    u32 width;              /* 0x14 */
    u32 height;             /* 0x18 */
    u8 *image;              /* 0x1C */
    s32 image_stride;       /* 0x20 */
    u8 *palette;            /* 0x24 */
    s32 palette_stride;     /* 0x28 */
} AnimationSet;
typedef struct {
    Gfx *display_list;      /* 0x00 */
    u8 field_04[0x18];
    u32 two_cycle;          /* 0x1C */
    u32 geometry_mode;      /* 0x20 */
    u32 render_mode[2];     /* 0x24 */
    u32 matrix_slot;        /* 0x2C */
    AnimationSet *animation; /* 0x30 */
} Model;
typedef struct {
    short palette_frame;    /* 0x00 */
    short image_frame;      /* 0x02 */
    float u, v;             /* 0x04, 0x08 */
    u8 field_0C[0x3C];
    Mtx *matrix;            /* 0x48 */
} State;

extern void func_80059B58(Mtx **out0, Mtx **out1, Mtx **out2);
extern s32 func_80062CF4(u32 value);

/* One display-list command. */
#define GFX(pkt, a, b) { Gfx *_g = (pkt); _g->w0 = (a); _g->w1 = (b); }
#define MAX1(x) (1 > (x) ? 1 : (x))
/* Tile mask words: log2 of the texture height (T) and width (S). */
#define TILE_WORD(tile) ((tile) | ((func_80062CF4(model->animation->height) & 0xF) << 14) | ((func_80062CF4(model->animation->width) & 0xF) << 4))
/* gDPLoadTextureBlock: load through tile 7 at img_siz, then describe render tile 0 at tile_siz. */
#define LOAD_TEXTURE_BLOCK(img_siz, tile_siz, lrs, words, line)                                       \
    GFX(gfx++, 0xFD000000 | (fmt << 21) | ((img_siz) << 19), (u32)image);                             \
    GFX(gfx++, 0xF5000000 | (fmt << 21) | ((img_siz) << 19), TILE_WORD(0x07000000));              \
    GFX(gfx++, 0xE6000000, 0);                                                                         \
    GFX(gfx++, 0xF3000000, 0x07000000 | (((lrs) < 0x7FF ? (lrs) : 0x7FF) & 0xFFF) << 12               \
        | (((0x800 + MAX1(words) - 1) / MAX1(words)) & 0xFFF));                                        \
    GFX(gfx++, 0xE7000000, 0);                                                                         \
    GFX(gfx++, 0xF5000000 | (fmt << 21) | ((tile_siz) << 19) | (((line) & 0x1FF) << 9), TILE_WORD(0));  \
    GFX(gfx++, 0xF2000000, ((((model->animation->width - 1) << 2) & 0xFFF) << 12)                     \
        | (((model->animation->height - 1) << 2) & 0xFFF));                                            \
    if (mode)                                                                                          \
        GFX(gfx++, 0xF5000000 | (fmt << 21) | ((tile_siz) << 19) | (((line) & 0x1FF) << 9),           \
            TILE_WORD(0x01000000));

Gfx *func_80062EA4(Gfx *gfx, Model *model, State *state, s32 count, void *vertices, u8 mode, s32 frame)
{
    Mtx *m0, *m1, *m2;
    u8 *palette = 0;
    u8 *image;
    u32 siz, fmt;
    u8 pushed_model = 0, pushed_state = 0;

    if (model->matrix_slot) {
        func_80059B58(&m0, &m1, &m2);
        pushed_model = 1;
        /* gSPMatrix push/mul of the selected camera matrix (physical address). */
        switch (model->matrix_slot & 3) {
        case 1: GFX(gfx++, 0xDA380000, (u32)m0 - 0x80000000); break;
        case 2: GFX(gfx++, 0xDA380000, (u32)m1 - 0x80000000); break;
        case 3: GFX(gfx++, 0xDA380000, (u32)m2 - 0x80000000); break;
        }
    }
    if (model->animation && model->animation->transform && state->matrix) {
        pushed_state = 1;
        GFX(gfx++, 0xDA380000, (u32)state->matrix - 0x80000000);
    }
    GFX(gfx++, 0xE7000000, 0); /* pipe sync */
    /* cycle type, render mode, then geometry mode without G_FOG (0x10000) */
    if (model->two_cycle) GFX(gfx++, 0xE3000A01, 0x100000)
    else GFX(gfx++, 0xE3000A01, 0)
    GFX(gfx++, 0xE200001C, model->render_mode[0] | model->render_mode[1]);
    GFX(gfx++, 0xD9010000, 0);
    GFX(gfx++, 0xD9FFFFFF, model->geometry_mode & 0xFFFEFFFF);
    if (model->animation && (model->animation->uv || model->animation->texture)) {
        if (model->animation->texture && frame >= 0) {
            if (model->animation->palette)
                palette = model->animation->palette + model->animation->texture[frame].palette_index * model->animation->palette_stride;
            image = model->animation->image + model->animation->texture[frame].image_index * model->animation->image_stride;
        } else {
            if (model->animation->palette)
                palette = model->animation->palette + state->palette_frame * model->animation->palette_stride;
            image = model->animation->image + state->image_frame * model->animation->image_stride;
        }
        switch (model->animation->format & 0xF00) {
        case 0x000:
        default: siz = 0; break;
        case 0x100: siz = 1; break;
        case 0x200: siz = 2; break;
        case 0x400: siz = 3; break;
        }
        switch (model->animation->format & 0xF) {
        case 0: fmt = 4; break;
        case 1: fmt = 3; break;
        case 2: fmt = 2; break;
        case 4: case 5: fmt = 0; break;
        default: fmt = 1; break;
        }
        if (model->animation->palette) {
            if (fmt == 2) GFX(gfx++, 0xE3001001, 0x8000); /* TLUT RGBA16 */
            /* gDPLoadTLUT_pal16 / gDPLoadTLUT_pal256 */
            switch (siz) {
            case 0:
                GFX(gfx++, 0xFD100000, (u32)palette);
                GFX(gfx++, 0xE8000000, 0);
                GFX(gfx++, 0xF5000100, 0x07000000);
                GFX(gfx++, 0xE6000000, 0);
                GFX(gfx++, 0xF0000000, 0x0703C000);
                GFX(gfx++, 0xE7000000, 0);
                break;
            case 1:
                GFX(gfx++, 0xFD100000, (u32)palette);
                GFX(gfx++, 0xE8000000, 0);
                GFX(gfx++, 0xF5000100, 0x07000000);
                GFX(gfx++, 0xE6000000, 0);
                GFX(gfx++, 0xF0000000, 0x073FC000);
                GFX(gfx++, 0xE7000000, 0);
                break;
            }
        } else {
            GFX(gfx++, 0xE3001001, 0); /* TLUT none */
        }
        switch (siz) {
        case 0:
            LOAD_TEXTURE_BLOCK(2, 0, ((model->animation->width * model->animation->height + 3) >> 2) - 1,
                               model->animation->width >> 4, ((model->animation->width >> 1) + 7) >> 3)
            break;
        case 1:
            LOAD_TEXTURE_BLOCK(2, 1, ((model->animation->width * model->animation->height + 1) >> 1) - 1,
                               model->animation->width >> 3, (model->animation->width + 7) >> 3)
            break;
        case 2:
            LOAD_TEXTURE_BLOCK(2, 2, model->animation->width * model->animation->height - 1,
                               model->animation->width * 2 / 8, (model->animation->width * 2 + 7) >> 3)
            break;
        case 3:
            LOAD_TEXTURE_BLOCK(3, 3, model->animation->width * model->animation->height - 1,
                               model->animation->width * 4 / 8, (model->animation->width * 2 + 7) >> 3)
            break;
        }
        GFX(gfx++, 0xF2000000 | ((((s32)(state->u * (model->animation->width << 2)) % (model->animation->width << 2)) & 0xFFF) << 12)
            | (((s32)(state->v * (model->animation->height << 2)) % (model->animation->height << 2)) & 0xFFF),
            ((((model->animation->width - 1) << 2) & 0xFFF) << 12) | (((model->animation->height - 1) << 2) & 0xFFF));
        /* Tile 0 scrolls by (u, v); the optional tile 1 mirrors or offsets the scroll by mode. */
        if (mode) switch (mode) {
        case 2:
            GFX(gfx++, 0xF2000000
                | ((((s32)(state->v * (model->animation->width << 2)) % (model->animation->width << 2)) & 0xFFF) << 12)
                | (((s32)((-state->u + 0.5) * (model->animation->height << 2)) % (model->animation->height << 2)) & 0xFFF),
                0x01000000 | ((((model->animation->width - 1) << 2) & 0xFFF) << 12) | (((model->animation->height - 1) << 2) & 0xFFF));
            break;
        case 3:
            GFX(gfx++, 0xF2000000
                | ((((s32)((state->u + 0.5) * (model->animation->width << 2)) % (model->animation->width << 2)) & 0xFFF) << 12),
                0x01000000 | ((((model->animation->width - 1) << 2) & 0xFFF) << 12) | (((model->animation->height - 1) << 2) & 0xFFF));
            break;
        case 4:
            GFX(gfx++, 0xF2000000
                | ((((s32)(-state->u * (model->animation->width << 2)) % (model->animation->width << 2)) & 0xFFF) << 12)
                | (((s32)((-state->v + 0.5) * (model->animation->height << 2)) % (model->animation->height << 2)) & 0xFFF),
                0x01000000 | ((((model->animation->width - 1) << 2) & 0xFFF) << 12) | (((model->animation->height - 1) << 2) & 0xFFF));
            break;
        case 1:
        default:
            GFX(gfx++, 0xF2000000
                | ((((s32)(state->u * (model->animation->width << 2)) % (model->animation->width << 2)) & 0xFFF) << 12)
                | (((s32)(state->v * (model->animation->height << 2)) % (model->animation->height << 2)) & 0xFFF),
                0x01000000 | ((((model->animation->width - 1) << 2) & 0xFFF) << 12) | (((model->animation->height - 1) << 2) & 0xFFF));
            break;
        }
    }
    if (count && vertices) /* gSPVertex(gfx++, vertices, count, 0) */
        GFX(gfx++, 0x01000000 | ((count & 0xFF) << 12) | ((count & 0x7F) << 1), (u32)vertices);
    GFX(gfx++, 0xDE000000, (u32)model->display_list);
    /* gSPPopMatrix for each matrix pushed above */
    if (pushed_state) GFX(gfx++, 0xD8380002, 0x40);
    if (pushed_model) GFX(gfx++, 0xD8380002, 0x40);
    return gfx;
}
