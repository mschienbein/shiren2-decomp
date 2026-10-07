#include "row_views.h"
typedef struct { u32 w0; u32 w1; } Gfx;
void func_80080558(Gfx **gfx, s32 a, s32 b, s32 c, s32 d, s32 *list, s32 count, s32 e);
void func_80080868(Gfx **gfx, s32 a, s32 b, s32 c, s32 d, s32 *list, s32 count, s32 e);
void func_80080E04(Gfx **gfx, s32 id, s32 d, s32 e) {
    s32 list[10];
    s32 count = 0;
    s32 i;
    for (i = 0; i < 10; i++) {
        if (D_801A9080[i].field_02 == id) {
            list[count++] = i;
        }
    }
    if (count == 0) {
        return;
    }
    {
        Gfx *g = (*gfx)++;
        g->w0 = 0xE7000000;
        g->w1 = 0;
    }
    {
        Gfx *g = (*gfx)++;
        g->w0 = 0xFC119623;
        g->w1 = 0xFF2FFFFF;
    }
    func_80080558(gfx, 0, 0, 8, d, list, count, e);
    func_80080558(gfx, 1, 0, 9, d, list, count, e);
    func_80080558(gfx, 0, 1, 12, d, list, count, e);
    func_80080558(gfx, 1, 1, 13, d, list, count, e);
    func_80080868(gfx, 0, 0, 0, d, list, count, e);
    func_80080868(gfx, 0, 1, 4, d, list, count, e);
    func_80080868(gfx, 1, 0, 2, d, list, count, e);
    func_80080868(gfx, 1, 1, 3, d, list, count, e);
    {
        Gfx *g = (*gfx)++;
        g->w0 = 0xE7000000;
        g->w1 = 0;
    }
    {
        Gfx *g = (*gfx)++;
        g->w0 = 0xFCFFFFFF;
        g->w1 = 0xFFFCF279;
    }
}
