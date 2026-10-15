#include "func_801E5564_137484/types.h"

void func_8005A234(void);
void func_80059728(ViewSrc *view);
f32 func_80059BF0(s32 value);
f32 func_80059B9C(s32 value);
void func_80059894(s32 value);
void func_8005ABBC(ViewSrc *view, s32 mode, s32 blend);
s32 func_8005B07C(void);
void func_8005A2E4(ViewSrc *view, s32 mode, s32 blend);
void func_8005A464(s32 mode, void *params, s32 blend, s32 arg3);
void func_8005B4AC(void);

void func_801E5564_137484(D_801E5564_arg *arg) {
    ViewSrc view;
    s32 blend;

    if (arg->x0 == 0) {
        return;
    }
    if (D_801F5128_147048) {
        func_8005A234();
    }
    D_801F5128_147048 = 0;
    func_80059728(&view);
    if (arg->xC != -1) {
        view.at.y = func_80059BF0((u8)arg->xC);
    }
    if (arg->xE != -1) {
        view.at.x = func_80059B9C((u8)arg->xE);
    }
    if (D_801F512C_14704C < 0) {
        view.eye.x = arg->x10 * 32 + 16 + arg->x4;
        view.eye.z = arg->x12 * 32 + 16 + arg->x8;
    } else {
        view.eye.x = arg->x4;
        view.eye.z = arg->x8;
    }
    if (arg->x6 != -12345) {
        view.eye.y = arg->x6;
    }
    if (arg->xA != -1) {
        view.roll = arg->xA;
    }
    view.fov = arg->x14;
    blend = D_801F512A_14704A != 0;
    func_80059894(0);
    if (D_801F512C_14704C == -1) {
        func_8005ABBC(&view, arg->x2, blend);
    } else if (D_801F512C_14704C == func_8005B07C()) {
        func_8005A2E4(&view, arg->x2, blend);
        return;
    } else {
        func_8005A464(D_801F512C_14704C, &view, arg->x2, blend);
    }
    if (arg->x2 == 0) {
        func_8005B4AC();
    }
}
