#include "common.h"
typedef struct { float x, y, z; } Vec3;
typedef struct { Vec3 eye; Vec3 at; float fov; float roll; } CamSave;
/* The saved view is +0xC in this whole 0x2C-byte camera. */
typedef struct { Vec3 field_00; CamSave view; } Camera;
/* Whole 0x20-byte camera preset block at D_80165364 (see func_80058EF0/func_8005935C);
 * the default view read here starts at +0xC (splat's interior label D_80165370). */
typedef struct { Vec3 field_00, field_0C; float field_18, field_1C; } Preset;
extern Vec3 D_8016530C, D_80165324, D_8016534C, D_80165358;
extern float D_8016533C, D_80165340;
extern Preset D_80165364;
extern Camera D_801653A0, D_801653CC, D_8016540C;
extern s32 D_80165394, D_801653F8, D_801653FC, D_80165400, D_80165404, D_80165408;
extern void func_80059590(s32 mode);
extern void func_800265E0(void *object, s32 bytes);
extern s32 func_8005B2DC(Camera *a, Camera *b);
extern void func_8005B3EC(Camera *camera);
extern void func_8005C4AC(void *a, void *b);
void func_8005ADAC(s32 duration, s32 mode, s32 reset) {
    if (D_80165394 != 4) {
        func_80059590(1);
        D_80165400 = 1;
        func_800265E0(&D_801653A0, 0x2C);
        func_800265E0(&D_801653CC, 0x2C);
        D_801653A0.view.eye = D_8016530C;
        D_801653A0.view.at = D_80165324;
        D_801653A0.view.fov = D_8016533C;
        D_801653A0.view.roll = D_80165340;
        if (D_80165408 && !reset) {
            D_80165394 = 1;
            D_801653CC = D_8016540C;
            D_801653F8 = duration;
            D_801653FC = 0;
            D_80165404 = mode;
            D_80165408 = 0;
        } else {
            D_80165394 = 0;
            D_801653CC.view.eye.x = 0;
            D_801653CC.view.eye.y = 0;
            D_801653CC.view.eye.z = 0;
            D_801653CC.view.at = D_80165364.field_0C;
            D_801653F8 = duration;
            D_801653FC = 0;
            D_80165404 = mode;
            D_80165408 = 0;
            D_801653CC.view.fov = D_80165364.field_18;
            D_801653CC.view.roll = D_80165364.field_1C;
        }
        if (!D_801653F8 || func_8005B2DC(&D_801653A0, &D_801653CC)) {
            func_8005B3EC(&D_801653CC);
            D_801653F8 = 0;
            D_80165400 = 0;
            func_8005C4AC(&D_8016534C, &D_80165358);
        }
    }
}
