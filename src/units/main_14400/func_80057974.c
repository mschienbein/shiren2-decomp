#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef float f32;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { u16 tile; u16 unk2; } TileRef;
typedef struct { s32 palette; u8 unk4[4]; u8 color[16]; } Style;
typedef struct {
    u8 unk0[0xC];
    s32 unkC;
    s32 unk10;
    Style style;
    f32 x;
    f32 y;
    f32 scaleX;
    f32 scaleY;
    u8 unk3C[4];
    s32 priority;
    s32 tile;
    s32 unk48;
    u8 unk4C;
    u8 unk4D;
    u8 unk4E[2];
} Sprite;
extern s32 D_8013A370;
extern s32 D_8013A36C;
extern s32 D_8013A294;
extern s32 D_8013A368;
extern Sprite *D_80163100;
extern u8 D_801630D8[];
extern Pos D_8013A374[];
extern Pos D_8013A4AC[];
extern Pos D_8013A58C[];
extern TileRef D_8013F24C[];
extern TileRef D_8013F2EC[];
extern u8 D_8013A6C4[];
extern u8 D_8013A6F0[];
extern s8 D_8013A6EC[];
s32 func_80042710(void);
s32 func_80042AB0(s32 id);
void func_800584EC(Sprite *s);
void func_80057F60(u32 phase, Style *style);
void func_80057FC0(s32 x, s32 y, Style *style);
s32 func_80070598(void *list, void *value);
void *func_80057974(void *arg) {
    s32 base;
    s32 n;
    Pos *pos;
    TileRef *tiles;
    s32 selected;
    s32 count;
    s32 pulse;
    s32 i;
    s32 j;
    s32 id;
    Sprite *s;
    f32 x;
    f32 y;

    selected = func_80042710();
    count = 0;

    if (D_8013A370 != selected) {
        D_8013A36C = 0;
        D_8013A370 = selected;
    }
    pulse = D_8013A36C - 0xC0;
    if (pulse < 0) pulse = -pulse;
    switch (D_8013A294) {
        case 0:
        default:
            base = 0x32;
            n = 0x27;
            pos = D_8013A374;
            tiles = D_8013F24C;
            for (i = 0; i < n; i++) {
                id = base + i;
                s = &D_80163100[count];
                func_800584EC(s);
                s->x = D_8013A58C[i].x;
                s->y = D_8013A58C[i].y;
                s->tile = 0x17C;
                s->unk48 = D_8013A6C4[i];
                count++;
                s->unk4C = 0;
                s->unk4D = 0xFE;
                if (id == selected) {
                    func_80057F60((u8)pulse, &s->style);
                } else {
                    func_80057FC0(pos[i].x, pos[i].y, &s->style);
                }
                func_80070598(D_801630D8, s);
            }
            break;
        case 1:
            base = 0x59;
            n = 0x1C;
            pos = D_8013A4AC;
            tiles = D_8013F2EC;
            for (i = 0; i < n; i++) {
                id = base + i;
                s = &D_80163100[count++];
                func_800584EC(s);
                s->x = pos[i].x;
                s->y = pos[i].y;
                s->tile = 0x17B;
                if (id == 0x6D) {
                    s->unk48 = 1;
                    s->unk4C = 1;
                } else {
                    s->unk48 = 0;
                    s->unk4C = 0;
                }
                s->unk4D = 0xFE;
                if (id == selected) {
                    func_80057F60((u8)pulse, &s->style);
                } else {
                    func_80057FC0(pos[i].x, pos[i].y, &s->style);
                }
                func_80070598(D_801630D8, s);
            }
            break;
    }
    for (j = 0; j < n; j++) {
        id = base + j;
        if (!func_80042AB0(id)) continue;
        s = &D_80163100[count++];
        func_800584EC(s);
        x = (pos[j].x - 0xA0) * 1.025f;
        y = (pos[j].y - 0xDC) * 1.025f;
        s->x = x + 160.0f;
        s->y = y + 220.0f;
        s->scaleX = 1.025f;
        s->scaleY = 1.025f;
        s->priority = 100;
        s->tile = tiles[j].tile;
        s->unk4D = 0xFE;
        s->style.color[3] = 3;
        s->style.color[4] = 1;
        s->style.color[5] = 7;
        s->style.color[6] = 3;
        s->style.color[7] = 7;
        s->style.color[11] = 3;
        s->style.color[12] = 1;
        s->style.color[13] = 7;
        s->style.color[14] = 3;
        s->unk4C = 0;
        s->style.color[0] = 0x1F;
        s->style.color[1] = 0x1F;
        s->style.color[2] = 0x1F;
        s->style.color[8] = 0x1F;
        s->style.color[9] = 0x1F;
        s->style.color[10] = 0x1F;
        s->style.color[15] = 7;
        s->unk48 = 8;
        s->unkC = 0x404340;
        s->unk10 = 0x104340;
        s->style.palette = D_8013A6F0[D_8013A368];
        func_80070598(D_801630D8, s);
        s = &D_80163100[count++];
        func_800584EC(s);
        s->x = pos[j].x;
        s->y = pos[j].y;
        s->priority = 200;
        s->tile = tiles[j].tile;
        s->unk48 = 8;
        s->unk4C = 0;
        s->unk4D = 0xFE;
        if (id == selected) {
            func_80057F60((u8)pulse, &s->style);
        } else {
            func_80057FC0(pos[j].x, pos[j].y, &s->style);
        }
        func_80070598(D_801630D8, s);
    }
    if (D_8013A6EC[D_8013A368] >= 0) {
        s = &D_80163100[count];
        func_800584EC(s);
        s->priority = 300;
        s->tile = 0x180;
        s->x = 159.0f;
        s->y = 216.0f;
        s->unk48 = D_8013A6EC[D_8013A368];
        s->unk4C = 0;
        s->unk4D = 0xFE;
        func_80070598(D_801630D8, s);
    }
    D_8013A368 = (D_8013A368 + 1) % 4;
    D_8013A36C = (D_8013A36C + 6) % 384;
    return arg;
}