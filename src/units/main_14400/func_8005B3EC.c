#include "common.h"

typedef struct { float x, y, z; } Vec3;
typedef struct { char pad0[0xC]; Vec3 eye, at; float roll, fov; } CamState;
extern Vec3 D_8016530C;
extern Vec3 D_80165324;
extern float D_8016533C;
extern float D_80165340;

void func_8005B3EC(CamState *state) {
    D_8016530C = state->eye;
    D_80165324 = state->at;
    D_8016533C = state->roll;
    D_80165340 = state->fov;
}
