#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad00[6];
    u16 x;
    u16 y;
    u16 width;
    u16 rows;
} Rect800828FC;

extern Rect800828FC *D_8013E81C;
extern s32 D_8013E8E4;
extern u8 *D_801A9050[];
extern void func_800837F4(void);

void func_800828FC(void)
{
    Rect800828FC *rect = D_8013E81C;
    s32 width = rect->width;
    s32 offset = (rect->y * 320 + rect->x) * 4;
    s32 rows = rect->rows * 8;
    s32 row;
    s32 col;
    u32 *dst;

    for (row = 0; row < rows; row++) {
        dst = (u32 *)(D_801A9050[D_8013E8E4] + offset);
        for (col = 0; col < width; col++) {
            *dst++ = 0;
        }
        offset += 0xA0;
    }
    func_800837F4();
}
