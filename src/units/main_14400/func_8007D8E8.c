#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* One 4x4-pixel 4bpp minimap cell: 4 rows of 2 bytes.
 * Composed as two words, blitted as bytes. */
typedef union {
    u32 w[2];
    u8 b[8];
} Pat8;

#define NULL 0

extern s32 D_8013DFDC;
extern u32 D_801A808C[2];    /* scratch pattern for wall cells */
extern u32 *D_801A8094;      /* -> D_801A808C */
extern u32 D_801D9358[][54]; /* map cell flags, 9-cell border */
extern u32 D_801A7F50[72];   /* 36 rows x 64 bits: cell drawn last frame */

void *func_80083908(s32 index);
s32 func_8005CB7C(void);

/* Pattern tables used by the minimap cell renderer (this file's .data). */
Pat8 D_8013E0A0 = { { 0xF00F0000, 0x0000F00F } };
Pat8 D_8013E0A8 = { { 0x00000FF0, 0x0FF00000 } };
Pat8 D_8013E0B0 = { { 0x0FF0F00F, 0xF00F0FF0 } };
Pat8 D_8013E0B8[2] = {
    { { 0xFFFFFFFF, 0xFFFFFFFF } },
    { { 0xF00F0FF0, 0x0FF0F00F } },
};
Pat8 D_8013E0C8 = { { 0x00000000, 0x00000000 } };
Pat8 D_8013E0D0 = { { 0x77777777, 0x77777777 } };
Pat8 D_8013E0D8 = { { 0xDDDDDDDD, 0xDDDDDDDD } };
Pat8 D_8013E0E0 = { { 0x0BB0BBBB, 0xBBBB0BB0 } };
Pat8 D_8013E0E8 = { { 0x08808888, 0x88880880 } };
Pat8 D_8013E0F0 = { { 0x05505555, 0x55550550 } };
Pat8 D_8013E0F8 = { { 0x03303883, 0x38830330 } };
Pat8 D_8013E100 = { { 0x30030330, 0x03303003 } };
/* Read by func_8007E0BC (texture data); owned here as part of this file's .data. */
u32 D_8013E108[20] = {
    0xCCCC0CC0, 0x00000000, 0xCCCCCCCC, 0x00000000,
    0xCCCCCCCC, 0x00000000, 0xCCCC0CC0, 0x00000000,
    0xCCCC0CC0, 0x00000000, 0xC00CC00C, 0x00000000,
    0xC00CC00C, 0x00000000, 0xCCCC0CC0, 0x00000000,
    0xAAAAAAAA, 0xAAAAAAAA, 0x99999999, 0x99999999,
};
Pat8 D_8013E158 = { { 0x7FF7F00F, 0xF00F7FF7 } };
Pat8 D_8013E160[3] = {
    { { 0x7AA7A00A, 0xA00A7AA7 } },
    { { 0x7AA7AAAA, 0xAAAA7AA7 } },
    { { 0x7AA7A99A, 0xA99A7AA7 } },
};
Pat8 D_8013E178[4] = {
    { { 0x77007F00, 0x00000000 } },
    { { 0x00000000, 0x7F007700 } },
    { { 0x00000000, 0x00F70077 } },
    { { 0x007700F7, 0x00000000 } },
};
Pat8 D_8013E198[4] = {
    { { 0x77007A00, 0x00000000 } },
    { { 0x00000000, 0x7A007700 } },
    { { 0x00000000, 0x00A70077 } },
    { { 0x007700A7, 0x00000000 } },
};
Pat8 D_8013E1B8[4] = {
    { { 0xFF00F000, 0x00000000 } },
    { { 0x00000000, 0xF000FF00 } },
    { { 0x00000000, 0x000F00FF } },
    { { 0x00FF000F, 0x00000000 } },
};
Pat8 D_8013E1D8[8] = {
    { { 0xFFFF0000, 0x00000000 } },
    { { 0xF0000000, 0x00000000 } },
    { { 0xF000F000, 0xF000F000 } },
    { { 0x00000000, 0x0000F000 } },
    { { 0x00000000, 0x0000FFFF } },
    { { 0x00000000, 0x0000000F } },
    { { 0x000F000F, 0x000F000F } },
    { { 0x000F0000, 0x00000000 } },
};
Pat8 D_8013E218[8] = {
    { { 0xA0A00000, 0x00000000 } },
    { { 0xA0000000, 0x00000000 } },
    { { 0xA0000000, 0xA0000000 } },
    { { 0x00000000, 0x00000000 } },
    { { 0x00000000, 0x00000A0A } },
    { { 0x00000000, 0x0000000A } },
    { { 0x0000000A, 0x0000000A } },
    { { 0x00000000, 0x00000000 } },
};
Pat8 D_8013E258 = { { 0xBBBBB00B, 0xB00BBBBB } };
Pat8 D_8013E260 = { { 0xCCCCC00C, 0xC00CCCCC } };
Pat8 D_8013E268 = { { 0x88888008, 0x80088888 } };
/* Neighbour offsets (dx, dy) clockwise from north. */
s32 D_8013E270[16] = {
    0, -1, -1, -1, -1, 0, -1, 1, 0, 1, 1, 1, 1, 0, 1, -1,
};

