#include "common.h"

typedef unsigned char u8;
typedef float f32;
typedef struct { f32 x; f32 y; f32 z; } Vec3f;
typedef struct { Vec3f eye; Vec3f at; f32 roll; f32 fov; } CamParams;
typedef struct { char pad0[0xC]; Vec3f eye; Vec3f at; f32 roll; f32 fov; } CamState;
typedef struct { Vec3f at; f32 roll; f32 fov; } CamDefault;
/* Whole 0x20-byte camera block D_80165364 (see func_80058EF0/func_8005935C); the default
 * view read here is its member at +0xC (splat's interior label D_80165370). */
typedef struct { Vec3f eye; CamDefault def; } CamBlock;
typedef struct { s32 data[4]; } CamWork;
extern s32 D_80165394;
extern s32 D_80165400;
extern s32 D_80165408;
extern s32 D_8016539C;
extern s32 D_801653F8;
extern s32 D_801653FC;
extern s32 D_80165404;
extern char D_8016534C[];
extern char D_80165358[];
extern u8 D_8013B140[];
extern Vec3f D_8016530C;
extern Vec3f D_80165324;
extern f32 D_8016533C;
extern f32 D_80165340;
extern CamBlock D_80165364;
extern CamState D_801653A0;
extern CamState D_801653CC;
void func_80059590(s32);
void func_800593F8(CamWork *);
void func_8005C4AC(void *, void *);
void func_800594C0(void *pos, s32 mode);
void func_800265E0(void *, s32);
s32 func_8005B2DC(CamState *, CamState *);
void func_8005B3EC(CamState *);
void func_8005A464(s32 mode, CamParams *params, s32 blend, s32 arg3) {
    CamWork work;
    CamState *from;
    CamState *to;
    f32 roll;
    f32 fov;

    func_80059590(1);
    D_80165394 = 0;
    D_80165400 = 0;
    D_80165408 = 0;
    func_800593F8(&work);
    D_8016539C = mode;
    func_8005C4AC(D_8016534C, D_80165358);
    func_800594C0(&work, D_8013B140[D_80165394]);
    from = &D_801653A0;
    func_800265E0(from, sizeof(CamState));
    to = &D_801653CC;
    func_800265E0(to, sizeof(CamState));
    roll = D_8016533C;
    fov = D_80165340;
    from->eye = D_8016530C;
    from->at = D_80165324;
    from->roll = roll;
    from->fov = fov;
    if (params != 0) {
        to->eye = params->eye;
        to->at = params->at;
        to->roll = params->roll;
        to->fov = params->fov;
    } else {
        CamDefault *def = &D_80165364.def;
        roll = def->roll;
        fov = def->fov;
        to->eye.x = 0.0f;
        to->eye.y = 0.0f;
        to->eye.z = 0.0f;
        to->at = def->at;
        to->roll = roll;
        to->fov = fov;
    }
    D_801653F8 = blend;
    D_801653FC = 0;
    D_80165404 = arg3;
    if (blend == 0 || func_8005B2DC(&D_801653A0, &D_801653CC) != 0) {
        func_8005B3EC(&D_801653CC);
        D_801653F8 = 0;
    }
}
