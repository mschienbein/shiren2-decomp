#include "common.h"

typedef unsigned char u8;
typedef struct { u32 id, frame; u8 palette, kind, slot; } SpriteReq;
typedef struct { u8 flags; u8 pad1[15]; } SpriteHeader;
typedef struct { u8 pad0[6], width, height; signed char originX, originY; u8 padA[2]; } Sprite;
typedef struct Vertex Vertex;
typedef struct { u32 word0, word1; } Gfx;
typedef union { u32 word; struct { u8 r, g, b, a; } channels; } Color;
typedef struct { u32 unknown00; Color field04; u8 field08, field09, field0a, field0b; u8 unknown0c[4]; u8 field10, field11, field12, field13; } Material;
typedef struct { u8 unknown00[2], field02, unknown03; s32 field04; u8 unknown08[12]; Material field14; u8 unknown28[0x11]; u8 field39; u8 unknown3a[2]; s32 field3c, field40, field44; SpriteReq field48; } Object;
typedef struct { u32 unknown00, field04, field08, unknown0c; Color field10; } State;
/* libultra gbi-style packet helpers: each command gets its own block-scoped packet pointer. */
#define gSetWords(pkt, first, second) \
    { Gfx *_g = (pkt); _g->word0 = (first); _g->word1 = (second); }
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define GCCc0w0(saRGB0, mRGB0, saA0, mA0) \
    (_SHIFTL((saRGB0), 20, 4) | _SHIFTL((mRGB0), 15, 5) | _SHIFTL((saA0), 12, 3) | _SHIFTL((mA0), 9, 3))
#define GCCc1w0(saRGB1, mRGB1) (_SHIFTL((saRGB1), 5, 4) | _SHIFTL((mRGB1), 0, 5))
#define GCCc0w1(sbRGB0, aRGB0, sbA0, aA0) \
    (_SHIFTL((sbRGB0), 28, 4) | _SHIFTL((aRGB0), 15, 3) | _SHIFTL((sbA0), 12, 3) | _SHIFTL((aA0), 9, 3))
#define GCCc1w1(sbRGB1, saA1, mA1, aRGB1, sbA1, aA1) \
    (_SHIFTL((sbRGB1), 24, 4) | _SHIFTL((saA1), 21, 3) | _SHIFTL((mA1), 18, 3) | \
     _SHIFTL((aRGB1), 6, 3) | _SHIFTL((sbA1), 3, 3) | _SHIFTL((aA1), 0, 3))
extern s32 func_80071CCC(SpriteReq *, void **, void **, SpriteHeader **, Sprite **);
extern Gfx *func_80071420(Gfx *, void *, Sprite *, s32, s32, s32, Vertex **);
Gfx *func_8006FEC4(Gfx *out, State *state, Object *object) {
    void *image;
    void *palette;
    SpriteHeader *flags;
    Sprite *mode;
    Vertex *progress = 0;
    Material *material = &object->field14;
    Gfx *before;
    if (object->field39) {
        s32 kind = object->field02 & 0xe;
        if (kind == 6 || kind == 8) {
            gSetWords(out++, 0xfb000000, ((u32)material->field04.channels.r << 24) |
                (material->field04.channels.g << 16) | (material->field04.channels.b << 8) |
                (((material->field04.channels.a * (255 - object->field39)) / 255) & 255));
        }
    }
    if (func_80071CCC(&object->field48, &image, &palette, &flags, &mode) != 0) return out;
    /* gDPSetTextureLUT: IA16 when the sprite header sets 0x20, otherwise RGBA16.
       ODD_C: the explicit zero case shares the default arm; it also shapes the dispatch. */
    switch (flags->flags & 0x20) {
    case 0:
    default:
        gSetWords(out++, 0xe3001001, 0x8000);
        break;
    case 0x20:
        gSetWords(out++, 0xe3001001, 0xc000);
        break;
    }
    /* Load the palette and synchronize the RDP texture pipeline. */
    gSetWords(out++, 0xfd100000, (u32)palette);
    gSetWords(out++, 0xe8000000, 0);
    gSetWords(out++, 0xf5000100, 0x07000000);
    gSetWords(out++, 0xe6000000, 0);
    gSetWords(out++, 0xf0000000, 0x0703c000);
    gSetWords(out++, 0xe7000000, 0);
    before = out;
    out = func_80071420(out, image, mode, object->field3c, object->field40, object->field44, &progress);
    if (out == before) {
        return out;
    }
    if (object->field39) {
        gSetWords(out++, 0xe7000000, 0);
        /* gDPSetCombineLERP-style word encoding of the material's combiner inputs. */
        if (!object->field04) {
            gSetWords(out++, _SHIFTL(0xFC, 24, 8) |
                _SHIFTL(GCCc0w0(material->field08, material->field0a, 1, 5) |
                        GCCc1w0(material->field10, material->field12), 0, 24),
                GCCc0w1(material->field09, material->field0b, 7, 7) |
                GCCc1w1(material->field11, 1, 5, material->field13, 7, 7));
            state->field04 = 0x404b40;
            state->field08 = 0x104b40;
        } else {
            gSetWords(out++, _SHIFTL(0xFC, 24, 8) |
                _SHIFTL(GCCc0w0(material->field08, material->field0a, 1, 5) |
                        GCCc1w0(material->field10, material->field12), 0, 24),
                GCCc0w1(material->field09, material->field0b, 7, 7) |
                GCCc1w1(material->field11, 7, 7, material->field13, 7, 0));
            state->field04 = 0x0c080000;
            state->field08 = 0x104b40;
        }
        gSetWords(out++, 0xe200001c, state->field04 | state->field08);
        state->field10.word = material->field04.word;
        switch (object->field02 & 0xe) {
        case 4: case 2: state->field10.channels.a = object->field39; break;
        case 6: case 8: state->field10.channels.a = (material->field04.channels.a * object->field39) / 255; break;
        }
        gSetWords(out++, 0xfb000000, ((u32)state->field10.channels.r << 24) | (state->field10.channels.g << 16) |
            (state->field10.channels.b << 8) | state->field10.channels.a);
        out = func_80071420(out, image, mode, object->field3c, object->field40, object->field44, &progress);
    }
    return out;
}
