#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef union { s32 word; struct { u16 x, y; } p; } Pos;

/* One map cell of a movement path: tile coordinates plus a state flag. */
typedef struct {
    s16 x;
    s16 y;
    s16 state;
} PathCell;

/* 0x2E-byte movement group record (D_801A79E8). */
typedef struct {
    s8 count;
    u8 pad_01;
    PathCell from[3];
    PathCell to[3];
    s16 anim_26[4];
} MoveGroup;

/* 0xB0-byte unit record (D_801DEAB4). */
typedef struct {
    u8 pad_00[2];
    s16 kind_02;
    u8 pad_04[5];
    u8 facing_09;
    u8 pad_0A[2];
    s16 x_0C;
    s16 z_0E;
    s16 y_10;
    u8 pad_12[0x2A];
    u16 sprite_3C;
    u8 pad_3E[0xC];
    s16 anim_4A;
    u8 pad_4C[0xB0 - 0x4C];
} MoveUnit;

extern MoveGroup D_801A79E8[];
extern MoveUnit D_801DEAB4[];

s32 func_80074114(void);
void func_80061820(s32 x, s32 y);
void func_80061908(Pos x, Pos y);
void func_80061A20(f32 progress);
s32 func_80076C34(s32 arg0, s32 arg1, s32 arg2);
float func_80032360(float x);
s32 func_800429E0(s32 id);
s32 func_80042A08(s32 value);
s32 func_800625FC(s32 a, s32 b);
s32 func_80076044(s32 index, s32 resource, s32 mode, s32 value, s32 enabled);

void func_80076F48(s32 step)
{
    s32 focus = func_80074114();
    s32 i;

    for (i = 0; i < 30; i++) {
        MoveGroup *g = &D_801A79E8[i];
        MoveUnit *u = &D_801DEAB4[i];
        s32 hit;
        s32 prev;
        s32 cur;
        s32 j;
        s32 n;
        s32 anim;

        hit = 0;
        if (u->kind_02 == -1 || g->count == 0) {
            continue;
        }
        prev = 0;
        cur = 8;
        n = g->count;
        for (j = 0; j < n; j++) {
            cur = (s32)((f32)(j + 1) * 8.0f / (f32)n + 0.5);
            if (cur >= step) {
                break;
            }
            prev = cur;
        }
        if (step == prev + 1 && i == focus) {
            Pos px, py;
            func_80061820(g->from[j].x, g->from[j].y);
            px.word = g->to[j].x;
            py.word = g->to[j].y;
            func_80061908(px, py);
        }
        if (step == cur) {
            if (i == focus) {
                Pos px, py;
                func_80061820(g->from[j].x, g->from[j].y);
                px.word = g->to[j].x;
                py.word = g->to[j].y;
                func_80061908(px, py);
                func_80061A20(100.0f);
            }
            u->x_0C = (g->to[j].x << 7) + 0x40;
            u->y_10 = (g->to[j].y << 7) + 0x40;
            u->z_0E = func_80076C34(i, 0, j);
        } else {
            f32 t = (f32)(step - prev) / (f32)(cur - prev);
            s32 a, b;
            f32 z;

            u->x_0C = (s32)((f32)((g->to[j].x - g->from[j].x) << 7) * t + (f32)((g->from[j].x << 7) + 0x40));
            u->y_10 = (s32)((f32)((g->to[j].y - g->from[j].y) << 7) * t + (f32)((g->from[j].y << 7) + 0x40));
            if (i == focus) {
                func_80061A20(t * 100.0);
            }
            a = func_80076C34(i, 0, j);
            b = func_80076C34(i, 1, j);
            if (g->to[j].state != g->from[j].state) {
                z = func_80032360(t * 3.141592654) * 32.0f * 4.0f;
            } else {
                z = (f32)(a - b) * t + (f32)b;
            }
            u->z_0E = (s32)z;
        }
        if (!func_800429E0(i) && !func_80042A08(i)) {
            if ((func_800625FC(g->from[j].x, g->from[j].y) & 0x80)
                || (func_800625FC(g->to[j].x, g->to[j].y) & 0x80)) {
                func_80076044(i, u->sprite_3C, 2, 0, 1);
                hit = 1;
            }
        }
        if (step == cur) {
            if (step != 8) {
                if ((func_800625FC(g->from[j + 1].x, g->from[j + 1].y) & 0x80)
                    || (func_800625FC(g->to[j + 1].x, g->to[j + 1].y) & 0x80)) {
                    hit = 0;
                }
            }
            if (hit == 1) {
                s32 frame;
                s32 flags;

                switch (u->facing_09) {
                case 2:
                    if (u->kind_02 == 0x17) {
                        frame = 0x24;
                        flags = 1;
                    } else {
                        frame = 8;
                        flags = 3;
                    }
                    break;
                case 1:
                    if (u->kind_02 == 0x17) {
                        frame = 0x22;
                        flags = 0;
                    } else {
                        frame = 8;
                        flags = 3;
                    }
                    break;
                case 0:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                default:
                    frame = 8;
                    flags = 3;
                    break;
                }
                func_80076044(i, u->sprite_3C, 1, frame, flags);
            }
        }
        if (step == 8) {
            anim = g->anim_26[3];
        } else {
            anim = g->anim_26[j];
        }
        if (anim != -1) {
            u->anim_4A = anim;
        }
    }
}