void func_8007D8E8(void) {
    s32 y;
    s32 x;
    u16 *dstBase;
    u16 *srcBase;

    y = 35;
    dstBase = func_80083908(D_8013DFDC);
    srcBase = func_80083908(D_8013DFDC ^ 1);
    D_801A8094 = D_801A808C;
    D_801A8094[0] = D_801A8094[1] = 0x99999999;
    do {
        x = 57;
        do {
            s32 mx = x + 9;
            s32 my = y + 9;
            u8 *dst = (u8 *)&dstBase[y * 232 + x];
            u32 cell = D_801D9358[mx][my];
            u32 bit = 1 << (x % 32);
            s32 word = y * 2 + (x >> 5);
            u8 *src = NULL; /* overlay pattern, or the previous frame's pixels */
            u8 *base;
            u8 *keep;

            if (cell & 0x800000) {
                if (cell & 0x10000) {
                    base = D_8013E0D0.b;
                    if (cell & 0x80) base = D_8013E0D8.b;
                } else {
                    s32 i;
                    s32 *o;

                    base = D_8013E0C8.b;
                    D_801A8094[0] = D_801A8094[1] = D_8013E0C8.w[0];
                    o = D_8013E270;
                    if (cell & 0xE100) {
                        for (i = 0; i < 8; i++) {
                            u32 nb = D_801D9358[mx + o[0]][my + o[1]];
                            o += 2;
                            if ((nb & 0x18120) == 0x10000) {
                                Pat8 *p;
                                if (cell & 0x2100) {
                                    p = &D_8013E218[i];
                                } else {
                                    p = &D_8013E1D8[i];
                                }
                                D_801A8094[0] |= p->w[0];
                                D_801A8094[1] |= p->w[1];
                            }
                        }
                        if (cell & 0x200) {
                            u32 n = 0;
                            for (i = 1; i < 8; i += 2) {
                                s32 a = (i + 1) % 8 * 2;
                                s32 b = (i + 7) % 8 * 2;
                                /* low flag halves of the two diagonal-adjacent neighbours */
                                u16 v = D_801D9358[mx + D_8013E270[a]][my + D_8013E270[a + 1]]
                                      | D_801D9358[mx + D_8013E270[b]][my + D_8013E270[b + 1]];
                                if (v & 0x400) {
                                    if (!(v & 0xE120)) {
                                        Pat8 *m = &D_8013E1B8[i / 2];
                                        Pat8 *p;
                                        if (cell & 0x2100) {
                                            p = &D_8013E198[i / 2];
                                        } else {
                                            p = &D_8013E178[i / 2];
                                        }
                                        n++;
                                        D_801A8094[0] = (~m->w[0] & D_801A8094[0]) | p->w[0];
                                        D_801A8094[1] = (~m->w[1] & D_801A8094[1]) | p->w[1];
                                    }
                                }
                            }
                            if (n >= 4) {
                                Pat8 *p = &D_8013E158;
                                if (cell & 0x2100) p = &D_8013E160[0];
                                D_801A8094[0] = p->w[0];
                                D_801A8094[1] = p->w[1];
                            }
                        }
                        base = (u8 *)D_801A8094;
                    }
                }
            } else {
                base = NULL;
            }

            if (base != NULL) {
                keep = D_8013E0B8[0].b;
                if (func_8005CB7C() == 0) {
                    if (cell & 0x40000) {
                        src = NULL;
                    } else if (cell & 0x80000) {
                        src = D_8013E0E8.b;
                        keep = D_8013E0A0.b;
                    } else if (cell & 0x4000000) {
                        src = D_8013E0F0.b;
                        keep = D_8013E0A0.b;
                    } else if (cell & 0x200000) {
                        src = D_8013E100.b;
                        keep = D_8013E0B0.b;
                    } else if (cell & 0x400000) {
                        src = D_8013E0E0.b;
                        keep = D_8013E0A0.b;
                    } else if (cell & 0x2000000) {
                        src = D_8013E0F8.b;
                        keep = D_8013E0A0.b;
                    } else if (cell & 0x100000) {
                        src = D_8013E258.b;
                        keep = D_8013E0A8.b;
                    } else if (cell & 0x20000000) {
                        src = D_8013E260.b;
                        keep = D_8013E0A8.b;
                    } else if (cell & 0x10000000) {
                        src = D_8013E268.b;
                        keep = D_8013E0A8.b;
                    }
                }
                if (src != NULL || base != NULL) {
                    if (src == NULL) src = D_8013E0C8.b;
                    if (base == NULL) base = D_8013E0C8.b;
                    dst[0] = src[0] | (base[0] & keep[0]);
                    dst[1] = src[1] | (base[1] & keep[1]);
                    dst[0x74] = src[2] | (base[2] & keep[2]);
                    dst[0x75] = src[3] | (base[3] & keep[3]);
                    dst[0xE8] = src[4] | (base[4] & keep[4]);
                    dst[0xE9] = src[5] | (base[5] & keep[5]);
                    dst[0x15C] = src[6] | (base[6] & keep[6]);
                    dst[0x15D] = src[7] | (base[7] & keep[7]);
                }
                D_801D9358[mx][my] &= ~0x800000;
                D_801A7F50[word] |= bit;
            } else {
                if (D_801A7F50[word] & bit) {
                    src = (u8 *)&srcBase[y * 232 + x];
                    dst[0] = src[0];
                    dst[1] = src[1];
                    dst[0x74] = src[0x74];
                    dst[0x75] = src[0x75];
                    dst[0xE8] = src[0xE8];
                    dst[0xE9] = src[0xE9];
                    dst[0x15C] = src[0x15C];
                    dst[0x15D] = src[0x15D];
                }
                D_801A7F50[word] &= ~bit;
            }
        } while (--x >= 0);
    } while (--y >= 0);
}
