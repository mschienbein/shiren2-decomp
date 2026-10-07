#include "common.h"

typedef float f32;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct {
    char pad0[0xC];
    Vec3 eye;
    Vec3 at;
    f32 unk24;
    f32 unk28;
} View8005A950;

typedef struct {
    Vec3 pos;
    f32 unkC;
    f32 unk10;
} Preset8005A950;

/* Whole 0x20-byte camera block D_80165364 (see func_80058EF0/func_8005935C); the preset
 * read here is its member at +0xC (splat's interior label D_80165370). */
typedef struct {
    Vec3 eye;
    Preset8005A950 preset;
} Camera8005A950;

extern s32 D_80165394;
extern s32 D_80165398;
extern s32 D_80165400;
extern s32 D_801653F8;
extern s32 D_801653FC;
extern s32 D_80165404;
extern s32 D_8013B680[];
extern s32 D_8013B6A0[];
extern View8005A950 D_801653A0;
extern View8005A950 D_801653CC;
extern Vec3 D_8016530C;
extern Vec3 D_80165324;
extern f32 D_8016533C;
extern f32 D_80165340;
extern Vec3 D_8016534C;
extern Vec3 D_80165358;
extern Camera8005A950 D_80165364;

void func_80059590(s32 arg0);
void func_8005C4AC(Vec3 *eye, Vec3 *at);
s32 func_80062554(s32 x, s32 z);
void func_800265E0(void *ptr, s32 size);
s32 func_8005B2DC(View8005A950 *from, View8005A950 *to);
void func_8005B3EC(View8005A950 *view);

void func_8005A950(s32 dir, f32 arg1, s32 arg2) {
    Vec3 eye;
    Vec3 at;
    s32 x;
    s32 z;
    f32 mid;

    if (D_80165394 == 4) {
        return;
    }
    func_80059590(2);
    D_80165394 = 1;
    D_80165398 = 0;
    D_80165400 = 0;
    func_8005C4AC(&eye, &at);
    x = (s32)at.x >> 5;
    z = (s32)at.z >> 5;
    mid = (func_80062554(x, z) + func_80062554(x + D_8013B680[dir], z + D_8013B6A0[dir])) * 0.5f;
    func_800265E0(&D_801653A0, 0x2C);
    func_800265E0(&D_801653CC, 0x2C);
    D_801653A0.eye = D_8016530C;
    D_801653A0.at = D_80165324;
    D_801653A0.unk24 = D_8016533C;
    D_801653A0.unk28 = D_80165340;
    D_801653CC.eye.x = D_8013B680[dir] * 16.0f;
    D_801653CC.eye.z = D_8013B6A0[dir] * 16.0f;
    D_801653CC.eye.y = mid + 10.0f;
    D_801653CC.at = D_80165364.preset.pos;
    D_801653CC.unk24 = arg1;
    D_801653CC.unk28 = D_80165364.preset.unk10;
    D_801653F8 = arg2;
    D_801653FC = 0;
    D_80165404 = 1;
    if (arg2 == 0 || func_8005B2DC(&D_801653A0, &D_801653CC)) {
        func_8005B3EC(&D_801653CC);
        D_801653F8 = 0;
        func_8005C4AC(&D_8016534C, &D_80165358);
    }
}
