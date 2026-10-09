#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct { s32 words[4][4]; } Mtx;
typedef union { u32 word; u8 rgba[4]; } Color;

/* 0x54-byte display record (layout shared with func_80070A0C/func_80070D30). */
typedef struct {
    u8 kind;          /* 0x00 */
    u8 flags;         /* 0x01 */
    u8 field2;        /* 0x02 */
    u8 pad3;
    u32 field4;       /* 0x04 */
    u32 draw8;        /* 0x08 */
    u32 modeC;        /* 0x0C */
    u32 mode10;       /* 0x10 */
    u32 color14;      /* 0x14 */
    Color env18;      /* 0x18 */
    u8 combine1C[16]; /* 0x1C */
    Mtx *matrix2C;    /* 0x2C */
    Mtx *matrix30;    /* 0x30: optional second matrix (func_8006F250 emits it as 0xDA380001) */
    f32 field34;      /* 0x34 */
    u8 field38;
    u8 field39;       /* 0x39 */
    u8 pad3A[2];
    s32 field3C;      /* 0x3C */
    s32 field40;      /* 0x40 */
    s32 field44;      /* 0x44 */
    s32 field48;      /* 0x48 */
    s32 field4C;      /* 0x4C */
    u8 red50;         /* 0x50 */
    u8 green51;       /* 0x51 */
    u8 blue52;        /* 0x52 */
    u8 pad53;
} Model;

typedef struct { u8 pad0[9]; s8 tilt9; u8 padA[2]; } Pose;
typedef struct { u8 flags0; u8 pad1[0xB]; Pose *poses_C; } UnitDef;
typedef struct { u8 pad0[5]; s8 height5; } GraphicHeader;
typedef struct { GraphicHeader *header; s32 field_04; } GraphicEntry;

typedef struct {
    s16 kind;          /* 0x00 */
    s16 id;            /* 0x02 */
    u8 pad4[2];
    u8 level6;         /* 0x06 */
    u8 pad7;
    u8 state8;         /* 0x08 */
    u8 pad9[3];
    s16 x;             /* 0x0C */
    s16 y;             /* 0x0E */
    s16 z;             /* 0x10 */
    u8 side12;         /* 0x12 */
    u8 slot13;         /* 0x13 */
    f32 field14;       /* 0x14 */
    s32 frame18;       /* 0x18 */
    f32 scale1C;       /* 0x1C */
    u8 pad20[0x14];
    s8 field34;        /* 0x34 */
    u8 pad35[2];
    u8 field37;        /* 0x37 */
    f32 field38;       /* 0x38 */
    u16 type3C;        /* 0x3C */
    u8 pad3E;
    u8 pose3F;         /* 0x3F */
    u8 pad40;
    u8 field41;        /* 0x41 */
    u8 pad42[2];
    u8 field44;        /* 0x44 */
    u8 field45;        /* 0x45 */
    union { u16 pair; u8 v[2]; } fade46; /* 0x46 */
    u8 bright48;       /* 0x48 */
    u8 pad49;
    s16 field4A;       /* 0x4A */
    UnitDef *def4C;    /* 0x4C */
    u8 pad50[0xC];
    Model model;       /* 0x5C */
} Unit;

/* Display-record cursors reset here each frame. D_8013D8C4 must be defined in
 * this file: gas 2.9.1 fills the jal delay slot at 0x8007ACC0 with its %lo
 * store only for a symbol defined in the same translation unit. */
Model *D_8013D8C0 = 0;
Model *D_8013D8C4 = 0;
extern s32 D_8013D8D0;
extern s32 D_8013D8D8;
extern s32 D_8013D8DC;
extern s32 D_8013D900;
extern s32 D_8013D448;
extern s32 D_801D255C;
extern s32 D_801DEAAC;
extern s32 D_801DE9AC;
extern s32 D_801DE9B0;
extern s32 D_801E4E78;
extern Model D_801D4D20[];
extern Model D_801DFF84[];
extern Unit *D_801A76B0[];
extern Unit D_801DEAB4[];
extern u8 D_801E4E48[];

typedef struct { s16 x; s16 z; s16 pad; } Pt;
typedef struct { s8 count; u8 pad1; Pt a[3]; Pt b[3]; u8 pad26[0x8]; } Zone;
extern Zone D_801A79E8[];

