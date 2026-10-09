#include "common.h"
typedef unsigned char u8;
typedef struct RenderContext RenderContext;
extern char D_801D8590[], D_80169A38[], D_801E4E48[];
extern s32 D_80169A44;
extern u8 *D_80169A54; /* saved CPU frame copy (func_8006B580 buffer) */
extern void *D_8013B748; /* resource pointer stored by func_8005F1D4 (func_8006ABC4 result) */
extern void func_80027EA0(void *queue, void *messages, long capacity), func_80034200(u32), func_8006E8E0(void *);
extern void func_8005DC90(void), func_8007F130(void), func_800538E0(void), func_80058EF0(void), func_80069D70(void), func_8005C6D0(void), func_800740F0(void), func_8007D720(void), func_80061290(void), func_8008BE00(void), func_8006E7B0(void), func_80055400(void), func_8005CBE8(void), func_80054D20(void), func_80056E20(void), func_800725B0(void), func_8006CD14(void), func_8005F1D4(void), func_8005DC9C(void), func_8007F2D8(void);
extern s32 func_8005F210(RenderContext *), func_8005F7E0(RenderContext *);
extern void func_8006B184(s32 (*)(RenderContext *)), func_8006B1B0(s32 (*)(RenderContext *));
void func_8005F0B0(void) {
    D_80169A44 = 0; D_8013B748 = 0; D_80169A54 = 0;
    func_80027EA0(D_801D8590, D_80169A38, 1);
    func_80034200(0x40); func_80034200(10); func_8006E8E0(D_801E4E48);
    func_8005DC90(); func_8007F130(); func_800538E0(); func_80058EF0(); func_80069D70();
    func_8005C6D0(); func_800740F0(); func_8007D720(); func_80061290(); func_8008BE00();
    func_8006E7B0(); func_80055400(); func_8005CBE8(); func_80054D20(); func_80056E20();
    func_800725B0(); func_8006CD14(); func_8005F1D4(); func_8005DC9C(); func_8007F2D8();
    func_8006B184(func_8005F210); func_8006B1B0(func_8005F7E0);
}
