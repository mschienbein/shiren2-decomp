#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef float f32;

typedef struct { f32 x, y, z; } Vector;
/* Animation step list consumed by func_80074E70. */
typedef struct { signed char value; signed char next; u8 pad[2]; } Step;
typedef struct { s32 count; Step *steps; } Sequence;
/* Animation frame list consumed by func_80074D88. */
typedef struct { signed char id; signed char arg; s16 value; } AnimFrame;
typedef struct { s32 count; AnimFrame *frames; } Anim;
/* 0xB0-byte unit record of the D_801E02A8 table. */
typedef struct {
    u8 pad00[2]; s16 id02;
    u8 pad04[8]; s16 x0C, y0E, z10;
    u8 pad12[0xA]; f32 scale1C, scale20, pad24, angle28;
    u8 pad2C[9]; u8 mode35, mode36, pad37;
    f32 speed38;
    u8 pad3C[2]; u8 frame3E, frame3F, pad40, field41, field42, field43;
    u8 pad44[3]; u8 alpha47;
    u8 pad48[0x2C]; u8 red74, green75, blue76;
    u8 pad77[0x39];
} Actor;

extern Actor D_801E02A8[110];
extern s32 D_8013D8F8;
extern s32 D_801E4E70;
extern Sequence D_8014CF34, D_8014CF3C;
extern Anim D_8014CF9C, D_8014CFA4;

void func_800593F8(Vector *out);
s32 func_8005CB7C(void);
s32 func_8006262C(s32 a, s32 b);
/* Interface note: the caller passes the full s32 result of func_8006262C
 * (daddu a0,s1 at 0x80078A6C, no andi); with the canonical u8 parameter GCC
 * narrows at the call. The callee narrows internally (andi a0 before
 * func_800AC1AC), so the parameter is s32. */
s32 func_80041DBC(s32 arg0);
s32 func_800625FC(s32 a, s32 b);
s32 func_80078590(s32 handle, s32 kind, s32 variant, s32 row, s32 column);
s32 func_80062554(s32 x, s32 y);
void func_80074778(Actor *actor);
/* Interface note: all five results here are used unnarrowed (beql v0 at
 * 0x80078DBC, daddu v1,v0 at 0x80078DD4/0x8007909C/0x8007912C); a u8 return
 * makes GCC insert andi 0xFF. The callee's lbu yields the full value. */
s32 func_80064990(s32 row, s32 col);
s32 func_800649C0(s32 row, s32 col);
void func_80074E70(Actor *actor, Sequence *seq, s32 elapsed);
void func_80074D88(Actor *actor, Anim *anim, s32 dt);

/* Refresh the tile-object actors in the 11x10 window around the camera:
 * hide out-of-map/empty cells, (re)bind each cell's actor via func_80078590,
 * place it, fade it near the window edge and apply per-kind animation. */
