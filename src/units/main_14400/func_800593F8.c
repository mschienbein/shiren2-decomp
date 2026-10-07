#include "common.h"
typedef struct { float x, y, z; } Vector;
extern s32 D_80165394;
extern unsigned char D_8013B140[];
extern Vector D_80165300, D_8016530C, D_8016534C, D_80165358;
void func_800593F8(Vector *out) { switch (D_8013B140[D_80165394]) { case 0: *out = D_80165300; break; case 1: out->x = D_8016530C.x + D_8016534C.x; out->y = D_8016530C.y + D_8016534C.y; out->z = D_8016530C.z + D_8016534C.z; break; case 2: out->x = D_8016530C.x + D_80165358.x; out->y = D_8016530C.y + D_80165358.y; out->z = D_8016530C.z + D_80165358.z; break; } }
