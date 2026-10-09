#include "common.h"
typedef struct { unsigned char fields00[7]; unsigned char field07; unsigned char fields08[0xC]; float field14; } Target;
typedef struct { s32 field00; unsigned short field04; unsigned short field06; unsigned short field08; unsigned short field0A; s32 fields0C[2]; s32 field14; s32 field18; s32 field1C; unsigned char fields20[0x10]; float field30; unsigned char fields34[0x14]; float field48; } State;
typedef struct { float values[8]; } Camera;
extern Target *func_8007946C(s32 zero, s32 id);
extern void func_80059728(Camera *camera);
extern s32 func_8005B07C(void);
extern void func_8005A464(s32 id, void *params, s32 y, s32 z);
extern void func_8005ABBC(Camera *camera, s32 x, s32 y);
void func_80088FD4(State *state) {
    Camera camera;
    s32 phase;
    Target *target;
    target = func_8007946C(0, state->field14);
    phase = state->field08;
    switch (phase) {
    case 0:
        target->field07 = 2;
        state->field30 = target->field14;
        state->field1C = 5;
        state->field08++;
        return;
    case 1:
        if (state->field1C-- != 0) {
            target->field14 += 1.5f;
            return;
        }
        func_80059728(&camera);
        state->field18 = func_8005B07C();
        func_8005A464(-1, 0, 0, 0);
        state->field48 = camera.values[1];
        target->field07 = phase;
        target->field14 = state->field30;
        state->field1C = 4;
        state->field0A = phase;
        state->field08++;
        return;
    case 2:
        if (--state->field0A == 0xFFFF) {
            if (state->field1C-- != 0) {
                func_80059728(&camera);
                camera.values[1] = state->field48 + (float)(state->field1C & 1U) * 10.0f - 5.0f;
                func_8005ABBC(&camera, 0, 0);
                state->field0A++;
                return;
            }
            func_80059728(&camera);
            camera.values[1] = state->field48;
            func_8005ABBC(&camera, 0, 0);
            func_8005A464(state->field18, 0, 0, 0);
            state->field04 = 4;
        }
        break;
    }
}
