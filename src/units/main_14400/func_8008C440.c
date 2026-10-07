#include "common.h"
extern s32 D_8013FEE0;
extern unsigned char *D_8013FEE4;
float func_8008C440(s32 index) {
    if (!D_8013FEE0)
        return 0.0f;
    else
        return *(float *)(D_8013FEE4 + index * 0x78 + 0x10C);
}