void func_80078908(void)
{
    Vector cam;
    s32 camX, camZ;
    s32 x, z;
    s32 slot;
    Actor *actor;
    s32 id, kind, attr, flags;
    f32 dx, dz, d;
    s32 red, green, blue;
    s32 v, t;
    s32 r, g, b;
    s32 visible;

    func_800593F8(&cam);
    camX = (s32)cam.x >> 5;
    camZ = (s32)cam.z >> 5;
    for (x = camX - 5; x <= camX + 5; x++) {
        for (z = camZ - 5; z <= camZ + 4; z++) {
            slot = (z % 10) * 11 + x % 11;
            if ((u32)slot >= 110) continue;
            actor = &D_801E02A8[slot];
            if (func_8005CB7C() != 1 && (x >= 10 && x < 66 && z >= 10 && z < 44)) {
                id = func_8006262C(x, z);
                if (id == 0) { func_80074778(actor); continue; }
                kind = func_80041DBC(id);
                attr = func_800625FC(x, z);
                flags = attr & 0x31700000;
                if (flags == 0 && D_801E4E70 != 0) { func_80074778(actor); continue; }
                if (kind == 15) { func_80074778(actor); continue; }
                if (kind == 19) { func_80074778(actor); continue; }
                /* kind 16 cells without the 0x31700000 flags are shown only
                 * when the 0x08000000 attribute is set (and D_801E4E70 == 0). */
                visible = 0;
                if (kind == 16 && flags == 0) {
                    visible = D_801E4E70 == 0 && (attr & 0x08000000);
                    if (!visible) { func_80074778(actor); continue; }
                }
                if (actor->id02 == -1 || actor->id02 != id || actor->x0C >> 7 != x || actor->z10 >> 7 != z) {
                    slot = func_80078590(slot, kind, id, x, z);
                    if (slot == -1) continue;
                    actor = &D_801E02A8[slot];
                }
                actor->x0C = (x << 7) + 0x40;
                actor->z10 = (z << 7) + 0x40;
                if (kind == 16) actor->y0E = (func_80062554(x, z) << 2) + 4;
                else actor->y0E = func_80062554(x, z) << 2;
                if (x < camX - 4 || x > camX + 4 || z < camZ - 4 || z > camZ + 3) {
                    dx = (f32)((x << 5) + 16) - cam.x;
                    dz = (f32)((z << 5) + 16) - cam.z;
                    if (dx >= 0.0f) dx -= 128.0f;
                    else dx = -(dx - -128.0f);
                    if (dz >= 0.0f) dz -= 96.0f;
                    else dz = -(dz - -128.0f);
                    d = dz;
                    if (d < dx) d = dx;
                    if (d >= 48.0f) d = 256.0f;
                    else d *= 256.0 / 48.0;
                    if (d < 255.0f) actor->alpha47 = 255 - (s32)d;
                    else actor->alpha47 = 0;
                } else {
                    actor->alpha47 = 255;
                }
            } else {
                func_80074778(actor);
                continue;
            }
            switch (kind) {
            case 16:
                switch (id) {
                case 0xD5:
                    if (func_80064990(x, z) != 0) actor->frame3E = 1;
                    else actor->frame3E = 0;
                    break;
                case 0xE7:
                    switch (func_80064990(x, z)) {
                    case 0:
                    default: actor->frame3E = 0; break;
                    case 2: actor->frame3E = 1; break;
                    case 4: actor->frame3E = 2; break;
                    case 6: actor->frame3E = 3; break;
                    }
                    func_80074E70(actor, &D_8014CF3C, 1);
                    break;
                case 0xE6:
                    func_80074E70(actor, &D_8014CF34, 1);
                    break;
                }
                if (D_8013D8F8 == 1) {
                    switch (func_800649C0(x, z)) {
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
                    switch (func_800649C0(x, z)) {
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
                if (func_800649C0(x, z) & 0x80) {
                    r = actor->red74;
                    g = actor->green75;
                    b = actor->blue76;
                    if (r >= red) {
                        /* fade a quarter step toward the target, never below it */
                        t = r - red / 4;
                        v = (red < t) ? t : red;
                        actor->red74 = v;
                    }
                    if (g >= green) {
                        t = g - green / 4;
                        v = (green < t) ? t : green;
                        actor->green75 = v;
                    }
                    if (b >= blue) {
                        t = b - blue / 4;
                        v = (blue < t) ? t : blue;
                        actor->blue76 = v;
                    }
                } else {
                    actor->red74 = red;
                    actor->green75 = green;
                    actor->blue76 = blue;
                }
                break;
            case 2:
                if (func_80064990(x, z) != 0) {
                    actor->frame3E = 1;
                    actor->mode35 = 0;
                    actor->mode36 = 0;
                    actor->speed38 = 0.0f;
                    actor->angle28 = 4.712389f;
                    actor->y0E = (func_80062554(x, z) << 2) + 4;
                } else {
                    actor->frame3E = 0;
                    actor->mode35 = 1;
                    actor->angle28 = 0.0f;
                    actor->mode36 = 3;
                    actor->speed38 = 0.75f;
                }
                break;
            case 20:
                switch (id) {
                case 0xF5:
                    switch (func_80064990(x, z)) {
                    case 1: actor->scale1C = 0.45f; actor->scale20 = 0.56f; break;
                    case 2: actor->scale1C = 0.8f; actor->scale20 = 1.0f; break;
                    case 4:
                    default: actor->scale1C = 1.2f; actor->scale20 = 1.8f; break;
                    }
                    func_80074D88(actor, &D_8014CF9C, 1);
                    break;
                case 0xF6:
                    switch (func_80064990(x, z)) {
                    case 1: actor->scale1C = actor->scale20 = 1.4f; break;
                    case 2: actor->scale1C = actor->scale20 = 2.45f; break;
                    case 4:
                    default: actor->scale1C = actor->scale20 = 3.5f; break;
                    }
                    func_80074D88(actor, &D_8014CFA4, 1);
                    break;
                }
                break;
            default:
                actor->field41 = 0;
                actor->field43 = 0;
                actor->field42 = 0;
                break;
            }
            actor->frame3F = actor->frame3E;
        }
    }
}
