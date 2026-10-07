#include "common.h"

typedef float f32;
typedef struct { f32 x, y, z; } Vec3f;
typedef struct {
    Vec3f eye;
    Vec3f at;
    f32 field_18;
    f32 field_1C;
} Camera8005935C;

extern Vec3f D_8016530C;
extern Vec3f D_80165324;
extern f32 D_8016533C;
extern f32 D_80165340;
extern Camera8005935C D_80165364;
extern s32 D_80165384;

void func_8005935C(void) {
    Camera8005935C *cam = &D_80165364;

    cam->eye = D_8016530C;
    cam->at = D_80165324;
    D_80165384 = 1;
    cam->field_18 = D_8016533C;
    cam->field_1C = D_80165340;
}
