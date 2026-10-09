#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* 4-byte unit descriptor: resource id and scale in percent. */
typedef struct { u16 resource; u8 scale, unused; } Descriptor;
/* 0xB0-byte unit record of the D_801E02A8 table. */
typedef struct {
    u16 kind00;
    u8 pad02[5]; u8 enabled07;
    u8 pad08[0x14]; float scale1C, scale20, scale24, angle28;
    u8 pad2C[9]; u8 mode35, mode36, pad37;
    float speed38;
    u8 pad3C[6]; u8 variant42, pad43, alpha44, field45;
    u8 pad46[0x2E]; u8 red74, green75, blue76;
    u8 pad77[0x39];
} Actor;
/* One descriptor bank covers every index selected below (the original
 * addresses +0x1D0, +0x224 and +0x228 are elements 0x74.., 0x89 and 0x8A). */
extern Descriptor D_8013F020[];
extern Actor D_801E02A8[];
extern s32 D_8013D8F8;
extern s32 func_80074500(Actor *, s32, s32, s32, s32);
/* Interface note: both callers here (0x80078740, 0x800787E0) and the three in
 * func_80078908 use the result without narrowing (daddu v1,v0); with a u8
 * return GCC inserts andi 0xFF. The callee's lbu already yields the full-width
 * value, so the result is s32 (canonical u8 to be corrected; see results.json). */
extern s32 func_800649C0(s32 row, s32 col);
extern u32 func_8002A9B0(void);
s32 func_80078590(s32 handle, s32 kind, s32 variant, s32 row, s32 column)
{
    Descriptor *descriptor;
    Actor *actor;
    s32 terrain;
    u8 blue, green, red;
    switch (kind) {
    case 16:
        descriptor = &D_8013F020[variant];
        break;
    case 20:
        switch (variant) {
        case 0xF5: descriptor = &D_8013F020[0x89]; break;
        case 0xF6: descriptor = &D_8013F020[0x8A]; break;
        default: return -1;
        }
        break;
    case 15:
    case 19:
        return -1;
    default:
        descriptor = &D_8013F020[0x74 + kind];
        break;
    }
    actor = D_801E02A8;
    handle = func_80074500(actor, handle, 0, variant, descriptor->resource);
    if (handle == -1) return -1;
    actor += handle;
    actor->kind00 = 3;
    actor->scale1C = actor->scale20 = actor->scale24 = descriptor->scale / 100.0f;
    switch (kind) {
    case 16:
        if (variant == 0xE7) actor->mode36 = 0;
        else actor->mode36 = 2;
        actor->mode35 = 0;
        actor->speed38 = 0.0f;
        actor->angle28 = 4.712389f;
        if (D_8013D8F8 == 1) {
            terrain = func_800649C0(row, column);
            switch (terrain) {
            case 1:
            default:
                red = 255; green = red; blue = red;
                break;
            case 2:
                red = 255; green = 100; blue = 105;
                break;
            case 4:
                red = 180; green = 255; blue = red;
                break;
            case 0x81:
                red = 128; green = red; blue = red;
                break;
            case 0x82:
                red = 128; green = 50; blue = 52;
                break;
            case 0x84:
                red = 90; green = 128; blue = red;
                break;
            }
        } else {
            terrain = func_800649C0(row, column);
            switch (terrain) {
            case 1:
            case 2:
            case 4:
            default:
                red = 255; green = red; blue = red;
                break;
            case 0x81:
            case 0x82:
            case 0x84:
                red = 128; green = red; blue = red;
                break;
            }
        }
        actor->red74 = red;
        actor->green75 = green;
        actor->blue76 = blue;
        break;
    case 20:
        switch (variant) {
        case 0xF5:
            actor->mode35 = 1;
            actor->mode36 = 3;
            actor->speed38 = 1.0f;
            break;
        case 0xF6:
            actor->speed38 = 0.0f;
            actor->mode35 = 0;
            actor->mode36 = 2;
            actor->angle28 = 5.8904862f;
            break;
        }
        actor->variant42 = (func_8002A9B0() * handle) & 3;
        break;
    default:
        actor->mode35 = 1;
        actor->mode36 = 3;
        actor->speed38 = 0.75f;
        break;
    }
    actor->enabled07 = 1;
    actor->alpha44 = 255;
    actor->field45 = 0;
    return handle;
}
