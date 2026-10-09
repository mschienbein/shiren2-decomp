#include "common.h"

typedef unsigned char u8;

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

typedef union {
    u32 word;
    struct {
        u8 r, g, b, a;
    } c;
} Color6F250;

/* Combiner inputs in gDPSetCombineLERP argument order. */
typedef struct {
    u8 a0, b0, c0, d0;
    u8 Aa0, Ab0, Ac0, Ad0;
    u8 a1, b1, c1, d1;
    u8 Aa1, Ab1, Ac1, Ad1;
} Combine6F250;

typedef struct {
    Color6F250 prim;
    Color6F250 env;
    Combine6F250 combine;
} Material6F250;

/* Last RDP state emitted into a display list. */
typedef struct {
    u32 cycle_type;
    u32 render_mode0;
    u32 render_mode1;
    u32 prim;
    u32 env;
} RenderState6F250;

typedef struct Block6F250 Block6F250;

typedef struct {
    u8 kind;
    u8 flags;
    u8 layer_bits;
    u8 pad3;
    u32 cycle_type;
    u32 geometry_mode;
    u32 render_mode0;
    u32 render_mode1;
    Material6F250 material;
    Block6F250 *matrix;
    Block6F250 *matrix2;
} Sprite6F250;

extern Gfx D_8013D4C8[];
extern Gfx *D_801A7140;
extern Gfx *D_801A7144;
extern Gfx *D_801A7148;
extern Gfx *D_801A714C;
extern RenderState6F250 D_801A7168;
extern RenderState6F250 D_801A717C;

extern s32 func_8006F778(u8 *p);
extern void func_80059B58(Block6F250 **out0, Block6F250 **out1, Block6F250 **out2);
extern Gfx *func_8006F7F8(Gfx *gfx, RenderState6F250 *state, Sprite6F250 *sprite);
extern Gfx *func_8006FEC4(Gfx *gfx, RenderState6F250 *state, Sprite6F250 *sprite);
extern Gfx *func_80070398(Gfx *gfx, RenderState6F250 *state, Sprite6F250 *sprite);

#define FIELD(v, s, w) (((u32)(v) & ((1 << (w)) - 1)) << (s))

#define RGBA_WORD(c) (FIELD((c).r, 24, 8) | FIELD((c).g, 16, 8) | FIELD((c).b, 8, 8) | FIELD((c).a, 0, 8))

/* gDPSetCombineLERP word fields (G_SETCOMBINE layout). */
#define CC_C0W0(a, c, Aa, Ac) (FIELD(a, 20, 4) | FIELD(c, 15, 5) | FIELD(Aa, 12, 3) | FIELD(Ac, 9, 3))
#define CC_C1W0(a, c) (FIELD(a, 5, 4) | FIELD(c, 0, 5))
#define CC_C0W1(b, d, Ab, Ad) (FIELD(b, 28, 4) | FIELD(d, 15, 3) | FIELD(Ab, 12, 3) | FIELD(Ad, 9, 3))
#define CC_C1W1(b, Aa, Ac, d, Ab, Ad) \
    (FIELD(b, 24, 4) | FIELD(Aa, 21, 3) | FIELD(Ac, 18, 3) | FIELD(d, 6, 3) | FIELD(Ab, 3, 3) | FIELD(Ad, 0, 3))

/* Emit one two-word display-list command (gbi-style block-scoped packet pointer). */
#define GFX_CMD(pkt, c, l)        \
    {                             \
        Gfx *_g = (Gfx *)(pkt);   \
        _g->w0 = (c);             \
        _g->w1 = (u32)(l);        \
    }

void func_8006F250(Sprite6F250 *sprite) {
    Gfx **head;
    Gfx *end;
    RenderState6F250 *saved;
    Gfx *gfx;
    RenderState6F250 state;
    Block6F250 *matrix;
    Material6F250 *mat;
    s32 layer;

    if (!(sprite->flags & 1)) {
        return;
    }
    layer = (sprite->layer_bits >> 1) & 7;
    /* Layers outside 0..3 go to the overlay list when one is active. */
    if ((layer < 0 ? 1 : layer >= 4) && D_801A7148 != 0) {
        head = &D_801A7148;
        end = D_801A714C;
        saved = &D_801A717C;
    } else {
        head = &D_801A7140;
        end = D_801A7144;
        saved = &D_801A7168;
    }
    gfx = *head;
    state = *saved;
    if (end != 0 && func_8006F778((u8 *)sprite) + 8 > (u32)(end - gfx)) {
        return;
    }

    GFX_CMD(gfx++, 0xDE000000, D_8013D4C8);

    if (sprite->kind < 2) {
        GFX_CMD(gfx++, 0xDA380000, sprite->matrix);
        switch (sprite->flags & 6) {
        case 2:
            func_80059B58(&matrix, 0, 0);
            break;
        case 4:
            func_80059B58(0, &matrix, 0);
            break;
        case 6:
            func_80059B58(0, 0, &matrix);
            break;
        default:
            matrix = 0;
            break;
        }
        if (matrix != 0) {
            GFX_CMD(gfx++, 0xDA380001, matrix);
        }
        if (sprite->matrix2 != 0) {
            GFX_CMD(gfx++, 0xDA380001, sprite->matrix2);
        }
    }

    GFX_CMD(gfx++, 0xD9FFFFFF, sprite->geometry_mode);

    if (state.cycle_type != sprite->cycle_type) {
        GFX_CMD(gfx++, 0xE3000A01, sprite->cycle_type);
        state.cycle_type = sprite->cycle_type;
    }
    if (state.render_mode0 != sprite->render_mode0 || state.render_mode1 != sprite->render_mode1) {
        GFX_CMD(gfx++, 0xE200001C, sprite->render_mode0 | sprite->render_mode1);
        state.render_mode0 = sprite->render_mode0;
        state.render_mode1 = sprite->render_mode1;
    }
    if (sprite->flags & 8) {
        GFX_CMD(gfx++, 0xE2001E01, 1);
    }

    mat = &sprite->material;
    if (state.prim != mat->prim.word) {
        GFX_CMD(gfx++, 0xFA000000, RGBA_WORD(mat->prim.c));
        state.prim = mat->prim.word;
    }
    if (state.env != mat->env.word) {
        GFX_CMD(gfx++, 0xFB000000, RGBA_WORD(mat->env.c));
        state.env = mat->env.word;
    }

    GFX_CMD(gfx++,
            FIELD(0xFC, 24, 8) | FIELD(CC_C0W0(mat->combine.a0, mat->combine.c0, mat->combine.Aa0, mat->combine.Ac0) |
                                           CC_C1W0(mat->combine.a1, mat->combine.c1),
                                       0, 24),
            CC_C0W1(mat->combine.b0, mat->combine.d0, mat->combine.Ab0, mat->combine.Ad0) |
                CC_C1W1(mat->combine.b1, mat->combine.Aa1, mat->combine.Ac1, mat->combine.d1, mat->combine.Ab1,
                        mat->combine.Ad1));

    switch (sprite->kind) {
    case 0:
        gfx = func_8006F7F8(gfx, &state, sprite);
        break;
    case 1:
        gfx = func_8006FEC4(gfx, &state, sprite);
        break;
    case 2:
        gfx = func_80070398(gfx, &state, sprite);
        break;
    }

    if (sprite->kind < 2) {
        GFX_CMD(gfx++, 0xD8380002, 0x40);
    }
    *head = gfx;
    *saved = state;
}
