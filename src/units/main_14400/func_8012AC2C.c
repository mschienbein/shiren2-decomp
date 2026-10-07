#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

extern s32 D_801CA714;
extern s32 D_801CA718;
extern s32 D_801CA71C;
extern u8 *D_801CA720;
void func_8012D8C8(void *destination, void *source, u32 count);
s32 func_8012AC2C(void *item) {
    s32 next = (D_801CA718 + 1) % D_801CA71C;
    if (next == D_801CA714) return 0;
    func_8012D8C8(D_801CA720 + D_801CA718 * 8, item, 8);
    D_801CA718 = next;
    return 1;
}
