#include "common.h"

typedef struct { float x, y, z; } Vec3;

extern Vec3 D_80165300;
s32 func_80041FF8(void);
void func_8005C4AC(Vec3 *position, Vec3 *copy);
extern float __builtin_sqrtf(float value);

/* Distance from the camera target to map cell (x, z) and the stereo pan
 * (0..255) derived from the clamped horizontal offset. */
void func_8005A048(s32 x, s32 z, s32 *distance, s32 *balance) {
    Vec3 position, target;
    float dx, dy, dz, length, pan;
    s32 cx = (x << 5) + 16;
    s32 cz = z << 5;

    if (func_80041FF8() & 0xFF) {
        dx = D_80165300.x - (float)cx;
        dy = D_80165300.y - 0.0f;
        dz = D_80165300.z - (float)cz;
    } else {
        func_8005C4AC(&position, &target);
        dx = target.x - (float)cx;
        dy = target.y - 0.0f;
        dz = target.z - (float)cz;
    }
    length = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    *distance = (s32)length;
    dx = (double)dx + 800.0;
    if ((double)dx < 100.0) dx = 100.0f;
    else if ((double)dx > 1500.0) dx = 1500.0f;
    pan = ((double)dx * 256.0) / 1600.0;
    pan = 256.0f - pan;
    *balance = (s32)pan % 256;
}
