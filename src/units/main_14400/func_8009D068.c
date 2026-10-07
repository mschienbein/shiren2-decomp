#include "common.h"

typedef unsigned short u16;
typedef struct { s32 w[6]; } Info;
typedef struct { s32 slot; Info info; s32 extra; } Found;
typedef struct { char pad[0x350]; Found found[3]; } Ctx;
extern s32 D_801E7894;
extern s32 D_801E7898[];
extern Info D_801E78A4[];
extern s32 D_8013960C;
extern void func_80048240(u16, ...);
extern void func_80048464(void);
extern void func_80048380(void);
extern s32 func_801E63E8(unsigned char);
s32 func_8009D068(Ctx *ctx) {
    s32 count = 0;
    s32 i;
    s32 state;
    if (D_801E7894 != 0) {
        func_80048240(0x504);
        func_80048240(0x506);
        func_80048464();
    }
    i = 0;
    while (1) {
        if (i >= 3) break;
        state = func_801E63E8(i);
        if (state == 2) return -1;
        if (state == 3) {
            ctx->found[count].slot = i;
            ctx->found[count].info = D_801E78A4[i];
            ctx->found[count].extra = D_801E7898[i];
            count++;
        }
        i++;
    }
    if (D_801E7894 != 0) {
        D_8013960C = 0;
        func_80048380();
    }
    return count;
}
