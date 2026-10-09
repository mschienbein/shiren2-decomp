#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u32 w0, w1; } Gfx;
typedef struct {
    u16 field00, id02, field04, x06, y08; u8 pad0A[0xA];
    u16 layer14, dx16, dy18; u8 pad1A[0x12];
} Tile;
extern u8 D_801A8BA0[30][40];
extern u16 D_801A906C[10];
extern Tile D_801A9080[10];
extern const u8 D_6000000[];

static inline s32 texture_bank(s32 alternate)
{
    return alternate != 0;
}

/* `unused`: the only caller (func_8007F350) passes 0 in a2 (0x800803EC); it is
 * never read. */
void func_80081040(Gfx **display, s32 id, s32 unused)
{
    s32 selected[10];
    s32 count = 0;
    s32 i, j;
    s32 width, height;
    for (i = 0; i < 10; i++) {
        if (D_801A9080[i].id02 == id) {
            selected[count++] = i;
        }
    }
    if (count != 0) {
        height = 8;
        for (i = 1, width = 8; i < 2; i++) {
            for (j = 0; j < count; j++) {
                s32 index = selected[j];
                Tile *tile = &D_801A9080[index];
                if (tile->layer14 == i) {
                    Gfx *cmd;
                    s32 alternate = D_801A906C[index];
                    s32 x, y, x0, y0, x1, y1;
                    { Gfx *packet = (*display)++; packet->w0 = 0xFD500000; packet->w1 = (u32)D_6000000; }
                    { Gfx *packet = (*display)++; packet->w0 = 0xF5500000; packet->w1 = 0x07000000; }
                    { Gfx *packet = (*display)++; packet->w0 = 0xE6000000; packet->w1 = 0; }
                    { Gfx *packet = (*display)++; packet->w0 = 0xF3000000; packet->w1 = 0x0713F400; }
                    { Gfx *packet = (*display)++; packet->w0 = 0xE7000000; packet->w1 = 0; }
                    { Gfx *packet = (*display)++; packet->w0 = 0xF5400400; packet->w1 = texture_bank(alternate) << 20; }
                    { Gfx *packet = (*display)++; packet->w0 = 0xF2000000; packet->w1 = 0x0007C09C; }
                    x = tile->x06 * 8 + D_801A9080[index].dx16;
                    y = tile->y08 * 8 + D_801A9080[index].dy18;
                    x0 = x / 8; y0 = y / 8;
                    x1 = (x + width - 1) / 8; y1 = (y + height - 1) / 8;
                    if ((D_801A8BA0[y0][x0] & 15) == index &&
                        (D_801A8BA0[y0][x1] & 15) == index &&
                        (D_801A8BA0[y1][x0] & 15) == index &&
                        (D_801A8BA0[y1][x1] & 15) == index) {
                        u32 right = (((x + width) * 4) & 0xFFF) << 12;
                        u32 bottom;
                        cmd = (*display)++;
                        bottom = (((y + height) * 4) & 0xFFF) | 0xE4000000;
                        cmd->w0 = right | bottom;
                        cmd->w1 = (((x * 4) & 0xFFF) << 12) | ((y * 4) & 0xFFF);
                        { Gfx *packet = (*display)++; packet->w0 = 0xE1000000; packet->w1 = 0x400; }
                        { Gfx *packet = (*display)++; packet->w0 = 0xF1000000; packet->w1 = 0x04000400; }
                    }
                }
            }
        }
    }
}
