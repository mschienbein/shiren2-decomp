#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef float f32;
typedef struct { f32 x; f32 y; f32 z; } Vec3f80058EF0;
typedef struct { Vec3f80058EF0 vec_0; Vec3f80058EF0 vec_C; f32 value_18; f32 value_1C; } View80058EF0;
extern View80058EF0 D_8013B120;
extern u8 D_80165300[];
extern s32 D_80165344;
extern s32 D_80165348;
extern u8 D_8016534C[];
extern u8 D_80165358[];
extern View80058EF0 D_80165364;
extern s32 D_80165384;
extern u8 D_80165388[];
extern s32 D_80165394;
extern s32 D_8016539C;
extern s32 D_80165448;
void func_800265E0(void *dst, s32 size);
void func_8005C4AC(void *a, void *b);
void func_8005A234(void);

void func_80058EF0(void) {
    D_80165384 = 0;
    D_80165364.vec_0 = D_8013B120.vec_0;
    D_80165364.vec_C = D_8013B120.vec_C;
    D_80165364.value_18 = D_8013B120.value_18;
    D_80165364.value_1C = D_8013B120.value_1C;
    D_80165448 = 0;
    func_800265E0(D_80165300, 0xC);
    D_8016539C = -1;
    func_8005C4AC(D_8016534C, D_80165358);
    D_80165394 = 0;
    func_8005A234();
    D_80165344 = 0x14;
    D_80165348 = 0x800;
    func_800265E0(D_80165388, 0xC);
}
