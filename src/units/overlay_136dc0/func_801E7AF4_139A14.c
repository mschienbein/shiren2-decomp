#include "func_801E7AF4_139A14/types.h"

extern s32 D_801F5130_147050;
s32 D_801F5134_147054 = -1;
extern s32 D_801F5138_147058;
extern s32 D_801F513C_14705C;
extern u8 D_801F5129_147049;
extern s32 func_80041FF8(void);
extern s32 func_8006E3D0(void);
extern s32 func_80042150(void);
extern void func_801E5EAC_137DCC(void);
extern void func_801E6264_138184(void);
extern void func_801E5D6C_137C8C(void);
extern void func_801E70F8_139018(void);
extern void func_801E6E70_138D90(void);
extern void func_801E74C0_1393E0(void);
extern void func_801E7878_139798(void);
extern void func_801E795C_13987C(void);
extern void func_801E7FF8_139F18(void);
extern void func_801E7E2C_139D4C(void);
extern void func_801E8394_13A2B4(void);
extern void func_801E8834_13A754(void);
extern void func_801E76CC_1395EC(void);
extern void func_801E6418_138338(void);

void func_801E7AF4_139A14(void) {
    if ((u8)func_80041FF8() != 0) {
        D_801F5138_147058 = func_8006E3D0();
        if (D_801F5134_147054 != D_801F5138_147058 && D_801F5138_147058 != -1) {
            D_801F5130_147050 = D_801F5138_147058;
        }
        D_801F5134_147054 = D_801F5138_147058;
        if (D_801F5138_147058 != -1) {
            D_801F513C_14705C = D_801F5138_147058;
        }
        func_801E5EAC_137DCC();
        func_801E6264_138184();
        func_801E5D6C_137C8C();
        func_801E70F8_139018();
        func_801E6E70_138D90();
        func_801E74C0_1393E0();
        func_801E7878_139798();
        func_801E795C_13987C();
        func_80042150();
        func_801E7FF8_139F18();
        func_801E7E2C_139D4C();
        func_801E8394_13A2B4();
        func_801E8834_13A754();
        func_801E76CC_1395EC();
        if (D_801F5129_147049 != 0) {
            func_801E6418_138338();
        }
    }
}
