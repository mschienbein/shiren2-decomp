#include "common.h"
typedef struct Vec3 { float x, y, z; } Vec3;
/* Whole 0x20-byte camera block: D_80165370 is its position member at +0x0C. */
typedef struct CameraState { Vec3 origin, position; float pitch, yaw; } CameraState;
extern s32 D_80165394, D_80165400, D_80165408, D_801653F8;
extern Vec3 D_8016530C, D_80165324, D_8016534C, D_80165358;
extern CameraState D_80165364;
extern float D_8016533C, D_80165340;
extern void func_8005C4AC(void *, void *);
void func_8005A234(void) {
    if (D_80165394 != 4) D_80165394 = 0;
    D_80165400 = 0;
    D_80165408 = 0;
    D_8016530C.x = 0;
    D_8016530C.y = 0;
    D_8016530C.z = 0;
    D_80165324 = D_80165364.position;
    D_801653F8 = 0;
    D_8016533C = D_80165364.pitch;
    D_80165340 = D_80165364.yaw;
    func_8005C4AC(&D_8016534C, &D_80165358);
}
