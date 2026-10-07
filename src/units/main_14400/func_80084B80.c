#include "common.h"

typedef unsigned short u16;
typedef struct { u16 first; u16 second; } Pair;
extern Pair D_801BF258[];
extern s32 D_801BF2C4;
extern s32 D_8013E900;
extern s32 D_801BF2C0;
void func_80084B80(void) {
    if (D_801BF2C4 != 0) {
        D_801BF2C4--;
        D_8013E900 = D_801BF258[D_801BF2C4].first;
        D_801BF2C0 = D_801BF258[D_801BF2C4].second;
    }
}
