#include "common.h"

typedef unsigned char u8;
typedef float f32;

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

/* Prefix view of func_80071CCC's SpriteHeader: only the flags byte is read here. */
typedef struct {
    u8 flags;
} Header80070398;

typedef struct {
    char pad0[6];
    u8 unk6;
    u8 unk7;
    signed char unk8;
    signed char unk9;
} Info80070398;

typedef struct {
    u32 unk0;
    u32 unk4;
    u8 unk8;
    u8 unk9;
    u8 unkA;
} TextureSelector80070398;

typedef struct {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 scaleX;
    f32 scaleY;
    char pad3C[0x44 - 0x3C];
    TextureSelector80070398 unk44;
} Obj80070398;

s32 func_80071CCC(TextureSelector80070398 *arg0, void **texture, void **palette, Header80070398 **header, Info80070398 **info);
Gfx *func_80060230(Gfx *gfx, const void *texture, s32 format, u32 width, u32 height, s32 x, s32 y, f32 scaleX, f32 scaleY);

/* The renderer interface passes render_state; this sprite path does not inspect it. */
Gfx *func_80070398(Gfx *gfx, void *render_state, Obj80070398 *obj) {
    void *texture;
    void *palette;
    Header80070398 *header;
    Info80070398 *info;
    s32 format;
    s32 ia;
    f32 x;
    f32 y;

    if (func_80071CCC(&obj->unk44, &texture, &palette, &header, &info) != 0) {
        return gfx;
    }

    {
        Gfx *g = gfx++;

        g->w0 = 0xE3000C00;
        g->w1 = 0;
    }

    ia = header->flags & 0x20;
    if (ia == 0 || (ia & 0x20) == 0) {
        {
            Gfx *g = gfx++;

            g->w0 = 0xE3001001;
            g->w1 = 0x8000;
        }
        format = 5;
    } else {
        {
            Gfx *g = gfx++;

            g->w0 = 0xE3001001;
            g->w1 = 0xC000;
        }
        format = 4;
    }

    {
        Gfx *g = gfx++;

        g->w0 = 0xFD100000;
        /* The TLUT address is encoded into the RDP command word here. */
        g->w1 = (u32)palette;
    }
    {
        Gfx *g = gfx++;

        g->w0 = 0xE8000000;
        g->w1 = 0;
    }
    {
        Gfx *g = gfx++;

        g->w0 = 0xF5000100;
        g->w1 = 0x07000000;
    }
    {
        Gfx *g = gfx++;

        g->w0 = 0xE6000000;
        g->w1 = 0;
    }
    {
        Gfx *g = gfx++;

        g->w0 = 0xF0000000;
        g->w1 = 0x0703C000;
    }
    {
        Gfx *g = gfx++;

        g->w0 = 0xE7000000;
        g->w1 = 0;
    }

    x = (obj->unk2C - (info->unk8 + 1) * obj->scaleX) * 4.0f;
    y = (obj->unk30 - ((f32)info->unk7 - (f32)info->unk9) * obj->scaleY) * 4.0f;
    return func_80060230(gfx, texture, format, info->unk6, info->unk7, x, y, obj->scaleX, obj->scaleY);
}
