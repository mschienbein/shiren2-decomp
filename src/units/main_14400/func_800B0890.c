#include "common.h"
typedef unsigned char u8;
extern u8 D_8014313C[5];
extern u8 D_80143144[40][4];
extern const u8 D_80153B14[5][4];
extern s32 D_80143110;
extern u8 D_80143114[40];
static inline s32 in_range(s32 index, s32 count)
{
    return index < count;
}
void func_800B0890(void)
{
    u8 *p = D_8014313C;
    s32 count = 4;
    s32 i, j;
    s32 more;
    do {
        *p++ = 0;
    } while (count-- > 0);
    for (i = 0; ; i++) {
        more = in_range(i, 5);
        if (!more) break;
        for (j = 0; j < 4; j++) {
            D_80143144[i][j] = D_80153B14[i][j];
        }
    }
    D_80143110 = 5;
    for (count = 5; count < 40; count++) {
        D_80143144[count][0] = 0;
    }
    for (count = 0; ; count++) {
        more = in_range(count, 40);
        if (!more) break;
        D_80143114[count] = count + 1;
    }
}
