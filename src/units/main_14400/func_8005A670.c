#include "common.h"
typedef float f32;
typedef struct { f32 x, y, z; } Vec3;
typedef struct { Vec3 eye; Vec3 at; f32 fov; f32 roll; } CamSave;
/* The saved view is +0xC in the whole 0x2C-byte camera copied by func_8005ADAC. */
typedef struct { Vec3 field_00; CamSave view; } SavedCamera;
typedef struct { s32 unk0[3]; Vec3 eye; Vec3 at; f32 fov; f32 roll; } CamKey;
typedef struct { Vec3 at; f32 fov; f32 roll; } CamDefault;
/* Whole 0x20-byte camera block D_80165364 (see func_80058EF0/func_8005935C); the default
 * view read here is its member at +0xC (splat's interior label D_80165370). */
typedef struct { Vec3 eye; CamDefault def; } CamBlock;
extern s32 D_80165394;
extern s32 D_80165398;
extern s32 D_80165400;
extern s32 D_80165408;
extern s32 D_801653F8;
extern s32 D_801653FC;
extern s32 D_80165404;
extern Vec3 D_8016530C;
extern Vec3 D_80165324;
extern f32 D_8016533C;
extern f32 D_80165340;
extern SavedCamera D_8016540C;
extern CamKey D_801653A0;
extern CamKey D_801653CC;
extern CamBlock D_80165364;
extern f32 D_8013B154[];
extern f32 D_8013B148[];
extern char D_8016534C[];
extern char D_80165358[];
void func_8005ADAC(s32, s32, s32);
void func_80059590(s32);
void func_800265E0(void *, s32);
s32 func_8005B2DC(CamKey *, CamKey *);
void func_8005B3EC(CamKey *);
void func_8005C4AC(void *, void *);
void func_8005A670(s32 mode, s32 save, s32 instant) {
    CamKey *from;
    CamKey *to;
    D_80165398 = mode;
    if (D_80165394 == 4) return;
    if (mode == 4) {
        func_8005ADAC(8, 1, 1);
        return;
    }
    func_80059590(2);
    if (save && D_80165394 == 1) {
        CamSave *s = &D_8016540C.view;
        D_80165408 = D_80165394;
        s->eye = D_8016530C;
        s->at = D_80165324;
        s->fov = D_8016533C;
        s->roll = D_80165340;
    }
    from = &D_801653A0;
    D_80165394 = 1;
    D_80165400 = 0;
    func_800265E0(from, 0x2C);
    to = &D_801653CC;
    func_800265E0(to, 0x2C);
    from->eye = D_8016530C;
    from->at = D_80165324;
    from->fov = D_8016533C;
    from->roll = D_80165340;
    if ((u32)mode < 3) {
        CamDefault *d = &D_80165364.def;
        to->eye.x = 0.0f;
        to->eye.y = D_8013B154[mode];
        to->eye.z = 0.0f;
        to->at = d->at;
        {
            f32 roll = d->roll;
            f32 fov = D_8013B148[mode];
            to->roll = roll;
            to->fov = fov;
        }
    } else {
        CamDefault *d = &D_80165364.def;
        to->eye.x = 0.0f;
        to->eye.y = 0.0f;
        to->eye.z = 0.0f;
        to->at = d->at;
        to->fov = d->fov;
        to->roll = d->roll;
    }
    if (D_801653CC.fov <= D_801653A0.fov || instant) {
        D_801653F8 = 0;
    } else {
        D_801653F8 = 8;
        D_801653FC = 0;
        D_80165404 = 1;
    }
    if (D_801653F8 == 0 || func_8005B2DC(&D_801653A0, &D_801653CC)) {
        func_8005B3EC(&D_801653CC);
        D_801653F8 = 0;
        func_8005C4AC(D_8016534C, D_80165358);
    }
}