s32 func_80074114(void);
s32 func_800627C4(void);
s32 func_800625FC(s32 x, s32 y);
s32 func_80041FF8(void);
s32 func_8004217C(void);
Unit *func_8007946C(s32 side, s32 slot);
s32 func_80070598(void *list, void *value);
GraphicEntry *func_80074784(s32 kind, s32 level);
s32 func_80076E74(s32 arg0, s32 arg1);
s32 func_800706CC(Model *desc, s32 flag, Mtx *src, f32 dist, f32 angle, f32 height, f32 scale,
                  f32 scaleMul, s32 alpha, s32 mode);
s32 func_80070A0C(Model *model, Mtx *parent, f32 height, s32 frame, s32 alpha);
s32 func_80070D30(Model *model, Mtx *parent, f32 height, s32 frame, s32 alpha, s32 index, s32 flags);

/* Fade product scaled to 0..255. */
#define FADE(u) ((u)->fade46.v[0] * (u)->fade46.v[1] / 255)
/* One four-byte group of the color-combiner setup. */
#define COMBINE(u, i, a, b, c, d) \
    ((u)->model.combine1C[(i)] = (a), (u)->model.combine1C[(i) + 1] = (b), \
     (u)->model.combine1C[(i) + 2] = (c), (u)->model.combine1C[(i) + 3] = (d))

