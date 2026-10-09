#include "common.h"

typedef unsigned char u8;

extern s32 D_8016DD18;
extern s32 D_8016DD1C;
extern s32 D_8016DD20;
u8 D_8013C490 = 1;
typedef struct { s32 field_00; void *field_04, *field_08, *field_0C; } Tracks;
typedef struct { u8 pad_00[0x30]; Tracks *tracks_30; } Node;
typedef struct { Node ***groups; u32 count; } Model;
typedef struct { u8 bytes[0x4C]; } Animation;
typedef struct { Node *node; Animation animation; } ActiveNode;
extern Model *D_801D2558;
extern void *D_801D85A8;
extern void *D_801D2C08;
extern ActiveNode *D_801D2C00;
extern u8 D_8016DC10;
extern void *func_80062CCC(void *value, void *base, u32 tag);
extern void func_80062D10(Node **head, void *base, u8 segment);
extern void func_80063F40(Animation *animation);

typedef struct { u8 kind, subtype, flags; } Cell;
typedef struct { u8 active, count, flags; u8 payload[0x405]; } SceneChunk;
typedef struct { u8 active; u8 chunk; u8 payload[0x12]; } SceneObject;
extern u8 D_8016DC13;
extern u8 D_8016DC14;
extern SceneObject *D_801D2560;
extern SceneChunk *D_801D40D4;
extern Cell *D_801E02A4;
void func_800684E8(u8 x0, u8 y0, u8 x1, u8 y1);
void func_80068168(u8 chunk, u8 x, u8 y);

void func_80067804(void)
{
    D_8013C490 = 1;
}

void func_80067814(s32 enable)
{
    D_8016DD18 = enable != 0;
    D_8013C490 = 1;
}

void func_80067830(s32 mode)
{
    D_8016DD1C = (mode & 1) << 2;
    D_8016DD20 = (mode << 1) & 4;
    D_8013C490 = 1;
}

void func_80067860(void)
{
    Node ***group;
    Node ***group_end;
    D_801D2558->groups = func_80062CCC(D_801D2558->groups, D_801D85A8, 5);
    D_8016DC10 = 0;
    group = D_801D2558->groups;
    group_end = group + D_801D2558->count;
    for (; group < group_end; ++group) {
        if (*group != 0) {
            Node **list = func_80062CCC(*group, D_801D2C08, 6);
            void *base = D_801D2C08;
            Node *node;
            *group = list;
            func_80062D10(list, base, 6);
            for (; (node = *list) != 0; ++list) {
                Tracks *tracks = node->tracks_30;
                if (tracks != 0 && (tracks->field_04 != 0 || tracks->field_08 != 0 || tracks->field_0C != 0)) {
                    ActiveNode *entry = D_801D2C00;
                    ActiveNode *end = entry + D_8016DC10;
                    for (; entry < end; ++entry) {
                        if (entry->node == node) break;
                    }
                    if (entry >= end && D_8016DC10 < 50U) {
                        entry->node = node;
                        func_80063F40(&entry->animation);
                        ++D_8016DC10;
                    }
                }
            }
        }
    }
}

void func_800679F0(u32 x, u32 y) {
    s32 d;
    s32 n;
    u32 i;
    u32 j;
    u32 colStart;
    u32 rowStart;
    u32 k;
    s32 baseX;
    s32 baseY;
    s32 px;
    SceneChunk *p;
    SceneObject *s;
    Cell *t;

    func_800684E8(x, y, x + 10, y + 8);
    if (D_8013C490 == 0) {
        d = x - D_8016DC13;
        if (d != 0) {
            if ((u32)(d + 10) >= 21) {
                D_8013C490 = 1;
            } else {
                if (d > 0) {
                    i = D_8016DC13 % 11;
                    for (n = 0; n < d; n++) {
                        for (j = 0; j < 9; j++) {
                            D_801D40D4[j * 11 + i].active |= 1;
                        }
                        i = (i + 1) % 11;
                    }
                } else {
                    i = x % 11;
                    for (n = 0; n > d; n--) {
                        for (j = 0; j < 9; j++) {
                            D_801D40D4[j * 11 + i].active |= 1;
                        }
                        i = (i + 1) % 11;
                    }
                }
            }
        }
        d = y - D_8016DC14;
        if (d != 0) {
            if ((u32)(d + 8) >= 17) {
                D_8013C490 = 1;
            } else {
                if (d > 0) {
                    j = D_8016DC14 % 9;
                    for (n = 0; n < d; n++) {
                        for (i = 0; i < 11; i++) {
                            D_801D40D4[j * 11 + i].active |= 1;
                        }
                        j = (j + 1) % 9;
                    }
                } else {
                    j = y % 9;
                    for (n = 0; n > d; n--) {
                        for (i = 0; i < 11; i++) {
                            D_801D40D4[j * 11 + i].active |= 1;
                        }
                        j = (j + 1) % 9;
                    }
                }
            }
        }
        colStart = 11 - x % 11;
        rowStart = 9 - y % 9;
        for (j = 0, p = D_801D40D4; j < 9; j++) {
            for (i = 0; i < 11; i++, p++) {
                d = (y + (j + rowStart) % 9) * 76 + (x + (i + colStart) % 11);
                if (D_801E02A4[d].flags & 1) {
                    p->active |= 1;
                }
            }
        }
    }
    if (D_8013C490 != 0) {
        D_8013C490 = 0;
        p = D_801D40D4;
        for (j = 0; j < 9; j++) {
            for (i = 0; i < 11; i++, p++) {
                p->active = 1;
                p->flags = 0;
                p->count = 0;
            }
        }
        for (s = D_801D2560; s < D_801D2560 + 198; s++) {
            s->active = 0;
        }
    } else {
        for (j = 0, k = 0, p = D_801D40D4; j < 9; j++) {
            for (i = 0; i < 11; i++, k++, p++) {
                if (p->active & 1) {
                    p->flags = 0;
                    p->count = 0;
                    for (s = D_801D2560; s < D_801D2560 + 198; s++) {
                        if (s->active != 0 && s->chunk == k) {
                            s->active = 0;
                        }
                    }
                }
            }
        }
    }
    colStart = x % 11;
    rowStart = y % 9;
    baseX = x - colStart;
    baseY = y - rowStart;
    p = D_801D40D4;
    for (j = 0, k = 0; j < 9; j++) {
        for (i = 0; i < 11; i++, k++, p++) {
            if (p->active & 1) {
                func_80068168(k, (px = baseX + i, i < colStart) ? (u8)(px + 11) : (u8)px,
                              (j < rowStart) ? (u8)(baseY + j + 9) : (u8)(baseY + j));
                p->active &= ~1;
            }
        }
    }
    for (j = 0; j < 9; j++) {
        t = &D_801E02A4[(y + j) * 76 + x];
        for (i = 0; i < 11; i++) {
            t->flags &= ~1;
            t++;
        }
    }
    D_8016DC13 = x;
    D_8016DC14 = y;
}
