#include "common.h"

void func_8008FC04(unsigned char *color, float *components) {
    u32 i;
    for (i = 0; i < 4; i++) {
        if (i == 3) {
            color[i] = (u32)((1.0f - components[3]) * 255.0f);
        } else {
            color[i] = (u32)(components[i] * 255.0f);
        }
    }
}
