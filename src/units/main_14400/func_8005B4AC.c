#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { float x, y, z; } Vector;
typedef float Matrix[4][4];
typedef union { long long alignment; u32 words[16]; } PackedMatrix;
typedef struct { Matrix projection, view; u8 field_80[0x80]; u16 normalize; u8 field_102[6]; } CameraBuffer;
extern u8 D_8013B140[], D_8013B145;
extern void (*D_8013B160[5])(void);
extern const double D_8014C1C0, D_8014C1C8, D_8014C1D0, D_8014C1D8;
extern Vector D_80165300, D_8016530C, D_80165318, D_80165324, D_80165330, D_8016534C, D_80165358, D_80165388;
extern float D_8016533C, D_80165340;
extern s32 D_80165344, D_80165348, D_80165394;
extern CameraBuffer D_80165450[2], *D_80165720;
extern Matrix D_801656E0;
extern PackedMatrix D_80165660[2], D_80165728[2], D_801657A8[2], D_80165828[2];
extern PackedMatrix *D_80165724, *D_801658A8, *D_801658AC, *D_801658B0;
extern void func_8005C4AC(Vector *, Vector *), func_800265E0(void *, s32);
extern void func_8002CE20(Matrix), func_80030E70(Matrix, float, float, float, float), func_8002CBC0(Matrix, Matrix, Matrix);
extern void func_8002CF00(Matrix, float, float, float, float *, float *, float *);
extern void func_8002D380(Matrix, u16 *, float, float, float, float, float);
extern void func_8002BF40(Matrix, float, float, float, float, float, float, float, float, float);
extern void func_8002CD40(Matrix, PackedMatrix *);
extern float func_8006A67C(float, float), __builtin_sqrtf(float);
void func_8005B4AC(void) {
    Vector player, target, center;
    Matrix transform, rotation, pitch, yaw, combined;
    struct { Vector eye_offset, up; } basis;
    float distance, field_of_view;
    D_80165720 = &D_80165450[D_8013B145];
    func_8005C4AC(&player, &target);
    D_8013B160[D_80165394]();
    switch (D_8013B140[D_80165394]) {
    case 0: center = D_80165300; break;
    case 1:
        D_80165300 = player;
        center.x = D_80165300.x + D_8016530C.x;
        center.y = D_80165300.y + D_8016530C.y;
        center.z = D_80165300.z + D_8016530C.z;
        break;
    case 2:
        D_80165300 = target;
        center.x = D_80165300.x + D_8016530C.x;
        center.y = D_80165300.y + D_8016530C.y;
        center.z = D_80165300.z + D_8016530C.z;
        break;
    }
    center.x += D_80165388.x; center.y += D_80165388.y; center.z += D_80165388.z;
    func_800265E0(&D_80165388, 0xC);
    distance = D_8016533C >= 1.0f ? D_8016533C : 1.0f;
    field_of_view = D_80165340 < 1.0f ? 1.0f :
        (D_80165340 > 180.0f ? 180.0f : D_80165340);
    func_8002CE20(transform);
    func_80030E70(rotation, (double)D_80165324.z * D_8014C1C0, 0.0f, 0.0f, 1.0f);
    func_8002CBC0(transform, rotation, transform);
    func_80030E70(rotation, (double)D_80165324.x * D_8014C1C0, 1.0f, 0.0f, 0.0f);
    func_8002CBC0(transform, rotation, transform);
    func_80030E70(rotation, (double)D_80165324.y * D_8014C1C0, 0.0f, 1.0f, 0.0f);
    func_8002CBC0(transform, rotation, transform);
    func_8002CF00(transform, 0.0f, 0.0f, distance, &basis.eye_offset.x, &basis.eye_offset.y, &basis.eye_offset.z);
    func_8002CF00(transform, 0.0f, 10.0f, 0.0f, &basis.up.x, &basis.up.y, &basis.up.z);
    D_80165318.x = center.x + basis.eye_offset.x;
    D_80165318.y = center.y + basis.eye_offset.y;
    D_80165318.z = center.z + basis.eye_offset.z;
    D_80165330.y = func_8006A67C(basis.eye_offset.x, basis.eye_offset.z);
    D_80165330.y += D_80165330.y < 0.0f ? D_8014C1C8 : 0.0;
    D_80165330.x = func_8006A67C(-basis.eye_offset.y, __builtin_sqrtf(basis.eye_offset.z * basis.eye_offset.z + basis.eye_offset.x * basis.eye_offset.x));
    D_80165330.x += D_80165330.x < 0.0f ? D_8014C1D0 : 0.0;
    D_80165330.z = D_80165324.z;
    func_8002D380(D_80165720->projection, &D_80165720->normalize, field_of_view, 1.3333334f, (float)(u32)D_80165344, (float)(u32)(D_80165348 - 20), 1.0f);
    func_8002BF40(D_80165720->view, D_80165318.x, D_80165318.y, D_80165318.z, center.x, center.y, center.z, basis.up.x, basis.up.y, basis.up.z);
    func_8002CBC0(D_80165720->view, D_80165720->projection, D_801656E0);
    D_80165724 = &D_80165660[D_8013B145];
    func_8002CD40(D_801656E0, D_80165724);
    func_80030E70(pitch, (double)D_80165330.x * D_8014C1D8, 1.0f, 0.0f, 0.0f);
    func_80030E70(yaw, (double)D_80165330.y * D_8014C1D8, 0.0f, 1.0f, 0.0f);
    func_8002CBC0(pitch, yaw, combined);
    D_801658A8 = &D_80165728[D_8013B145];
    D_801658AC = &D_801657A8[D_8013B145];
    D_801658B0 = &D_80165828[D_8013B145];
    func_8002CD40(pitch, D_801658A8);
    func_8002CD40(yaw, D_801658AC);
    func_8002CD40(combined, D_801658B0);
    D_8016534C = player; D_80165358 = target;
    D_8013B145 ^= 1;
}