void func_8007AC64(void) {
    Unit **p;
    Unit *u;
    Unit *other;
    Zone *zone;
    GraphicEntry *gfx;
    s32 mode;
    s32 idx;
    f32 limit;
    s32 flags;
    s32 v;
    s32 maxv; /* brightness ceiling, reloaded every iteration */
    s32 alpha;
    f32 tilt;
    f32 lift;
    f32 ground;
    f32 h0;
    f32 scale;
    f32 size;
    s32 i;
    Unit *units;
    Unit *leader;

    D_8013D8C0 = D_801D4D20;
    D_8013D8C4 = D_801DFF84;
    idx = func_80074114();
    switch ((u32)func_800627C4()) {
    case 2:
        limit = -4.0f;
        mode = 0;
        break;
    case 4:
        limit = -8.0f;
        mode = 1;
        break;
    case 1:
        limit = -15.0f;
        mode = 0;
        break;
    case 3:
    default:
        limit = -8.0f;
        mode = 0;
        break;
    }

    for (p = D_801A76B0; *p != 0; p++) {
        (*p)->model.flags = 0;
        (*p)->model.kind = 1;
        (*p)->model.field2 = 0;
        (*p)->model.flags &= 0xF9;
        (*p)->model.matrix30 = 0;
        (*p)->model.flags = 0x10;
        if ((*p)->state8 == 0) {
            (*p)->model.flags |= 1;
        }
        maxv = 0x60;
        if (D_8013D8D0 == 1) {
            (*p)->bright48 = ((*p)->bright48 + 0x18 < maxv) ? (*p)->bright48 + 0x18 : maxv;
        } else {
            switch ((*p)->kind) {
            case 3:
            case 5:
                flags = 0x4000;
                break;
            case 1:
            case 4:
                u = &D_801DEAB4[(*p)->slot13];
                if (u->id != -1) {
                    flags = func_800625FC(u->x >> 7, u->z >> 7);
                } else {
                    flags = 0;
                }
                break;
            case 2:
                flags = 0;
                break;
            case 0:
            default:
                flags = func_800625FC((*p)->x >> 7, (*p)->z >> 7);
                break;
            }
            if ((u8)func_80041FF8() == 0 && func_8004217C() == 0
                && ((flags & 0x4000) || ((flags & 0x20100000) && func_800627C4() == 3))) {
                (*p)->bright48 = ((*p)->bright48 + 0x18 < maxv) ? (*p)->bright48 + 0x18 : maxv;
            } else {
                v = (*p)->bright48 - 0x18;
                if (v < 0) {
                    v = 0;
                }
                (*p)->bright48 = v;
            }
        }
        (*p)->model.field39 = (*p)->bright48;
        (*p)->model.draw8 = 0x200005;
        (*p)->model.field4 = 0x100000;
        (*p)->model.field48 = (*p)->type3C;
        (*p)->model.field4C = (*p)->pose3F;
        (*p)->model.red50 = (*p)->field41;
        (*p)->model.green51 = (*p)->field44;
        (*p)->model.blue52 = (*p)->field45;
        (*p)->model.field3C = 0;
        (*p)->model.field40 = 0;
        if ((*p)->field37 == 1 && (other = func_8007946C((*p)->side12, (*p)->slot13)) != 0
            && other->id != -1) {
            (*p)->model.field44 = other->field34 + (*p)->field34 * 2;
        } else {
            (*p)->model.field44 = (*p)->field34;
        }

        if ((*p)->def4C->flags0 & 0x20) {
            switch ((*p)->type3C) {
            case 2:
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104F50;
                (*p)->model.color14 = 0;
                (*p)->model.env18.word = (u8)FADE(*p);
                COMBINE(*p, 0, 3, 5, 1, 5);
                COMBINE(*p, 4, 1, 7, 5, 7);
                COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                COMBINE(*p, 12, 7, 7, 7, 0);
                break;
            case 0x114:
            case 0x181:
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104B50;
                (*p)->model.color14 = 0xF9DE7A00;
                (*p)->model.env18.word = (u8)FADE(*p) | 0xD84E0000;
                COMBINE(*p, 0, 3, 5, 1, 5);
                COMBINE(*p, 4, 1, 7, 5, 7);
                COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                COMBINE(*p, 12, 7, 7, 7, 0);
                break;
            case 0x13B:
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104B50;
                (*p)->model.color14 = 0xA09B9B00;
                (*p)->model.env18.word = (u8)FADE(*p) | 0x3C323200;
                COMBINE(*p, 0, 3, 5, 1, 5);
                COMBINE(*p, 4, 1, 7, 5, 7);
                COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                COMBINE(*p, 12, 7, 7, 7, 0);
                break;
            case 0x13A:
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104B50;
                (*p)->model.color14 = 0xFFFFFF00;
                (*p)->model.env18.word = (u8)FADE(*p) | 0x73A0F100;
                COMBINE(*p, 0, 3, 5, 1, 5);
                COMBINE(*p, 4, 1, 7, 5, 7);
                COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                COMBINE(*p, 12, 7, 7, 7, 0);
                break;
            case 3:
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104B50;
                (*p)->model.color14 = 0xFFFFFF00;
                (*p)->model.env18.word = (u8)FADE(*p) | 0xFFFFFF00;
                COMBINE(*p, 0, 3, 5, 1, 5);
                COMBINE(*p, 4, 1, 7, 5, 7);
                COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                COMBINE(*p, 12, 7, 7, 7, 0);
                break;
            case 0x162:
            default:
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104B50;
                (*p)->model.color14 = 0xFFFFFF00;
                (*p)->model.env18.word = (u8)FADE(*p) | 0x10108F00;
                COMBINE(*p, 0, 3, 5, 1, 5);
                COMBINE(*p, 4, 1, 7, 5, 7);
                COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                COMBINE(*p, 12, 7, 7, 7, 0);
                break;
            }
        } else {
            if ((*p)->fade46.pair == 0xFFFF) {
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x113078;
                (*p)->model.env18.rgba[3] = 0xFF;
                if ((*p)->model.env18.word & 0xFFFFFF00) {
                    COMBINE(*p, 0, 1, 0x1F, 5, 0x1F);
                    COMBINE(*p, 4, 7, 7, 7, 1);
                    COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                    COMBINE(*p, 12, 7, 7, 7, 0);
                } else {
                    COMBINE(*p, 0, 3, 1, 10, 1);
                    COMBINE(*p, 4, 7, 7, 7, 1);
                    COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                    COMBINE(*p, 12, 7, 7, 7, 0);
                }
            } else {
                (*p)->model.modeC = 0xC080000;
                (*p)->model.mode10 = 0x104B50;
                (*p)->model.env18.rgba[3] = FADE(*p);
                if ((*p)->model.env18.word & 0xFFFFFF00) {
                    COMBINE(*p, 0, 1, 0x1F, 5, 0x1F);
                    COMBINE(*p, 4, 1, 7, 5, 7);
                    COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                    COMBINE(*p, 12, 7, 7, 7, 0);
                } else {
                    COMBINE(*p, 0, 3, 1, 10, 1);
                    COMBINE(*p, 4, 1, 7, 5, 7);
                    COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                    COMBINE(*p, 12, 7, 7, 7, 0);
                }
            }
            if (D_801DEAAC != 0) {
                /* Low bytes of the big-endian color words. */
                (*p)->model.env18.rgba[0] = ((u8 *)&D_801DE9AC)[3];
                (*p)->model.env18.rgba[1] = ((u8 *)&D_801E4E78)[3];
                (*p)->model.env18.rgba[2] = ((u8 *)&D_801DE9B0)[3];
                if ((*p)->fade46.pair == 0xFFFF) {
                    COMBINE(*p, 0, 1, 0x1F, 5, 0x1F);
                    COMBINE(*p, 4, 7, 7, 7, 1);
                    COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                    COMBINE(*p, 12, 7, 7, 7, 0);
                } else {
                    COMBINE(*p, 0, 1, 0x1F, 5, 0x1F);
                    COMBINE(*p, 4, 1, 7, 5, 7);
                    COMBINE(*p, 8, 0x1F, 0x1F, 0x1F, 0);
                    COMBINE(*p, 12, 7, 7, 7, 0);
                }
            }
        }

        func_80070598(D_801E4E48, &(*p)->model);

        if ((*p)->field38 != 0.0f) {
            switch ((*p)->kind) {
            case 0:
                gfx = func_80074784((*p)->id, (*p)->level6);
                tilt = (*p)->field14;
                if ((*p)->field44 == 1) {
                    lift = gfx->header->height5;
                } else {
                    lift = gfx->header->height5 * 0.5f;
                }
                zone = &D_801A79E8[*p - D_801DEAB4];
                if (zone->count == 1) {
                    h0 = func_80076E74(zone->a[0].x, zone->a[0].z);
                    if (h0 == (f32)func_80076E74(zone->b[0].x, zone->b[0].z)) {
                        ground = h0;
                    } else {
                        ground = func_80076E74((*p)->x >> 7, (*p)->z >> 7);
                    }
                } else {
                    ground = func_80076E74((*p)->x >> 7, (*p)->z >> 7);
                }
                alpha = (*p)->fade46.v[1] * (*p)->fade46.v[0] / 255;
                size = (*p)->field38;
                scale = (*p)->scale1C;
                if (func_800706CC(D_8013D8C0, mode, (*p)->model.matrix2C, tilt, ground, lift,
                                  size, scale, alpha, -1) == 0) {
                    func_80070598(D_801E4E48, D_8013D8C0);
                    D_8013D8C0++;
                }
                break;
            case 3:
            case 4:
                tilt = -(*p)->def4C->poses_C[(*p)->pose3F].tilt9;
                lift = tilt * 0.4f;
                ground = func_80076E74((*p)->x >> 7, (*p)->z >> 7);
                alpha = (*p)->fade46.v[1] * (*p)->fade46.v[0] / 255;
                scale = (*p)->scale1C;
                switch ((*p)->type3C) {
                case 0x43: case 0x44: case 0x45: case 0x46: case 0x47: case 0x48: case 0x49:
                case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
                case 0x51: case 0x52:
                case 0x134:
                    size = (*p)->scale1C;
                    if (func_800706CC(D_8013D8C0, mode, (*p)->model.matrix2C, tilt, ground, lift,
                                      size, scale, alpha, (*p)->type3C) == 0) {
                        func_80070598(D_801E4E48, D_8013D8C0);
                        D_8013D8C0++;
                    }
                    break;
                default:
                    size = (*p)->field38;
                    if (func_800706CC(D_8013D8C0, mode, (*p)->model.matrix2C, tilt, ground, lift,
                                      size, scale, alpha, -1) == 0) {
                        func_80070598(D_801E4E48, D_8013D8C0);
                        D_8013D8C0++;
                    }
                    break;
                }
                break;
            }
        }

        if (D_8013D900 == 1 && (*p)->kind == 0
            && ((f32)((*p)->y >> 2) <= limit || (*p)->id == 0x54)
            && (func_800625FC((*p)->x >> 7, (*p)->z >> 7) & 0x2000)) {
            if (func_80070A0C(D_8013D8C0, (*p)->model.matrix2C, limit, (*p)->frame18,
                              (*p)->fade46.v[1]) == 0) {
                func_80070598(D_801E4E48, D_8013D8C0);
                D_8013D8C0++;
            }
        }
    }

    /* Shadow under the leader unit. */
    units = D_801DEAB4;
    leader = &units[idx];
    if (leader->id != -1 && D_801DEAB4[idx].state8 != 1
        && D_801DEAB4[idx].model.env18.rgba[3] != 0) {
        u = leader;
        if (D_8013D8D8 == 1) {
            if (func_80070D30(D_8013D8C4, u->model.matrix2C, u->y >> 2, u->frame18, 0xFF,
                              u->field4A, 0) == 0) {
                func_80070598(D_801E4E48, D_8013D8C4);
                D_8013D8C4++;
            }
        } else if (D_8013D8DC == 1) {
            for (i = 0; i < 4; i++) {
                if (func_80070D30(D_8013D8C4, u->model.matrix2C, u->y >> 2, u->frame18, 0xFF,
                                  i * 2 + 1, 0) == 0) {
                    func_80070598(D_801E4E48, D_8013D8C4);
                    D_8013D8C4++;
                }
            }
        }
    }
    if (D_8013D448 != 0) {
        D_801D255C = D_801DEAB4[idx].model.field34;
    }
}
