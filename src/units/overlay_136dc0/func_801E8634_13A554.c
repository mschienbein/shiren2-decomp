#include "func_801E8634_13A554/types.h"
extern s32 func_800627C4(void);
extern s32 func_800627D4(void);
extern void func_80067578(float *x, float *y, float *z);
extern void func_800675EC(float x, float y, float z);
void func_801E8634_13A554(s32 dx, s32 dy, s32 dz) {
    float x, y, z;
    if (func_800627C4() == 1 && func_800627D4() == 16) {
        if (!dx && !dy && !dz) {
            x = 320.0f;
            y = 0.0f;
            z = 320.0f;
            func_800675EC(x, y, z);
        } else {
            func_80067578(&x, &y, &z);
            x += dx;
            z += dz;
            y += dy;
            func_800675EC(x, y, z);
        }
    }
}
