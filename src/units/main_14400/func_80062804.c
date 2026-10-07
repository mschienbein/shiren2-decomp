#include "common.h"

typedef float f32;

extern u32 D_8016DB18;
extern s32 D_8016DB1C;
extern s32 D_8016DB20;
extern f32 D_8016DB24;
extern s32 D_8013B800;
extern f32 *D_8013B804; /* float grid pointer (func_80064A70 result); cleared here */
extern s32 D_801DE9AC;
extern s32 D_801DE9B0;
extern s32 D_801DEAAC;
extern s32 D_801E4E78;
void func_80067830(s32 arg);
void func_80061724(void);
void func_800626C4(void);
void func_8007D788(void);
void func_8007D32C(void);

void func_80062804(u32 mode, s32 arg1, s32 arg2, s32 arg3) {
    D_8016DB20 = 0;
    switch (mode) {
        case 2:
            D_8016DB18 = mode;
            D_8016DB20 = arg2;
            break;
        case 1:
        case 4:
            D_8016DB18 = mode;
            break;
        case 3:
            D_8016DB18 = mode;
            func_80067830(arg3);
            break;
        default:
            D_8016DB18 = 0;
            break;
    }
    D_8016DB1C = arg1;
    D_8013B800 = 0;
    D_8013B804 = 0;
    D_801DE9B0 = 0xFF;
    D_801E4E78 = 0xFF;
    D_801DE9AC = 0xFF;
    D_801DEAAC = 0;
    D_8016DB24 = 1.0f;
    func_80061724();
    func_800626C4();
    func_8007D788();
    func_8007D32C();
}
